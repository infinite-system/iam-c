#ifndef IAM_H
#define IAM_H

#include <stdlib.h>

/* === MEMORY MACRO === */

#define NEW(T) (T *)malloc(sizeof(T))

/* === INHERITANCE MACRO === */

#define INHERIT_METHODS_FROM(parent_fn, child_fn) \
  memcpy(&(child_fn), &(parent_fn), sizeof(parent_fn));

/* === ASYNC MACROS */
#define ASYNC(ctx)           \
  do {                       \
    if (!(ctx)->sync) {      \
      switch ((ctx)->step) { \
      case 0:

#define AWAIT(ctx, expr)      \
  do {                        \
    if ((ctx)->sync) {        \
      expr;                   \
    } else {                  \
      (ctx)->step = __LINE__; \
      expr;                   \
      return;                 \
    case __LINE__:;           \
    }                         \
  } while (0)

#define END_ASYNC(ctx) \
  }                    \
  if (!(ctx)->sync) {  \
    (ctx)->step = 0;   \
  }                    \
  }                    \
  while (0)

/* === FIELD MACROS === */

#define X_FIELD(name, type) type name;
#define X_ARRAY(name, type, size) type name[size];
#define X_FLEX(name, type) type name[];
#define X_BITS(name, type, bits) type name : bits;

/* === CLASS REGISTRY === */

typedef void (*PrototypeRegistry)(void);

/* === FIELDS === */

#define IAM_FIELD_LIST                   \
  X_FIELD(registry, PrototypeRegistry *) \
  X_FIELD(registry_count, size_t)        \
  X_FIELD(registry_cap, size_t)

#define IAM_METHOD_LIST                              \
  METHOD(void, registerPrototype, PrototypeRegistry) \
  METHOD(void, boot)

typedef struct IAM {
  struct IAM_Fn *fn;
#define X_FIELD(name, type) type name;
  IAM_FIELD_LIST
#undef X_FIELD
} IAM;

typedef struct IAM_Fn {
#define METHOD(ret, name, ...) ret (*name)(IAM * self, ##__VA_ARGS__);
  IAM_METHOD_LIST
#undef METHOD
} IAM_Fn;

extern IAM_Fn IAM_fn;

/* === STATIC METHODS === */

#define IAM_STATIC_METHOD_LIST                       \
  METHOD(IAM *, getDefaultInstance)                  \
  METHOD(int, getDefaultRegistryCap)                 \
  METHOD(void, registerPrototype, PrototypeRegistry) \
  METHOD(void, boot)

typedef struct IAM_Static_Fn {
#define METHOD(ret, name, ...) ret (*name)(__VA_ARGS__);
  IAM_STATIC_METHOD_LIST
#undef METHOD
} IAM_Static_Fn;

extern IAM_Static_Fn IAM_static;

/* === CONSTRUCTOR === */

IAM *IAM_new(size_t registry_cap);

/* === GLOBAL WORLD HELPERS === */

void iam_register(PrototypeRegistry fn);
void iam_boot(void);

#endif
