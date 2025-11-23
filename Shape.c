#include <stdio.h>
#include "Shape.h"

/* ---- implementations ---- */

static double area(Shape *s)
{
    double base = s->w * s->h * 3;
    printf("[Shape.area] w=%.2f h=%.2f base=%.2f\n", s->w, s->h, base);
    return base;
}

static double perimeter(Shape *s)
{
    return 2 * (s->w + s->h);
}

static void describe(Shape *s)
{
    printf("[Shape] w=%.2f h=%.2f area=%.2f\n",
           s->w, s->h, area(s));
} 

/* ---- vtable ---- */

Shape_Fn Shape_fn;

/* class initializer */
static void Shape_init(void)
{
    Shape_fn.area      = area;
    Shape_fn.perimeter = perimeter;
    Shape_fn.describe  = describe;
}

/* auto-run BEFORE main() */
__attribute__((constructor))
static void Shape_auto(void) {
    Shape_init();
}


/* ---- constructor ---- */

Shape *Shape_new()
{
    Shape *s = NEW(Shape);
    s->fn = &Shape_fn;

    return s;
}