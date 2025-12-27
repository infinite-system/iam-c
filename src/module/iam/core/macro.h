
/* === MEMORY MACRO === */

#define NEW(T) (T *)malloc(sizeof(T))

/* === INHERITANCE MACRO === */

#define INHERIT_METHODS_FROM(parent_fn, child_fn)                              \
  memcpy(&(child_fn), &(parent_fn), sizeof(parent_fn));

/* === ASYNC MACROS */
#define ASYNC(ctx)                                                             \
  do {                                                                         \
    if (!(ctx)->sync) {                                                        \
      switch ((ctx)->step) {                                                   \
      case 0:

#define AWAIT(ctx, expr)                                                       \
  do {                                                                         \
    if ((ctx)->sync) {                                                         \
      expr;                                                                    \
    } else {                                                                   \
      (ctx)->step = __LINE__;                                                  \
      expr;                                                                    \
      return;                                                                  \
    case __LINE__:;                                                            \
    }                                                                          \
  } while (0)

#define END_ASYNC(ctx)                                                         \
  }                                                                            \
  if (!(ctx)->sync) {                                                          \
    (ctx)->step = 0;                                                           \
  }                                                                            \
  }                                                                            \
  while (0)

/* === FIELD MACROS === */

#define FIELD(name, type) type name;
#define FIELD_ARRAY(name, type, size) type name[size];
#define FIELD_FLEX(name, type) type name[];
#define FIELD_BITS(name, type, bits) type name : bits;

/* === METHOD DECLARATION MACROS === */

#define __IAMC_DECLARE_INSTANCE(Global, Class, fn, args, returnType)           \
  returnType Global##_##fn args;

// Static methods: returnType Global_fn(...)
#define __IAMC_DECLARE_STATIC(Global, Class, fn, args, returnType)             \
  returnType Global##_##fn args;

// Generate prototypes for all methods declared in Global_METHOD_LIST.
// Global is the full global type fn, e.g. iam_geometry_Shape.
#define IAMC_DECLARE_METHODS(Global)                                           \
  Global##_METHOD_LIST(                                                        \
    __IAMC_DECLARE_INSTANCE, __IAMC_DECLARE_STATIC, Global, Global             \
  )

// Internal helpers: build an "anchor" array of function pointers.
// If any method is missing at link time, you get an undefined symbol error.

#define __IAMC_REQUIRE_ANCHOR_INSTANCE(Global, Class, fn, args, returnType)    \
  (const void *)&Global##_##fn,

#define __IAMC_REQUIRE_ANCHOR_STATIC(Global, Class, fn, args, returnType)      \
  (const void *)&Global##_##fn,

// Strong enforcement: always active.
// Usage in ONE .c file per class:
//   #include "module/iam/.../Rectangle.h"
//   #include "module/iam/core/iamc_require.h"
//   IAMC_REQUIRE_IMPLEMENTED(iam_geometry_Rectangle);
#define __IAMC_REQUIRE_IMPLEMENTED_DEBUG(Global)                               \
  static const void *const iamc_require_##Global##_anchors[]                   \
    __attribute__((used)) = { Global##_METHOD_LIST(                            \
      __IAMC_REQUIRE_ANCHOR_INSTANCE,                                          \
      __IAMC_REQUIRE_ANCHOR_STATIC,                                            \
      Global,                                                                  \
      Global                                                                   \
    ) };

// Debug-only enforcement:
// Build with -DIAMC_DEBUG to turn it on; otherwise it’s a no-op.
#ifdef IAMC_DEBUG
#define IAMC_REQUIRE_IMPLEMENTED(Global)                                       \
  __IAMC_REQUIRE_IMPLEMENTED_DEBUG(Global)
#else
#define IAMC_REQUIRE_IMPLEMENTED(Global) /* debug enforcement disabled */
#endif

// Local wrappers for instance methods:
//  Kernel_init(Kernel* self, ...) → iam_Kernel_init(iam_Kernel* self, ...)
#define __IAMC_LOCAL_INSTANCE(Global, Class, fn, args, returnType)             \
  static returnType(*Class##_##fn) args = Global##_##fn;

// Local static alias: Kernel_init(...) → (*Kernel_init)(...)
#define __IAMC_LOCAL_STATIC(Global, Class, fn, args, returnType)               \
  static returnType(*Class##_##fn) args = Global##_##fn;

// User-facing:
//   IAMC_USE_CLASS(iam_Kernel, Kernel);
// gives:
//   typedef iam_Kernel Kernel;
//   static Kernel_register_prototype = iam_Kernel_register_prototype;
//   static Kernel_boot               = iam_Kernel_boot;
//   static Kernel_init               = iam_Kernel_init;
//   ...
#define IAMC_USE_CLASS(Global, LocalName)                                      \
  typedef Global LocalName;                                                    \
  Global##_METHOD_LIST(                                                        \
    __IAMC_LOCAL_INSTANCE, __IAMC_LOCAL_STATIC, Global, LocalName              \
  )