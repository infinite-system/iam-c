#include "Shape.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <stdio.h>

int main()
{
    Shape *s = Shape_new();
    s->w = 4;
    s->h = 3;

    s->fn->describe(s);

    Rectangle *r = Rectangle_new();
    r->w = 10;
    r->h = 5;
    r->multiplier = 3;

    r->fn->describe(r);
    printf("[Rectangle] perimeter=%.2f\n", r->fn->perimeter(r));

    Triangle *t = Triangle_new();
    t->w = 10;
    t->h = 5;
    t->sideLength = 6;
    t->multiplier = 3;

    t->fn->describe(t);

    return 0;
}