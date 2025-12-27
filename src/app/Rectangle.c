#include "app/Rectangle.h"
#include "app/Shape.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* === PRIVATE METHODS === */

static double _privateMethod(Rectangle *r) {
  return r->w * r->h * r->multiplier * 42;
}

/* === PUBLIC METHODS === */

static double area(Rectangle *r) {
  printf("[Rectangle.area] called\n");
  double base = Shape_fn.area((Shape *)r);
  printf("[Rectangle.area] base=%.2f\n", base);
  return base * r->multiplier * 3 * r->fn->_privateMethod(r);
}

static double scaledArea(Rectangle *r) { return r->w * r->h * r->multiplier; }

static double diagonal(Rectangle *r) { return sqrt(r->w * r->w + r->h * r->h); }

static void describe(Rectangle *r) {
  printf("[Rectangle] scaled=%.2f diag=%.2f\n", scaledArea(r), diagonal(r));
}

static void parentMethod(Rectangle *r, double x) {
  printf("[Rectangle.parentMethod] called with x=%.2f\n", x);
}

Rectangle_Fn Rectangle_fn;

#define Rectangle_PRIVATE_METHODS X(_privateMethod)

#define Rectangle_PUBLIC_METHODS                                               \
  /* Core geometry */                                                          \
  X(area)                                                                      \
  X(describe)                                                                  \
  X(scaledArea)                                                                \
  X(diagonal)                                                                  \
  X(parentMethod)

#define Rectangle_IMPLEMENTED_METHODS                                          \
  Rectangle_PRIVATE_METHODS Rectangle_PUBLIC_METHODS

/* class initializer */
void Rectangle_prototype(void) {
  static int done = 0;
  if (done)
    return;
  done = 1;
  Shape_prototype(); // ensure parent prototype is initialized
  INHERIT_METHODS_FROM(Shape_fn, Rectangle_fn); // auto copy

#define X(name) Rectangle_fn.name = name;
  Rectangle_IMPLEMENTED_METHODS
#undef X
}

__attribute__((constructor)) static void register_Rectangle(void) {
  iam_Kernel_register(Rectangle_prototype);
}

/* ctor */
Rectangle *Rectangle_new() {
  Rectangle *r = NEW(Rectangle);
  r->fn = &Rectangle_fn;

  return r;
}