#ifndef iam_Kernel_h
#define iam_Kernel_h

#include "module/iam/core/macro.h"
#include "module/iam/core/string.h"

/* === KERNEL REGISTRY === */

#define iam_Kernel_DEFAULT_REGISTRY_CAP 50000
typedef void (*iam_KernelRegistry)(void);

/* === FIELDS === */

#define iam_Kernel_FIELD_LIST                                                                                \
  FIELD(registry, iam_KernelRegistry *)                                                                      \
  FIELD(registry_count, int)                                                                                 \
  FIELD(registry_cap, int)

typedef struct iam_Kernel {
#define FIELD(name, type) type name;
  iam_Kernel_FIELD_LIST
#undef FIELD
} iam_Kernel;

/* === METHODS === */

// clang-format off
#define iam_Kernel_METHOD_LIST(INSTANCE, STATIC, Global, Class)                                              \
  INSTANCE(Global, Class, void, register_prototype, (Global * self, iam_KernelRegistry fn))                  \
  INSTANCE(Global, Class, void, boot, (Global * self))                                                       \
  STATIC(Global, Class, iam_Kernel *, get_default_instance, ())                                              \
  STATIC(Global, Class, int, get_default_registry_cap, ())                                                   \
  STATIC(Global, Class, void, register, (iam_KernelRegistry fn))                                             \
  STATIC(Global, Class, void, init, ())                                                                      \
  STATIC(Global, Class, iam_Kernel *, new, (int registry_cap))
// clang-format on

IAMC_DECLARE_METHODS(iam_Kernel)

#endif
