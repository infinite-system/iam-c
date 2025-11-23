#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

#define RECTANGLE_ONLY_FIELD_LIST \
  X(multiplier, double)
#define RECTANGLE_FIELD_LIST \
  SHAPE_FIELD_LIST           \
  RECTANGLE_ONLY_FIELD_LIST

#define RECTANGLE_ONLY_METHOD_LIST        \
  METHOD(double, scaledArea) \
  METHOD(double, diagonal)
#define RECTANGLE_METHOD_LIST \
  SHAPE_METHOD_LIST           \
  RECTANGLE_ONLY_METHOD_LIST

typedef struct Rectangle Rectangle;
typedef struct Rectangle_Fn Rectangle_Fn;

/* Child struct */
typedef struct Rectangle
{
  struct Rectangle_Fn *fn; /* own vtable */
#define X(name, type) type name;
  RECTANGLE_FIELD_LIST
#undef X
} Rectangle;

/* Rectangle vtable */
struct Rectangle_Fn
{
  void *super; /* placeholder for inheritance */
#define METHOD(ret, name) ret (*name)(Rectangle *);
  RECTANGLE_METHOD_LIST
#undef METHOD
};

extern Rectangle_Fn Rectangle_fn;

Rectangle *Rectangle_new();

#endif