#include "module/iam/core/Kernel.h"
#include <stdlib.h>

/* === ENFORCE METHODS IMPLEMENTATON === */

IAMC_REQUIRE_IMPLEMENTED(iam_Kernel);

#define Kernel_(fn) iam_Kernel_##fn
typedef iam_Kernel Kernel;
typedef iam_KernelRegistry KernelRegistry;

/* === CONSTRUCTOR === */

Kernel *
iam_Kernel_new(int registry_cap) {
  if (registry_cap == 0) {
    registry_cap = iam_Kernel_get_default_registry_cap();
  }

  Kernel *kernel = malloc(sizeof(Kernel));

  kernel->registry_cap = registry_cap;
  kernel->registry_count = 0;
  kernel->registry = malloc(sizeof(KernelRegistry) * registry_cap);

  return kernel;
}

/* === DEFAULT INSTANCE === */

static Kernel *iam_default = NULL;

/* === INSTANCE METHODS === */

void
Kernel_(register_prototype)(Kernel *kernel, KernelRegistry register_fn) {
  if (kernel->registry_count < kernel->registry_cap) {
    kernel->registry[kernel->registry_count++] = register_fn;
  }
}

void
Kernel_(boot)(Kernel *kernel) {
  for (int i = 0; i < kernel->registry_count; i++) {
    kernel->registry[i]();
  }
}

/* === STATIC METHODS === */

int
iam_Kernel_get_default_registry_cap() {
  return iam_Kernel_DEFAULT_REGISTRY_CAP;
}

Kernel *
iam_Kernel_get_default_instance() {
  if (!iam_default) {
    iam_default = iam_Kernel_new(0);
  }
  return iam_default;
}

void
iam_Kernel_register(iam_KernelRegistry fn) {
  Kernel *kernel = iam_Kernel_get_default_instance();
  iam_Kernel_register_prototype(kernel, fn);
}

void
iam_Kernel_init() {
  Kernel *kernel = iam_Kernel_get_default_instance();
  iam_Kernel_boot(kernel);
}

#undef Kernel_
