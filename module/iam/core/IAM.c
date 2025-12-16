#include "IAM.h"

#define IAM_DEFAULT_REGISTRY_CAP 50000

/* === DEFAULT INSTANCE === */

static IAM *iam_default = NULL;

/* === INSTANCE METHODS === */

static void registerPrototype(IAM *self, PrototypeRegistry registerFn) {
  if (self->registry_count < self->registry_cap) {
    self->registry[self->registry_count++] = registerFn;
  }
}

static void boot(IAM *self) {
  for (size_t i = 0; i < self->registry_count; i++) {
    self->registry[i]();
  }
}

/* === CLASS PROTOTYPE === */

#define IAM_IMPLEMENTED_METHODS \
  X(registerPrototype)          \
  X(boot)

IAM_Fn IAM_fn = {
  .registerPrototype = registerPrototype,
  .boot = boot
};

/* === CONSTRUCTOR === */

IAM *IAM_new(size_t registry_cap) {
  if (registry_cap == 0) {
    registry_cap = IAM_static.getDefaultRegistryCap();
  }

  IAM *iam = malloc(sizeof(IAM));
  iam->fn = &IAM_fn;

  iam->registry_cap = registry_cap;
  iam->registry_count = 0;
  iam->registry = malloc(sizeof(PrototypeRegistry) * registry_cap);

  return iam;
}

/* === STATIC METHODS === */

static inline int static_defaultRegistryCap() {
  return IAM_DEFAULT_REGISTRY_CAP;
}

static IAM *static_getDefaultInstance() {
  if (!iam_default) {
    iam_default = IAM_new(0);
  }
  return iam_default;
}

static void static_registerPrototype(PrototypeRegistry fn) {
  IAM *iam = IAM_static.getDefaultInstance();
  iam->fn->registerPrototype(iam, fn);
}

static void static_boot(void) {
  IAM *iam = IAM_static.getDefaultInstance();
  iam->fn->boot(iam);
}

/* === STATIC PROTOTYPE === */

IAM_Static_Fn IAM_static = {
  .getDefaultRegistryCap = static_defaultRegistryCap,
  .getDefaultInstance = static_getDefaultInstance,
  .registerPrototype = static_registerPrototype,
  .boot = static_boot
};

/* === GLOBAL WORLD HELPERS === */

void iam_register(PrototypeRegistry fn) {
  IAM_static.registerPrototype(fn);
}

void iam_boot(void) {
  IAM_static.boot();
}