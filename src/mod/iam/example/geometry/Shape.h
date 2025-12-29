#ifndef IAMC_Ex_Geometry2D_Shape_H
#define IAMC_Ex_Geometry2D_Shape_H

#include "mod/iam/core/iam.h"

/* === FIELDS === */

#define IAMC_Ex_Geometry2D_Shape_FIELD_LIST X(type, const char *)

/* === ABSTRACT CLASS === */

typedef struct IAMC_Ex_Geometry2D_Shape {
  struct IAMC_Ex_Geometry2D_Shape_Fn *fn;
#define X(name, type) type name;
  IAMC_Ex_Geometry2D_Shape_FIELD_LIST
#undef X
} IAMC_Ex_Geometry2D_Shape;

/* === EXTRA STRUCTS === */

typedef struct {
  double x;
  double y;
} IAMC_Position2D;

/* === METHODS === */

#define IAMC_Ex_Geometry2D_Shape_METHOD_LIST                                   \
  METHOD(IAMC_Position2D, getPosition)                                         \
  METHOD(void, setPosition, double x, double y)                                \
  METHOD(void, translate, double dx, double dy)                                \
  METHOD(double, area)                                                         \
  METHOD(double, perimeter)                                                    \
  METHOD(double, resize, double factor)

/* === METHODS VTABLE === */

typedef struct IAMC_Ex_Geometry2D_Shape_Fn {
#define METHOD(ret, name, ...)                                                 \
  ret (*name)(IAMC_Ex_Geometry2D_Shape *, ##__VA_ARGS__);
  IAMC_Ex_Geometry2D_Shape_METHOD_LIST
#undef METHOD
} IAMC_Ex_Geometry2D_Shape_Fn;

extern IAMC_Ex_Geometry2D_Shape_Fn IAMC_Ex_Geometry2D_Shape_fn;

#endif
