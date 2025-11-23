#ifndef SHAPE_H
#define SHAPE_H

#include "iam.h"

/* X-macro field lists */
#define SHAPE_FIELD_LIST \
  X(w, double)           \
  X(h, double)

/* X-macro method lists */
#define SHAPE_METHOD_LIST            \
  METHOD(double, area)      \
  METHOD(double, perimeter) \
  METHOD(void, describe)

/* Generate struct */
typedef struct Shape
{
  struct Shape_Fn *fn;
  /* fields */
  #define X(name, type) type name;
  SHAPE_FIELD_LIST
  #undef X
} Shape;

/* vtable */
typedef struct Shape_Fn
{
#define METHOD(ret, name) ret (*name)(Shape *);
  SHAPE_METHOD_LIST
#undef METHOD
} Shape_Fn;

extern Shape_Fn Shape_fn;

/* ctor */
Shape *Shape_new();

#endif
