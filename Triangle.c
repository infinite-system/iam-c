#include <stdio.h>
#include <math.h>
#include <string.h>

#include "iam.h"
#include "AreaByHeightSide.h"
#include "Rectangle.h"
#include "Triangle.h"

/* === PRIVATE METHODS === */

static double _calculateComplex(Triangle *r)
{
    return r->w * r->multiplier + r->fn->diagonal(r) * r->w;
}

static double _calculateAreaIntense(Triangle *r)
{
    return r->h * r->multiplier + r->fn->diagonal(r) * 5;
}

static double _calculateAreaAlt(Triangle *r)
{
    return r->h * r->multiplier + r->fn->diagonal(r);
}

/* === PUBLIC METHODS === */

static double area(Triangle *r)
{
    Rectangle_fn.parentMethod((Rectangle *)r, 3.14);
    /* 'super' is stored as void* in the fn table; cast it to Rectangle_Fn* to access area */
    printf("[Triangle.area] called\n");
    double base = Rectangle_fn.area((Rectangle *)r);
    printf("[Triangle.area] base=%.2f\n", base);
    return base * r->multiplier * 2 * r->fn->diagonal(r) * r->fn->_calculateComplex(r);
}

static double scaledArea(Triangle *r)
{
    return r->w * r->h * r->multiplier;
}

static double diagonal(Triangle *r)
{
    return sqrt(r->w * r->w + r->h * r->h);
}

static double sideArea(Triangle *r)
{
    return AreaBySideHeight_fn.sideArea(r, &r->fn->AreaBySideHeight);
}

static double sideArea2(Triangle *r)
{
    return AreaBySideHeight_fn.sideArea2(r, &r->fn->AreaBySideHeight);
}

static double rightAngle(Triangle *r)
{
    return 90.0;
}

static void describe(Triangle *r)
{
    printf("[Triangle] area=%.2f scaled=%.2f diag=%.2f side=%.2f side2=%.2f angle=%.2f\n",
           r->fn->area(r),
           r->fn->scaledArea(r),
           r->fn->diagonal(r),
           r->fn->sideArea(r),
           r->fn->sideArea2(r),
           r->fn->rightAngle(r));
}

/* AreaBySideHeight Adapter methods */

static double getHeight(void *r)
{
    return ((Triangle *)r)->h;
}

static double getSide(void *r)
{
    return ((Triangle *)r)->sideLength;
}

Triangle_Fn Triangle_fn;

#define Triangle_PRIVATE_METHODS \
    X(_calculateComplex)         \
    X(_calculateAreaIntense)     \
    X(_calculateAreaAlt)

#define Triangle_PUBLIC_METHODS \
    /* Core geometry */         \
    X(area)                     \
    X(describe)                 \
    X(rightAngle)               \
    X(sideArea)                 \
    X(sideArea2)                \
    X(scaledArea)               \
    X(diagonal)

#define Triangle_IMPLEMENTED_METHODS \
    Triangle_PRIVATE_METHODS         \
    Triangle_PUBLIC_METHODS

#define Triangle_AREA_BY_SIDE_HEIGHT_ADAPTER_METHODS \
    X(getHeight)                                     \
    X(getSide)

/* class prototype */
void Triangle_prototype(void)
{
    INHERIT_METHODS_FROM(Rectangle_fn, Triangle_fn); // auto copy

#define X(name) Triangle_fn.name = name;
    Triangle_IMPLEMENTED_METHODS
#undef X

    /* Setup AreaBySideHeight adapter */
#define X(name) Triangle_fn.AreaBySideHeight.name = name;
    Triangle_AREA_BY_SIDE_HEIGHT_ADAPTER_METHODS
#undef X
}

/* ctor */
Triangle *Triangle_new()
{
    Triangle *r = NEW(Triangle);
    r->fn = &Triangle_fn;

    return r;
}