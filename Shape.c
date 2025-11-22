#include <stdio.h>
#include "Shape.h"

/* ---- implementations ---- */

static double Shape_area(Shape *s)
{
    return s->w * s->h;
}

static double Shape_perimeter(Shape *s)
{
    return 2 * (s->w + s->h);
}

static void Shape_describe(Shape *s)
{
    printf("[Shape] w=%.2f h=%.2f area=%.2f\n",
           s->w, s->h, Shape_area(s));
}

/* ---- vtable ---- */

Shape_Fn Shape_fn;

/* class initializer */
static void Shape_init(void)
{
    Shape_fn.area      = Shape_area;
    Shape_fn.perimeter = Shape_perimeter;
    Shape_fn.describe  = Shape_describe;
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