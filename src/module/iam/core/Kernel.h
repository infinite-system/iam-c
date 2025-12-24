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

#define iam_Kernel_METHOD_LIST(INST, STAT, Global, Class)                      \
  INST(                                                                        \
    Global,                                                                    \
    Class,                                                                     \
    register_prototype,                                                        \
    void,                                                                      \
    (Class * self, iam_KernelRegistry fn))                                     \
  INST(Global, Class, boot, void, (Class * self))                              \
  STAT(Global, Class, get_default_instance, Class *, ())                       \
  STAT(Global, Class, get_default_registry_cap, int, ())                       \
  STAT(Global, Class, register, void, (iam_KernelRegistry fn))                 \
  STAT(Global, Class, init, void, ())                                          \
  STAT(Global, Class, new, Class *, (int registry_cap))

IAMC_DECLARE_METHODS(iam_Kernel)

#endif
