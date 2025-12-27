#include "module/iam/core/Kernel.h"
#include <stdlib.h>

/* === ENFORCE METHODS IMPLEMENTATON === */

IAMC_REQUIRE_IMPLEMENTED(iam_Kernel);

#define Kernel_(fn) iam_Kernel_##fn
typedef iam_Kernel Kernel;
typedef iam_KernelRegistry KernelRegistry;

/* === CONSTRUCTOR === */

Kernel *
Kernel_(new)(int registry_cap) {
  Kernel *kernel = malloc(sizeof(Kernel));
  return Kernel_(constructor)(kernel, registry_cap);
}

inline Kernel *
Kernel_(constructor)(Kernel *kernel, int registry_cap) {
  if (registry_cap <= 0) {
    registry_cap = Kernel_(DEFAULT_REGISTRY_CAP);
  }
  kernel->registry = malloc(sizeof(KernelRegistry) * registry_cap);
  kernel->registry_count = 0;
  kernel->registry_cap = registry_cap;
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
Kernel_(get_default_registry_cap)() {
  return Kernel_(DEFAULT_REGISTRY_CAP);
}

Kernel *
Kernel_(get_default_instance)() {
  if (!iam_default) {
    iam_default = Kernel_(new)(0);
  }
  return iam_default;
}

void
Kernel_(register)(iam_KernelRegistry fn) {
  Kernel *kernel = Kernel_(get_default_instance)();
  Kernel_(register_prototype)(kernel, fn);
}

void
Kernel_(init)() {
  Kernel *kernel = Kernel_(get_default_instance)();
  Kernel_(boot)(kernel);
}

#undef Kernel_
