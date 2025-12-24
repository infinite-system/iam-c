#ifndef iam_Kernel_h
#define iam_Kernel_h

#include "module/iam/core/macro.h"
#include "module/iam/core/string.h"

/* === KERNEL REGISTRY === */

#define iam_Kernel_DEFAULT_REGISTRY_CAP 50000
typedef void (*iam_KernelRegistry)(void);

/* === FIELDS === */

#define iam_Kernel_FIELD_LIST                                                  \
  FIELD(registry, iam_KernelRegistry *)                                        \
  FIELD(registry_count, int)                                                   \
  FIELD(registry_cap, int)

typedef struct iam_Kernel {
#define FIELD(name, type) type name;
  iam_Kernel_FIELD_LIST
#undef FIELD
} iam_Kernel;

/* === METHODS === */

#define iam_Kernel_METHOD_LIST(THIS, STATIC, Global, Class)                    \
  THIS(                                                                        \
    Global,                                                                    \
    Class,                                                                     \
    register_prototype,                                                        \
    void,                                                                      \
    (Class * self, iam_KernelRegistry fn)                                      \
  )                                                                            \
  THIS(Global, Class, boot, void, (Class * self))                              \
  STATIC(Global, Class, get_default_instance, Class *, ())                     \
  STATIC(Global, Class, get_default_registry_cap, int, ())                     \
  STATIC(Global, Class, register, void, (iam_KernelRegistry fn))               \
  STATIC(Global, Class, init, void, ())                                        \
  STATIC(Global, Class, new, Class *, (int registry_cap))

IAMC_DECLARE_METHODS(iam_Kernel)

#endif
