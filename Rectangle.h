#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

#define RECTANGLE_ONLY_FIELD_LIST \
    X(multiplier, double)

#define RECTANGLE_FIELD_LIST \
    SHAPE_FIELD_LIST         \
    RECTANGLE_ONLY_FIELD_LIST;

#define RECTANGLE_ONLY_METHOD_LIST          \
    METHOD(double, scaledArea, Rectangle *) \
    METHOD(double, diagonal, Rectangle *);

#define RECTANGLE_METHOD_LIST \
    SHAPE_METHOD_LIST         \
    RECTANGLE_ONLY_METHOD_LIST;

typedef struct Rectangle Rectangle;
typedef struct Rectangle_Fn Rectangle_Fn;

/* Child struct */
struct Rectangle
{
#define X(name, type) type name;
    RECTANGLE_FIELD_LIST
#undef X


    Shape_Fn *superFn; /* base vtable */
    Rectangle_Fn *fn;  /* own vtable */
};

/* Rectangle vtable */
struct Rectangle_Fn
{
#define METHOD(ret, name, ptype) ret (*name)(Rectangle *);
    RECTANGLE_METHOD_LIST
#undef METHOD
};

extern Rectangle_Fn Rectangle_fn;

Rectangle *Rectangle_new();

#endif