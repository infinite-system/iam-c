#ifndef Triangle_H
#define Triangle_H

#include "app/AreaByHeightSide.h"
#include "app/Rectangle.h"
#include "module/iam/core/iam.h"

void
Triangle_prototype(void);

#define Triangle_ONLY_FIELD_LIST X(sideLength, double)

#define Triangle_FIELD_LIST Rectangle_FIELD_LIST Triangle_ONLY_FIELD_LIST;

#define Triangle_ONLY_METHOD_LIST                                              \
  METHOD(double, _calculateComplex)                                            \
  METHOD(double, _calculateAreaIntense)                                        \
  METHOD(double, _calculateAreaAlt)                                            \
  METHOD(double, rightAngle)                                                   \
  METHOD(double, sideArea)                                                     \
  METHOD(double, sideArea2)

#define Triangle_METHOD_LIST Rectangle_METHOD_LIST Triangle_ONLY_METHOD_LIST;

/* Child struct */
typedef struct Triangle {
  struct Triangle_Fn *fn; /* own vtable */
#define X(name, type) type name;
  Triangle_FIELD_LIST
#undef X
} Triangle;

/* Triangle vtable */
typedef struct Triangle_Fn {
#define METHOD(ret, name, ...) ret (*name)(Triangle *, ##__VA_ARGS__);
  Triangle_METHOD_LIST
#undef METHOD
      AreaBySideHeight_Adapter AreaBySideHeight;
} Triangle_Fn;

extern Triangle_Fn Triangle_fn;

Triangle *
Triangle_new();

#endif