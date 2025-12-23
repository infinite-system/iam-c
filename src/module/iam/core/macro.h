
/* === MEMORY MACRO === */

#define NEW(T) (T *)malloc(sizeof(T))

/* === INHERITANCE MACRO === */

#define INHERIT_METHODS_FROM(parent_fn, child_fn)                                                  \
  memcpy(&(child_fn), &(parent_fn), sizeof(parent_fn));

/* === ASYNC MACROS */
#define ASYNC(ctx)                                                                                 \
  do {                                                                                             \
    if (!(ctx)->sync) {                                                                            \
      switch ((ctx)->step) {                                                                       \
      case 0:

#define AWAIT(ctx, expr)                                                                           \
  do {                                                                                             \
    if ((ctx)->sync) {                                                                             \
      expr;                                                                                        \
    } else {                                                                                       \
      (ctx)->step = __LINE__;                                                                      \
      expr;                                                                                        \
      return;                                                                                      \
    case __LINE__:;                                                                                \
    }                                                                                              \
  } while (0)

#define END_ASYNC(ctx)                                                                             \
  }                                                                                                \
  if (!(ctx)->sync) {                                                                              \
    (ctx)->step = 0;                                                                               \
  }                                                                                                \
  }                                                                                                \
  while (0)

/* === FIELD MACROS === */

#define FIELD(name, type) type name;
#define FIELD_ARRAY(name, type, size) type name[size];
#define FIELD_FLEX(name, type) type name[];
#define FIELD_BITS(name, type, bits) type name : bits;

/* === METHOD DECLARATION MACROS === */

#define IAMC__DECLARE_INSTANCE(global_Name, Class, ret, name, args) ret global_Name##_##name args;

// Static methods: ret Global_name(...)
#define IAMC__DECLARE_STATIC(global_Name, Class, ret, name, args) ret global_Name##_##name args;

// Generate prototypes for all methods declared in Global_METHOD_LIST.
// Global is the full global type name, e.g. iam_geometry_Shape.
#define IAMC_DECLARE_METHODS(global_Name)                                                          \
  global_Name##_METHOD_LIST(IAMC__DECLARE_INSTANCE, IAMC__DECLARE_STATIC, global_Name, global_Name)

// Internal helpers: build an "anchor" array of function pointers.
// If any method is missing at link time, you get an undefined symbol error.

#define IAMC__REQUIRE_ANCHOR_INSTANCE(Global, ClassType, ret, name, args)                          \
  (const void *)&Global##_##name,

#define IAMC__REQUIRE_ANCHOR_STATIC(Global, ClassType, ret, name, args)                            \
  (const void *)&Global##_##name,

// Strong enforcement: always active.
// Usage in ONE .c file per class:
//   #include "module/iam/.../Rectangle.h"
//   #include "module/iam/core/iamc_require.h"
//   IAMC_REQUIRE_IMPLEMENTED(iam_geometry_Rectangle);
#define IAMC_REQUIRE_IMPLEMENTED_DEBUG(GlobalType)                                                 \
  static const void *const iamc_require_##GlobalType##_anchors[]                                   \
      __attribute__((used)) = { GlobalType##_METHOD_LIST(                                          \
          IAMC__REQUIRE_ANCHOR_INSTANCE, IAMC__REQUIRE_ANCHOR_STATIC, GlobalType, GlobalType) };

// Debug-only enforcement:
// Build with -DIAMC_DEBUG to turn it on; otherwise it’s a no-op.
#ifdef IAMC_DEBUG
#define IAMC_REQUIRE_IMPLEMENTED(GlobalType) IAMC_REQUIRE_IMPLEMENTED_DEBUG(GlobalType)
#else
#define IAMC_REQUIRE_IMPLEMENTED(GlobalType) /* debug enforcement disabled */
#endif

// Local wrappers for instance methods:
//  Kernel_init(Kernel* self, ...) → iam_Kernel_init(iam_Kernel* self, ...)
#define IAMC__LOCAL_INSTANCE(Global, Class, ret, name, args)                                       \
  static ret(*Class##_##name) args = Global##_##name;

// Local static alias: Kernel_init(...) → (*Kernel_init)(...)
#define IAMC__LOCAL_STATIC(Global, Class, ret, name, args)                                         \
  static ret(*Class##_##name) args = Global##_##name;

// User-facing:
//   IAMC_USE_CLASS(iam_Kernel, Kernel);
// gives:
//   typedef iam_Kernel Kernel;
//   static Kernel_register_prototype = iam_Kernel_register_prototype;
//   static Kernel_boot               = iam_Kernel_boot;
//   static Kernel_init               = iam_Kernel_init;
//   ...
#define IAMC_USE_CLASS(GlobalType, LocalName)                                                      \
  typedef GlobalType LocalName;                                                                    \
  GlobalType##_METHOD_LIST(IAMC__LOCAL_INSTANCE, IAMC__LOCAL_STATIC, GlobalType, LocalName)