#include <stdio.h>
#include <math.h>
#include "Triangle.h"
#include <string.h>
#include "iam.h"
/* ---- Implementations ---- */

static double Triangle_area(Triangle *r)
{
    double base = r->superFn->area((Rectangle*)r);
    printf("[Triangle.area] base=%.2f\n", base);
    return base * r->multiplier * 10;
}

static double Triangle_scaledArea(Triangle *r)
{
    return r->w * r->h * r->multiplier;
}

static double Triangle_diagonal(Triangle *r)
{
    return sqrt(r->w * r->w + r->h * r->h);
}

static double Triangle_sideArea(Triangle *r)
{
    return 0.5 * r->sideLength * r->h;
}

static double Triangle_rightAngle(Triangle *r)
{
    return 90.0;
}

static void Triangle_describe(Triangle *r)
{
    printf("[Triangle] area=%.2f scaled=%.2f diag=%.2f side=%.2f angle=%.2f\n",
           Triangle_area(r),
           Triangle_scaledArea(r),
           Triangle_diagonal(r),
           Triangle_sideArea(r),
           Triangle_rightAngle(r));
}

Triangle_Fn Triangle_fn;

/* class initializer */
static void Triangle_init(void)
{
    INHERIT_METHODS(Triangle_fn, Rectangle_fn); // auto copy

    /* overrides */
    Triangle_fn.area = Triangle_area;         // override
    Triangle_fn.describe = Triangle_describe; // override
    Triangle_fn.scaledArea = Triangle_scaledArea;
    Triangle_fn.diagonal = Triangle_diagonal;

    /* child only */
    Triangle_fn.sideArea = Triangle_sideArea;
    Triangle_fn.rightAngle = Triangle_rightAngle;
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
    r->superFn = &Rectangle_fn;
    r->fn = &Triangle_fn;

    return r;
}