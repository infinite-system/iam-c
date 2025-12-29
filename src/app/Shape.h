#ifndef Shape_H
#define Shape_H

#include "mod/iam/core/iam.h"

void
Shape_prototype(void);

/* X-macro field lists */
#define Shape_FIELD_LIST                                                       \
  X(w, double)                                                                 \
  X(h, double)

/* X-macro method lists */
#define Shape_METHOD_LIST                                                      \
  METHOD(double, area)                                                         \
  METHOD(double, perimeter)                                                    \
  METHOD(void, describe)

/* Generate struct */
typedef struct Shape {
  struct Shape_Fn *fn;
/* fields */
#define X(name, type) type name;
  Shape_FIELD_LIST
#undef X
} Shape;

/* vtable */
typedef struct Shape_Fn {
#define METHOD(ret, name, ...) ret (*name)(Shape *, ##__VA_ARGS__);
  Shape_METHOD_LIST
#undef METHOD
} Shape_Fn;

extern Shape_Fn Shape_fn;

/* ctor */
Shape *
Shape_new();

#endif
