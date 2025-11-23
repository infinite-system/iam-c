#include <stdio.h>
#include <math.h>
#include "Triangle.h"
#include "Rectangle.h"
#include <string.h>
#include "iam.h"
/* ---- Implementations ---- */

static double area(Triangle *r)
{
    /* 'super' is stored as void* in the fn table; cast it to Rectangle_Fn* to access area */
    printf("[Triangle.area] called\n");
    double base = SUPER(Rectangle, r, area);
    printf("[Triangle.area] base=%.2f\n", base);
    return base * r->multiplier * 2;
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
    return 0.5 * r->sideLength * r->h;
}

static double rightAngle(Triangle *r)
{
    return 90.0;
}

static void describe(Triangle *r)
{
    printf("[Triangle] area=%.2f scaled=%.2f diag=%.2f side=%.2f angle=%.2f\n",
           area(r),
           scaledArea(r),
           diagonal(r),
           sideArea(r),
           rightAngle(r));
}

Triangle_Fn Triangle_fn;

/* class initializer */
static void Triangle_init(void)
{
    INHERIT_METHODS(Triangle_fn, Rectangle_fn); // auto copy

    Triangle_fn.super = &Rectangle_fn;
    /* overrides */
    Triangle_fn.area = area;         // override
    Triangle_fn.describe = describe; // override
    Triangle_fn.scaledArea = scaledArea;
    Triangle_fn.diagonal = diagonal;

    /* child only */
    Triangle_fn.sideArea = sideArea;
    Triangle_fn.rightAngle = rightAngle;
}

/* auto-run before main */
__attribute__((constructor)) static void Triangle_auto(void)
{
    Triangle_init();
}

/* ctor */
Triangle *Triangle_new()
{
    Triangle *r = NEW(Triangle);
    r->fn = &Triangle_fn;

    return r;
}