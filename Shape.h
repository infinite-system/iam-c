#ifndef SHAPE_H
#define SHAPE_H

#include "iam.h"

/* X-macro field lists */
#define SHAPE_FIELD_LIST \
    X(w, double)         \
    X(h, double)

    /* X-macro method lists */
#define SHAPE_METHOD_LIST  \
    METHOD(double, area, Shape*)   \
    METHOD(double, perimeter, Shape*) \
    METHOD(void, describe, Shape*)

/* Generate struct */
typedef struct Shape {
    /* fields */
    #define X(name, type) type name;
    SHAPE_FIELD_LIST
    #undef X

    struct Shape_Fn *fn;
} Shape;

/* vtable */
typedef struct Shape_Fn {
    #define METHOD(ret,name,ptype) ret (*name)(ptype);
    SHAPE_METHOD_LIST
    #undef METHOD
} Shape_Fn;

extern Shape_Fn Shape_fn;

/* ctor */
Shape* Shape_new();

#endif
