#include <stdio.h>
#include <math.h>
#include <string.h>

#include "iam.h"
#include "AreaByHeightSide.h"
#include "Rectangle.h"
#include "Triangle.h"

/* ---- Implementations ---- */

static double area(Triangle *r)
{
    Rectangle_fn.parentMethod((Rectangle *)r, 3.14);
    /* 'super' is stored as void* in the fn table; cast it to Rectangle_Fn* to access area */
    printf("[Triangle.area] called\n");
    double base = Rectangle_fn.area((Rectangle *)r);
    printf("[Triangle.area] base=%.2f\n", base);
    return base * r->multiplier * 2 * r->fn->diagonal(r);
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

static double getHeight(Triangle *r)
{
    return r->h;
}

static double getSide(Triangle *r)
{
    return r->sideLength;
}

Triangle_Fn Triangle_fn;

#define TRIANGLE_IMPLEMENTED                \
    /* Core geometry */                     \
    X(area)                                 \
    X(describe)                             \
    X(rightAngle)                           \
    X(sideArea)                             \
    X(sideArea2)                            \
    X(scaledArea)                           \
    X(diagonal)                             \
    /* AreaBySideHeight Adapter contract */ \
    X(getHeight)                            \
    X(getSide)

/* class initializer */
void Triangle_init(void)
{
    INHERIT_METHODS(Triangle_fn, Rectangle_fn); // auto copy

#define X(name) Triangle_fn.name = name;
    TRIANGLE_IMPLEMENTED
#undef X

#define METHOD(returnType, name, ...) \
    Triangle_fn.AreaBySideHeight.name = (void *)name;
    AREA_BY_SIDE_HEIGHT_ADAPTER_METHOD_LIST
#undef METHOD
}

/* ctor */
Triangle *Triangle_new()
{
    Triangle *r = NEW(Triangle);
    r->fn = &Triangle_fn;

    return r;
}