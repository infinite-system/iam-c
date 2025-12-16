#ifndef IAM_Ex_Geometry2D_Shape_H
#define IAM_Ex_Geometry2D_Shape_H

#include "module/iam/core/IAM.h"

/* === FIELDS === */

#define IAM_Ex_Geometry2D_Shape_FIELD_LIST \
  X(type, const char *)

/* === ABSTRACT CLASS === */

typedef struct IAM_Ex_Geometry2D_Shape {
  struct IAM_Ex_Geometry2D_Shape_Fn *fn;
#define X(name, type) type name;
  IAM_Ex_Geometry2D_Shape_FIELD_LIST
#undef X
} IAM_Ex_Geometry2D_Shape;

/* === EXTRA STRUCTS === */

typedef struct {
  double x;
  double y;
} IAM_Position2D;

/* === METHODS === */

#define IAM_Ex_Geometry2D_Shape_METHOD_LIST     \
  METHOD(IAM_Position2D, getPosition)           \
  METHOD(void, setPosition, double x, double y) \
  METHOD(void, translate, double dx, double dy) \
  METHOD(double, area)                          \
  METHOD(double, perimeter)                     \
  METHOD(double, resize, double factor)

/* === METHODS VTABLE === */

typedef struct IAM_Ex_Geometry2D_Shape_Fn {
#define METHOD(ret, name, ...) ret (*name)(IAM_Ex_Geometry2D_Shape *, ##__VA_ARGS__);
  IAM_Ex_Geometry2D_Shape_METHOD_LIST
#undef METHOD
} IAM_Ex_Geometry2D_Shape_Fn;

extern IAM_Ex_Geometry2D_Shape_Fn IAM_Ex_Geometry2D_Shape_fn;

#endif
