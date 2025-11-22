#include <stdio.h>
#include <math.h>
#include "Shape.h"
#include "Rectangle.h"
#include <string.h>
#include "iam.h"
/* ---- Implementations ---- */

static double Rectangle_area(Rectangle *r)
{
    double base = r->superFn->area((Shape*)r);
    return base * r->multiplier;
}

static double Rectangle_scaledArea(Rectangle *r)
{
    return r->w * r->h * r->multiplier;
}

static double Rectangle_diagonal(Rectangle *r)
{
    return sqrt(r->w * r->w + r->h * r->h);
}

static void Rectangle_describe(Rectangle *r)
{
    printf("[Rectangle] scaled=%.2f diag=%.2f\n",
           Rectangle_scaledArea(r),
           Rectangle_diagonal(r));
}

Rectangle_Fn Rectangle_fn;

/* class initializer */
static void Rectangle_init(void)
{
    INHERIT_METHODS(Rectangle_fn, Shape_fn); // auto copy

    /* overrides */
    Rectangle_fn.area = Rectangle_area;         // override
    Rectangle_fn.describe = Rectangle_describe; // override

    /* child only */
    Rectangle_fn.scaledArea = Rectangle_scaledArea;
    Rectangle_fn.diagonal = Rectangle_diagonal;
}

/* auto-run before main */
__attribute__((constructor)) static void Rectangle_auto(void)
{
    Rectangle_init();
}

/* ctor */
Rectangle *Rectangle_new()
{
    Rectangle *r = NEW(Rectangle);
    r->superFn = &Shape_fn;
    r->fn = &Rectangle_fn;

    return r;
}