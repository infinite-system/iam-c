#ifndef Rectangle_H
#define Rectangle_H

#include "app/Shape.h"
#include "mod/iam/core/iam.h"

void
Rectangle_prototype(void);

#define Rectangle_ONLY_FIELD_LIST X(multiplier, double)

#define Rectangle_FIELD_LIST Shape_FIELD_LIST Rectangle_ONLY_FIELD_LIST

#define Rectangle_ONLY_METHOD_LIST                                             \
  METHOD(double, _privateMethod)                                               \
  METHOD(double, scaledArea)                                                   \
  METHOD(double, diagonal)                                                     \
  METHOD(void, parentMethod, double)

#define Rectangle_METHOD_LIST Shape_METHOD_LIST Rectangle_ONLY_METHOD_LIST

/* Child struct */
typedef struct Rectangle {
  struct Rectangle_Fn *fn; /* own vtable */
#define X(name, type) type name;
  Rectangle_FIELD_LIST
#undef X
} Rectangle;

/* Rectangle vtable */
typedef struct Rectangle_Fn {
#define METHOD(ret, name, ...) ret (*name)(Rectangle *, ##__VA_ARGS__);
  Rectangle_METHOD_LIST
#undef METHOD
} Rectangle_Fn;

extern Rectangle_Fn Rectangle_fn;

Rectangle *
Rectangle_new();

#endif