#include "app/Shape.h"
#include <stdio.h>
#include <stdlib.h>

/* ---- implementations ---- */

static double
area(Shape *s) {
  double base = s->w * s->h * 3;
  printf("[Shape.area] w=%.2f h=%.2f base=%.2f\n", s->w, s->h, base);
  return base;
}

static double
perimeter(Shape *s) {
  return 2 * (s->w + s->h);
}

static void
describe(Shape *s) {
  printf("[Shape] w=%.2f h=%.2f area=%.2f\n", s->w, s->h, area(s));
}

Shape_Fn Shape_fn;

#define Shape_PUBLIC_METHODS                                                                                 \
  /* Core geometry */                                                                                        \
  X(area)                                                                                                    \
  X(describe)                                                                                                \
  X(perimeter)

#define Shape_IMPLEMENTED_METHODS Shape_PUBLIC_METHODS

/* Class prototype */
void
Shape_prototype(void) {
#define X(name) Shape_fn.name = name;
  Shape_IMPLEMENTED_METHODS
#undef X
}

__attribute__((constructor)) static void
register_Shape(void) {
  iam_Kernel_register(Shape_prototype);
}

Shape *
Shape_new() {
  Shape *s = NEW(Shape);
  s->fn = &Shape_fn;

  return s;
}