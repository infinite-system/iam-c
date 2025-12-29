#!/usr/bin/env node
/* iamc.ts - v1 IAM-C codegen
 *
 * Run:
 *   node --experimental-strip-types iamc.ts gen -ns iam mc
 *   node --experimental-strip-types iamc.ts gen
 *
 * Scans:
 *   - Always: src/app/**
 *   - Namespaces: src/mod/<ns>/** for each -ns
 * Also pulls in ONLY referenced headers via: #include "mod/.../*.h"
 *
 * Generates:
 *   - <Class>.inherits.h for missing inherited methods (instance + eligible statics)
 *   - Injects #include "mod/.../<Class>.inherits.h" into <Class>.c if needed
 *   - Removes include and deletes file if nothing to generate
 */

import * as path from "node:path";
import * as fs from "node:fs/promises";

type MethodKind = "instance" | "static";

type Method = {
  kind: MethodKind;
  fn: string;
  argsRaw: string; // including parens: "(Class * self, ...)" or "(const char *x)"
  returnType: string; // e.g. "double" or "Class *"
};

type ClassInfo = {
  global: string; // iam_example_ancestry_Parent
  headerPathAbs: string; // /abs/.../src/mod/.../Parent.h
  headerPathRelFromSrc: string; // mod/.../Parent.h (POSIX)
  cPathAbs?: string; // /abs/.../src/mod/.../Parent.c
  parentGlobal?: string; // iam_example_ancestry_GrandParent
  methods: Method[]; // parsed ONLY from this header's method list (not expanded)
  includesRelFromSrc: string[]; // mod/.../*.h includes (POSIX)
};

type ResolvedMethod = {
  method: Method;
  definedIn: ClassInfo; // nearest ancestor that defines it
};

type GenPlan = {
  child: ClassInfo;
  missing: ResolvedMethod[]; // what wrappers to generate
};

function toPosix(p: string) {
  return p.split(path.sep).join("/");
}

function stripBom(s: string) {
  return s.charCodeAt(0) === 0xfeff ? s.slice(1) : s;
}

async function readText(fileAbs: string): Promise<string> {
  const buf = await fs.readFile(fileAbs);
  return stripBom(buf.toString("utf8")).replace(/\r\n/g, "\n");
}

async function writeIfChanged(fileAbs: string, next: string): Promise<boolean> {
  let prev: string | null = null;
  try {
    prev = (await fs.readFile(fileAbs)).toString("utf8").replace(/\r\n/g, "\n");
  } catch {
    prev = null;
  }
  const normNext = next.replace(/\r\n/g, "\n");
  if (prev === normNext) return false;
  await fs.mkdir(path.dirname(fileAbs), { recursive: true });
  await fs.writeFile(fileAbs, normNext, "utf8");
  return true;
}

async function fileExists(fileAbs: string): Promise<boolean> {
  try {
    await fs.stat(fileAbs);
    return true;
  } catch {
    return false;
  }
}

async function removeFileIfExists(fileAbs: string): Promise<boolean> {
  try {
    await fs.unlink(fileAbs);
    return true;
  } catch {
    return false;
  }
}

async function walk(dirAbs: string, out: string[] = []): Promise<string[]> {
  let entries: any[];
  try {
    entries = await fs.readdir(dirAbs, { withFileTypes: true });
  } catch {
    return out;
  }

  for (const ent of entries) {
    const full = path.join(dirAbs, ent.name);

    if (ent.isDirectory()) {
      if (
        ent.name === "node_modules" ||
        ent.name === ".git" ||
        ent.name === "dist" ||
        ent.name === "build" ||
        ent.name === "out" ||
        ent.name === "target"
      )
        continue;
      await walk(full, out);
    } else if (ent.isFile()) {
      out.push(full);
    }
  }
  return out;
}

/** Split by commas at depth 0 of parentheses. */
function splitTopLevelCommas(s: string): string[] {
  const parts: string[] = [];
  let start = 0;
  let depth = 0;
  for (let i = 0; i < s.length; i++) {
    const ch = s[i];
    if (ch === "(") depth++;
    else if (ch === ")") depth = Math.max(0, depth - 1);
    else if (ch === "," && depth === 0) {
      parts.push(s.slice(start, i).trim());
      start = i + 1;
    }
  }
  parts.push(s.slice(start).trim());
  return parts.filter(Boolean);
}

function findMatchingParen(text: string, openIndex: number): number {
  if (text[openIndex] !== "(") return -1;
  let depth = 0;
  for (let i = openIndex; i < text.length; i++) {
    const ch = text[i];
    if (ch === "(") depth++;
    else if (ch === ")") {
      depth--;
      if (depth === 0) return i;
    }
  }
  return -1;
}

function skipWsAndComments(text: string, i: number): number {
  while (i < text.length) {
    const ch = text[i];
    if (ch === " " || ch === "\t" || ch === "\n" || ch === "\r") {
      i++;
      continue;
    }

    if (ch === "/" && text[i + 1] === "/") {
      i += 2;
      while (i < text.length && text[i] !== "\n") i++;
      continue;
    }
    if (ch === "/" && text[i + 1] === "*") {
      i += 2;
      while (i < text.length) {
        if (text[i] === "*" && text[i + 1] === "/") {
          i += 2;
          break;
        }
        i++;
      }
      continue;
    }
    break;
  }
  return i;
}

function argListWithoutOuterParens(argsRaw: string): string {
  const t = argsRaw.trim();
  if (t.startsWith("(") && t.endsWith(")")) return t.slice(1, -1).trim();
  return t;
}

function replaceClassToken(s: string, classType: string): string {
  return s.replace(/\bClass\b/g, classType);
}

function extractParamDecls(argsListNoParens: string): string[] {
  if (!argsListNoParens) return [];
  const parts = splitTopLevelCommas(argsListNoParens);
  return parts.map((p) => p.trim()).filter(Boolean);
}

function extractParamNamesFromDecls(paramDecls: string[]): string[] {
  const names: string[] = [];
  for (const decl of paramDecls) {
    const t = decl.trim();
    if (!t || t === "void") continue;

    const m = /([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)?$/.exec(t);
    names.push(m ? m[1] : "arg");
  }
  return names;
}

function paramIsClassTyped(paramDecl: string, childGlobal: string): boolean {
  // after replacement, any param decl that mentions childGlobal is originally "Class ..."
  return new RegExp(
    `\\b${childGlobal.replace(/[.*+?^${}()|[\]\\]/g, "\\$&")}\\b`
  ).test(paramDecl);
}

function uppercaseGuard(s: string): string {
  return s.replace(/[^A-Za-z0-9]/g, "_").toUpperCase();
}

/** Extract mod/... includes (POSIX paths) */
function extractModIncludes(headerText: string): string[] {
  const out: string[] = [];
  const re = /^\s*#include\s+"(mod\/[^"]+\.h)"\s*$/gm;
  let m: RegExpExecArray | null;
  while ((m = re.exec(headerText))) {
    out.push(m[1]);
  }
  return out;
}

/** Parse this header's METHOD_LIST (not expanded), plus parent METHOD_LIST reference */
function parseMethodListFromHeader(
  headerText: string
): { global: string; parentGlobal?: string; methods: Method[] } | null {
  const defineRe =
    /#define\s+([A-Za-z_]\w*)_METHOD_LIST\s*\(\s*INSTANCE\s*,\s*STATIC\s*,\s*Global\s*,\s*Class\s*\)\s*\\?/m;
  const m = defineRe.exec(headerText);
  if (!m) return null;

  const global = m[1];

  // line-based body grab
  const lines = headerText.split("\n");
  const defLineIdx = lines.findIndex((ln) =>
    ln.includes(`#define ${global}_METHOD_LIST`)
  );
  if (defLineIdx < 0) return null;

  const bodyLines: string[] = [];
  for (let i = defLineIdx + 1; i < lines.length; i++) {
    const ln = lines[i];
    bodyLines.push(ln);
    if (!ln.trimEnd().endsWith("\\")) break;
  }

  const bodyText = bodyLines
    .map((ln) => ln.replace(/\\\s*$/, "").trim())
    .filter((ln) => ln.length > 0)
    .join("\n");

  // parent method list call inside body
  const parentRe =
    /\b([A-Za-z_]\w*)_METHOD_LIST\s*\(\s*INSTANCE\s*,\s*STATIC\s*,\s*Global\s*,\s*Class\s*\)/m;
  const pm = parentRe.exec(bodyText);
  const parentGlobal = pm && pm[1] !== global ? pm[1] : undefined;

  const methods: Method[] = [];
  for (const inv of ["INSTANCE", "STATIC"] as const) {
    let idx = 0;
    while (idx < bodyText.length) {
      const at = bodyText.indexOf(inv + "(", idx);
      if (at < 0) break;
      const open = at + inv.length;
      const close = findMatchingParen(bodyText, open);
      if (close < 0) break;

      const inside = bodyText.slice(open + 1, close);
      const parts = splitTopLevelCommas(inside);

      if (parts.length >= 5) {
        const fn = parts[2].trim();
        const argsRaw = parts[3].trim();
        const returnType = parts.slice(4).join(",").trim();

        methods.push({
          kind: inv === "INSTANCE" ? "instance" : "static",
          fn,
          argsRaw,
          returnType,
        });
      }

      idx = close + 1;
    }
  }

  return { global, parentGlobal, methods };
}

function stripCComments(input: string): string {
  const s = input;
  const out: string[] = [];
  let i = 0;

  type State = "code" | "line" | "block" | "string" | "char";
  let st: State = "code";

  while (i < s.length) {
    const ch = s[i];
    const next = i + 1 < s.length ? s[i + 1] : "";

    if (st === "code") {
      if (ch === "/" && next === "/") {
        // line comment
        out.push(" ", " ");
        i += 2;
        st = "line";
        continue;
      }
      if (ch === "/" && next === "*") {
        // block comment
        out.push(" ", " ");
        i += 2;
        st = "block";
        continue;
      }
      if (ch === '"') {
        out.push(ch);
        i++;
        st = "string";
        continue;
      }
      if (ch === "'") {
        out.push(ch);
        i++;
        st = "char";
        continue;
      }

      out.push(ch);
      i++;
      continue;
    }

    if (st === "line") {
      if (ch === "\n") {
        out.push("\n");
        i++;
        st = "code";
      } else {
        out.push(" ");
        i++;
      }
      continue;
    }

    if (st === "block") {
      if (ch === "*" && next === "/") {
        out.push(" ", " ");
        i += 2;
        st = "code";
      } else if (ch === "\n") {
        out.push("\n");
        i++;
      } else {
        out.push(" ");
        i++;
      }
      continue;
    }

    if (st === "string") {
      if (ch === "\\") {
        // preserve escapes so we don't accidentally terminate early
        out.push(ch);
        if (i + 1 < s.length) out.push(s[i + 1]);
        i += 2;
        continue;
      }
      out.push(ch);
      i++;
      if (ch === '"') st = "code";
      continue;
    }

    // st === "char"
    if (ch === "\\") {
      out.push(ch);
      if (i + 1 < s.length) out.push(s[i + 1]);
      i += 2;
      continue;
    }
    out.push(ch);
    i++;
    if (ch === "'") st = "code";
  }

  return out.join("");
}

/** Find local macro like: #define Parent_(fn) iam_example_ancestry_Parent_##fn */
function findLocalFnMacroName(
  cText: string,
  childGlobal: string
): string | null {
  const escaped = childGlobal.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");

  // Very tolerant match for:
  //   #define Parent_(fn) iam_example_ancestry_Parent_##fn
  //   #define Parent_(x)  iam_example_ancestry_Parent_ ## x
  //   #define Parent_(x)  iam_example_ancestry_Parent ## x
  const re = new RegExp(
    String.raw`^\s*#define\s+([A-Za-z_]\w*)\s*$begin:math:text$\\s\*\[A\-Za\-z\_\]\\w\*\\s\*$end:math:text$\s+${escaped}\s*_?\s*##\s*[A-Za-z_]\w*\b`,
    "m"
  );

  const m = re.exec(cText);
  return m ? m[1] : null;
}

/** Detect method DEFINITIONS only: <Macro>(fn)(...){ ... } or <Global>_fn(...){...} */
function findImplementedMethodsInC(
  cText: string,
  childGlobal: string
): Set<string> {
  const implemented = new Set<string>();

  cText = stripCComments(cText); // ✅ important

  function scanMacroDefs(macroName: string) {
    let i = 0;
    const needle = macroName + "(";

    while (i < cText.length) {
      const at = cText.indexOf(needle, i);
      if (at < 0) break;

      let j = at + needle.length;
      j = skipWsAndComments(cText, j);

      const fnMatch = /^[A-Za-z_]\w*/.exec(cText.slice(j));
      if (!fnMatch) {
        i = at + needle.length;
        continue;
      }

      const fn = fnMatch[0];
      j += fn.length;
      j = skipWsAndComments(cText, j);

      if (cText[j] !== ")") {
        i = at + needle.length;
        continue;
      }
      j++;
      j = skipWsAndComments(cText, j);

      if (cText[j] !== "(") {
        i = at + needle.length;
        continue;
      }
      const argsClose = findMatchingParen(cText, j);
      if (argsClose < 0) {
        i = at + needle.length;
        continue;
      }

      j = argsClose + 1;
      j = skipWsAndComments(cText, j);

      // Only count as implemented if it's a definition (next token is '{')
      if (cText[j] === "{") implemented.add(fn);

      i = argsClose + 1;
    }
  }

  // 1) Preferred: macro name from #define
  let localMacro = findLocalFnMacroName(cText, childGlobal);
  if (localMacro) {
    scanMacroDefs(localMacro);
  } else {
    // 2) Fallback: infer macro name by scanning for definitions like X_(fn)(...) { ... }
    // Find the most frequent macro-name that appears in definition form.
    const counts = new Map<string, number>();

    // We only consider macro names ending with '_' (Parent_, GrandParent_, etc.)
    const re = /\b([A-Za-z_]\w*_) *\(\s*([A-Za-z_]\w*)\s*\)\s*\(/g;

    let m: RegExpExecArray | null;
    while ((m = re.exec(cText))) {
      const macroName = m[1];
      const startAt = m.index;

      // Confirm it's a definition using the same parsing rules:
      // macroName(fn)(args...) {  <-- must hit '{'
      let j = startAt + macroName.length + 1; // after macroName + '('
      j = skipWsAndComments(cText, j);

      const fnMatch = /^[A-Za-z_]\w*/.exec(cText.slice(j));
      if (!fnMatch) continue;

      j += fnMatch[0].length;
      j = skipWsAndComments(cText, j);
      if (cText[j] !== ")") continue;

      j++;
      j = skipWsAndComments(cText, j);
      if (cText[j] !== "(") continue;

      const argsClose = findMatchingParen(cText, j);
      if (argsClose < 0) continue;

      j = argsClose + 1;
      j = skipWsAndComments(cText, j);

      if (cText[j] === "{") {
        counts.set(macroName, (counts.get(macroName) ?? 0) + 1);
      }
    }

    // Pick best candidate and scan it
    let best: string | null = null;
    let bestCount = 0;
    for (const [k, v] of counts.entries()) {
      if (v > bestCount) {
        best = k;
        bestCount = v;
      }
    }

    if (best) {
      localMacro = best;
      scanMacroDefs(localMacro);
    }
  }

  // 3) Optional: direct global defs fallback (if someone writes iam_example_..._fn(...) { ... })
  let i = 0;
  const directNeedle = childGlobal + "_";
  while (i < cText.length) {
    const at = cText.indexOf(directNeedle, i);
    if (at < 0) break;

    const before = at > 0 ? cText[at - 1] : " ";
    if (/[A-Za-z0-9_]/.test(before)) {
      i = at + directNeedle.length;
      continue;
    }

    let j = at + directNeedle.length;
    const fnMatch = /^[A-Za-z_]\w*/.exec(cText.slice(j));
    if (!fnMatch) {
      i = at + directNeedle.length;
      continue;
    }

    const fn = fnMatch[0];
    j += fn.length;
    j = skipWsAndComments(cText, j);

    if (cText[j] !== "(") {
      i = at + directNeedle.length;
      continue;
    }
    const argsClose = findMatchingParen(cText, j);
    if (argsClose < 0) {
      i = at + directNeedle.length;
      continue;
    }

    j = argsClose + 1;
    j = skipWsAndComments(cText, j);

    if (cText[j] === "{") implemented.add(fn);

    i = argsClose + 1;
  }

  return implemented;
}

function methodKey(m: Method): string {
  return `${m.kind}:${m.fn}`;
}

function computeInheritsHeaderPathAbs(childHeaderAbs: string): string {
  const dir = path.dirname(childHeaderAbs);
  const base = path.basename(childHeaderAbs).replace(/\.h$/i, "");
  return path.join(dir, `${base}.inherits.h`);
}

function computeIncludePathsFromSrc(childHeaderRelFromSrc: string): {
  headerInclude: string;
  inheritsInclude: string;
} {
  const headerInclude = toPosix(childHeaderRelFromSrc);
  const inheritsInclude = headerInclude.replace(/\.h$/i, ".inherits.h");
  return { headerInclude, inheritsInclude };
}

function injectOrRemoveInheritsInclude(
  cText: string,
  headerInclude: string,
  inheritsInclude: string,
  shouldHaveInclude: boolean
): { next: string; changed: boolean } {
  const lines = cText.replace(/\r\n/g, "\n").split("\n");
  const includeLine = `#include "${inheritsInclude}"`;
  const hasInclude = lines.some((ln) => ln.trim() === includeLine);

  if (shouldHaveInclude) {
    if (hasInclude)
      return { next: cText.replace(/\r\n/g, "\n"), changed: false };

    let insertAt = -1;

    for (let i = 0; i < lines.length; i++) {
      const ln = lines[i];
      const m = /^\s*#include\s+"([^"]+)"\s*$/.exec(ln);
      if (!m) continue;
      if (toPosix(m[1]) === toPosix(headerInclude)) {
        insertAt = i + 1;
        break;
      }
    }

    if (insertAt === -1) {
      for (let i = 0; i < lines.length; i++) {
        if (/^\s*#include\b/.test(lines[i])) {
          insertAt = i + 1;
          break;
        }
      }
    }

    if (insertAt === -1) insertAt = 0;

    lines.splice(insertAt, 0, includeLine);
    return { next: lines.join("\n"), changed: true };
  } else {
    if (!hasInclude)
      return { next: cText.replace(/\r\n/g, "\n"), changed: false };
    const nextLines = lines.filter((ln) => ln.trim() !== includeLine);
    return { next: nextLines.join("\n"), changed: true };
  }
}

/** Builds initial header set by walking app + selected namespaces */
async function collectRootHeaders(
  rootAbs: string,
  namespaces: string[]
): Promise<string[]> {
  const srcAbs = path.join(rootAbs, "src");
  const scanDirs: string[] = [path.join(srcAbs, "app")];
  for (const ns of namespaces) scanDirs.push(path.join(srcAbs, "mod", ns));

  const out: string[] = [];
  for (const dir of scanDirs) {
    const files = await walk(dir);
    for (const f of files) if (f.endsWith(".h")) out.push(f);
  }
  return out;
}

/** Parse headers and recursively follow #include "mod/.../*.h" only. */
async function buildClassIndexWithDeps(
  rootAbs: string,
  namespaces: string[]
): Promise<Map<string, ClassInfo>> {
  const srcAbs = path.join(rootAbs, "src");
  const srcPrefix = srcAbs + path.sep;

  const queue: string[] = await collectRootHeaders(rootAbs, namespaces);
  const seenHeaders = new Set<string>();
  const index = new Map<string, ClassInfo>();

  while (queue.length) {
    const hAbs = queue.pop()!;
    if (seenHeaders.has(hAbs)) continue;
    seenHeaders.add(hAbs);

    if (!hAbs.startsWith(srcPrefix)) continue;
    const relFromSrc = toPosix(hAbs.slice(srcPrefix.length));

    const text = await readText(hAbs);
    const includes = extractModIncludes(text);

    // enqueue includes (surgical vendor pulls)
    for (const inc of includes) {
      const incAbs = path.join(srcAbs, inc);
      if (await fileExists(incAbs)) {
        if (!seenHeaders.has(incAbs)) queue.push(incAbs);
      }
    }

    const parsed = parseMethodListFromHeader(text);
    if (!parsed) continue;

    const cAbs = hAbs.replace(/\.h$/i, ".c");
    const hasC = await fileExists(cAbs);

    const info: ClassInfo = {
      global: parsed.global,
      parentGlobal: parsed.parentGlobal,
      methods: parsed.methods,
      headerPathAbs: hAbs,
      headerPathRelFromSrc: relFromSrc,
      cPathAbs: hasC ? cAbs : undefined,
      includesRelFromSrc: includes,
    };

    // first one wins
    if (!index.has(info.global)) index.set(info.global, info);
  }

  return index;
}

const fullMethodsMemo = new Map<string, Map<string, Method>>();

function getFullMethods(
  ci: ClassInfo,
  index: Map<string, ClassInfo>
): Map<string, Method> {
  const cached = fullMethodsMemo.get(ci.global);
  if (cached) return cached;

  const out = new Map<string, Method>();

  // add this class's direct methods
  for (const m of ci.methods) {
    out.set(methodKey(m), m);
  }

  // merge parent full methods for anything not overridden locally
  if (ci.parentGlobal) {
    const p = index.get(ci.parentGlobal);
    if (p) {
      const pm = getFullMethods(p, index);
      for (const [k, m] of pm.entries()) {
        if (!out.has(k)) out.set(k, m);
      }
    }
  }

  fullMethodsMemo.set(ci.global, out);
  return out;
}

/** Resolve inheritance chain methods to nearest ancestor that defines each method key. */
function resolveAncestorMethods(
  child: ClassInfo,
  index: Map<string, ClassInfo>
): Map<string, ResolvedMethod> {
  const resolved = new Map<string, ResolvedMethod>();

  let curGlobal = child.parentGlobal;
  while (curGlobal) {
    const cur = index.get(curGlobal);
    if (!cur) break;

    const curFull = getFullMethods(cur, index);

    // nearest ancestor wins
    for (const [k, m] of curFull.entries()) {
      if (!resolved.has(k)) resolved.set(k, { method: m, definedIn: cur });
    }

    curGlobal = cur.parentGlobal;
  }

  return resolved;
}

function shouldGenerateWrapper(child: ClassInfo, r: ResolvedMethod): boolean {
  const m = r.method;

  // skip new always
  if (m.kind === "static" && m.fn === "new") return false;

  // constructor is allowed, but special-cased in emission
  if (m.kind === "static" && m.fn === "constructor") return true;

  // For other methods:
  // If return mentions Class, it's ambiguous (could return a base pointer). Skip for safety.
  // (Instance fluent APIs returning Class* are handled below by returning self.)
  const retHasClass = /\bClass\b/.test(m.returnType);

  if (m.kind === "static") {
    if (retHasClass) return false; // only constructor gets this
    return true;
  }

  // instance:
  // allow; if returnType is Class* we'll emit "call; return self;"
  return true;
}

function generateWrapper(
  child: ClassInfo,
  parent: ClassInfo,
  r: ResolvedMethod
): string {
  const m = r.method;
  const definedIn = r.definedIn;

  const childRet = replaceClassToken(m.returnType.trim(), child.global);
  const childArgsNoParens = replaceClassToken(
    argListWithoutOuterParens(m.argsRaw),
    child.global
  );
  const childParamDecls = extractParamDecls(childArgsNoParens);
  const childParamNames = extractParamNamesFromDecls(childParamDecls);
  const selfName = childParamNames[0] ?? "self";

  // Build call args: cast any Class-typed params to the defining ancestor's type
  const callArgExprs: string[] = [];
  for (let i = 0; i < childParamNames.length; i++) {
    const name = childParamNames[i];
    const decl = childParamDecls[i] ?? "";
    if (paramIsClassTyped(decl, child.global)) {
      callArgExprs.push(`(${definedIn.global} *)${name}`);
    } else {
      callArgExprs.push(name);
    }
  }

  const childFn = `${child.global}_${m.fn}`;
  const parentFn = `${definedIn.global}_${m.fn}`;

  // special: static constructor
  if (m.kind === "static" && m.fn === "constructor") {
    // Signature is (Child *self, ...) -> Child*
    // Body: Parent_constructor((Parent*)self, ...); return self;
    return [
      `${childRet}`,
      `${childFn}(${childArgsNoParens}) {`,
      `  ${parentFn}(${callArgExprs.join(", ")});`,
      `  return ${selfName};`,
      `}`,
      ``,
    ].join("\n");
  }

  // instance fluent case: return is Child* (meaning original return was Class*)
  const retLooksLikeSelfPtr = new RegExp(
    `\\b${child.global.replace(/[.*+?^${}()|[\]\\]/g, "\\$&")}\\s*\\*\\b`
  ).test(childRet);

  // If instance returns self pointer, emit: parentFn(...); return self;
  if (m.kind === "instance" && retLooksLikeSelfPtr) {
    return [
      `${childRet}`,
      `${childFn}(${childArgsNoParens}) {`,
      `  ${parentFn}(${callArgExprs.join(", ")});`,
      `  return ${selfName};`,
      `}`,
      ``,
    ].join("\n");
  }

  // normal call/return
  if (childRet === "void") {
    return [
      `void`,
      `${childFn}(${childArgsNoParens}) {`,
      `  ${parentFn}(${callArgExprs.join(", ")});`,
      `}`,
      ``,
    ].join("\n");
  }

  return [
    `${childRet}`,
    `${childFn}(${childArgsNoParens}) {`,
    `  return ${parentFn}(${callArgExprs.join(", ")});`,
    `}`,
    ``,
  ].join("\n");
}

function generateInheritsHeader(
  plan: GenPlan,
  index: Map<string, ClassInfo>
): string {
  const child = plan.child;
  const guard = uppercaseGuard(`${child.global}_INHERITS_H`);

  const lines: string[] = [];
  lines.push("#pragma once");
  lines.push("");
  lines.push(`#ifndef ${guard}`);
  lines.push(`#define ${guard}`);
  lines.push("");

  for (const r of plan.missing) {
    lines.push(generateWrapper(child, r.definedIn, r).trimEnd());
    lines.push("");
  }

  lines.push(`#endif`);
  lines.push("");

  return lines.join("\n");
}

async function planGeneration(
  index: Map<string, ClassInfo>
): Promise<GenPlan[]> {
  const plans: GenPlan[] = [];

  for (const child of index.values()) {
    if (!child.parentGlobal) continue;
    if (!child.cPathAbs) continue;

    const parent = index.get(child.parentGlobal);
    if (!parent) continue; // parent header not found even via deps

    // What methods exist in ancestors (nearest definition wins)
    const ancestorResolved = resolveAncestorMethods(child, index);

    // What child already implements (definitions only)
    const cText = await readText(child.cPathAbs);
    const implemented = findImplementedMethodsInC(cText, child.global);

    // Determine which methods are missing in child that exist in ancestors
    const missing: ResolvedMethod[] = [];
    for (const r of ancestorResolved.values()) {
      const m = r.method;

      if (implemented.has(m.fn)) continue;
      if (!shouldGenerateWrapper(child, r)) continue;

      missing.push(r);
    }

    // stable ordering
    missing.sort((a, b) => {
      const ka = methodKey(a.method);
      const kb = methodKey(b.method);
      return ka.localeCompare(kb);
    });

    plans.push({ child, missing });
  }

  return plans;
}

async function applyPlans(plans: GenPlan[], index: Map<string, ClassInfo>) {
  let touchedC = 0;
  let wroteHeaders = 0;
  let deletedHeaders = 0;

  for (const plan of plans) {
    const child = plan.child;
    if (!child.cPathAbs) continue;

    const inheritsAbs = computeInheritsHeaderPathAbs(child.headerPathAbs);
    const { headerInclude, inheritsInclude } = computeIncludePathsFromSrc(
      child.headerPathRelFromSrc
    );

    const needHeader = plan.missing.length > 0;

    // header write/delete
    if (needHeader) {
      const header = generateInheritsHeader(plan, index);
      const wrote = await writeIfChanged(inheritsAbs, header);
      if (wrote) wroteHeaders++;
    } else {
      const removed = await removeFileIfExists(inheritsAbs);
      if (removed) deletedHeaders++;
    }

    // include inject/remove
    const cText = await readText(child.cPathAbs);
    const { next, changed } = injectOrRemoveInheritsInclude(
      cText,
      headerInclude,
      inheritsInclude,
      needHeader
    );

    if (changed) {
      await writeIfChanged(child.cPathAbs, next);
      touchedC++;
    }
  }

  return { touchedC, wroteHeaders, deletedHeaders };
}

function printUsage() {
  console.log(
    [
      "iamc v1",
      "",
      "Usage:",
      "  iamc gen -ns <namespaces...>",
      "  iamc gen              (only scans src/app + deps from includes)",
      "",
      "Examples:",
      "  node --experimental-strip-types iamc.ts gen -ns iam mc",
      "  node --experimental-strip-types iamc.ts gen",
      "",
      "Notes:",
      "  - Generates <Class>.inherits.h for missing inherited methods.",
      "  - Wraps INSTANCE + eligible STATIC methods.",
      "  - Skips STATIC new(). Special-cases STATIC constructor().",
    ].join("\n")
  );
}

function parseArgs(argv: string[]) {
  let cmd: string | null = null;
  let i = 0;

  if (argv[0] && !argv[0].startsWith("-")) {
    cmd = argv[0];
    i = 1;
  } else {
    cmd = "gen";
    i = 0;
  }

  const namespaces: string[] = [];
  while (i < argv.length) {
    const a = argv[i];

    if (a === "-h" || a === "--help")
      return { cmd: "help" as const, namespaces: [] };

    if (a === "-ns" || a === "--namespaces") {
      i++;
      while (i < argv.length && !argv[i].startsWith("-")) {
        namespaces.push(argv[i]);
        i++;
      }
      continue;
    }

    i++;
  }

  return { cmd: (cmd ?? "gen") as "gen" | "help", namespaces };
}

async function main() {
  const argv = process.argv.slice(2);
  const { cmd, namespaces } = parseArgs(argv);

  if (cmd !== "gen") {
    printUsage();
    process.exit(0);
  }

  const rootAbs = process.cwd();

  const index = await buildClassIndexWithDeps(rootAbs, namespaces);
  const plans = await planGeneration(index);
  const res = await applyPlans(plans, index);

  console.log(
    [
      `iamc gen: namespaces=[${namespaces.join(", ")}]`,
      `classes indexed: ${index.size}`,
      `child classes processed: ${plans.length}`,
      `updated .c files: ${res.touchedC}`,
      `wrote .inherits.h files: ${res.wroteHeaders}`,
      `deleted .inherits.h files: ${res.deletedHeaders}`,
    ].join("\n")
  );
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
