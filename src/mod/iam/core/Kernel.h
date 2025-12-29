// clang-format off
#pragma once

#ifndef iam_Kernel_h
#define iam_Kernel_h

#include "mod/iam/core/macro.h"
#include "string.h"

/* === KERNEL REGISTRY === */

#define iam_Kernel_DEFAULT_REGISTRY_CAP 50000
typedef void (*iam_KernelRegistry)(void);

/* === FIELDS === */

#define iam_Kernel_FIELD_LIST                                                  \
  IAMC_FIELD(registry, iam_KernelRegistry *)                                   \
  IAMC_FIELD(registry_count, int)                                              \
  IAMC_FIELD(registry_cap, int)

typedef struct iam_Kernel {
#define IAMC_FIELD(name, type) type name;
  iam_Kernel_FIELD_LIST
#undef FIELD
} iam_Kernel;

/* === METHODS === */

#define iam_Kernel_METHOD_LIST(INSTANCE, STATIC, Global, Class)                \
  INSTANCE(Global, Class, register_prototype, (Class * self, iam_KernelRegistry fn), void) \
  INSTANCE(Global, Class, boot, (Class * self), void) \
  STATIC(Global, Class, get_default_instance, (), Class *) \
  STATIC(Global, Class, get_default_registry_cap, (), int) \
  STATIC(Global, Class, register, (iam_KernelRegistry fn), void) \
  STATIC(Global, Class, init, (), void) \
  STATIC(Global, Class, constructor, (Class * self, int registry_cap), Class *) \
  STATIC(Global, Class, new, (int registry_cap), Class *)

IAMC_DECLARE_METHODS(iam_Kernel)

#endif
