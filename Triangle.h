#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "iam.h"
#include "Rectangle.h"

void Triangle_init(void);

__attribute__((constructor)) static void register_Triangle(void)
{
  iam_register(Triangle_init);
}

#define TRIANGLE_ONLY_FIELD_LIST \
  X(sideLength, double)

#define TRIANGLE_FIELD_LIST \
  RECTANGLE_FIELD_LIST      \
  TRIANGLE_ONLY_FIELD_LIST;

#define TRIANGLE_ONLY_METHOD_LIST \
  METHOD(double, sideArea)        \
  METHOD(double, rightAngle);

#define TRIANGLE_METHOD_LIST \
  RECTANGLE_METHOD_LIST      \
  TRIANGLE_ONLY_METHOD_LIST;

typedef struct Triangle Triangle;
typedef struct Triangle_Fn Triangle_Fn;

/* Child struct */
struct Triangle
{
  struct Triangle_Fn *fn; /* own vtable */
#define X(name, type) type name;
  TRIANGLE_FIELD_LIST
#undef X
};

/* Triangle vtable */
struct Triangle_Fn
{
#define METHOD(ret, name) ret (*name)(Triangle *);
  TRIANGLE_METHOD_LIST
#undef METHOD
};

extern Triangle_Fn Triangle_fn;

Triangle *Triangle_new();

#endif