#ifndef GrandParent_H
#define GrandParent_H

#include "../../core/IAM.h"

void GrandParent_prototype(void);

/* X-macro field lists */
#define GrandParent_FIELD_LIST \
  X(name, IAM_Str64)           \
  X(age, int)                  \
  X(height, int)               \
  X(weight, double)            \
  X(gender, char)

/* X-macro method lists */
#define GrandParent_METHOD_LIST                                            \
  METHOD(GrandParent *, constructor, const char *, int, int, double, char) \
  METHOD(void, eat, double)                                                \
  METHOD(void, diet, double)                                               \
  METHOD(void, describe)

/* Generate struct */
typedef struct GrandParent {
  struct GrandParent_Fn *fn;
/* fields */
#define X(name, type) type name;
  GrandParent_FIELD_LIST
#undef X
} GrandParent;

/* vtable */
typedef struct GrandParent_Fn {
#define METHOD(ret, name, ...) ret (*name)(void *, ##__VA_ARGS__);
  GrandParent_METHOD_LIST
#undef METHOD
} GrandParent_Fn;

extern GrandParent_Fn GrandParent_fn;

/* ctor */
GrandParent *GrandParent_new(const char *name, int age, int height, double weight, char gender);

#endif
