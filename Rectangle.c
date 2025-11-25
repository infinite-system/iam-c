#include <stdio.h>
#include <math.h>
#include <string.h>

#include "iam.h"

#include "Shape.h"
#include "Rectangle.h"
/* ---- Implementations ---- */

static double area(Rectangle *r)
{
  printf("[Rectangle.area] called\n");
  double base = SUPER(Shape, r, area);
  printf("[Rectangle.area] base=%.2f\n", base);
  return base * r->multiplier * 3;
}

static double scaledArea(Rectangle *r)
{
  return r->w * r->h * r->multiplier;
}

static double diagonal(Rectangle *r)
{
  return sqrt(r->w * r->w + r->h * r->h);
}

static void describe(Rectangle *r)
{
  printf("[Rectangle] scaled=%.2f diag=%.2f\n",
         scaledArea(r),
         diagonal(r));
}

Rectangle_Fn Rectangle_fn;

/* class initializer */
void Rectangle_init(void)
{
  INHERIT_METHODS(Rectangle_fn, Shape_fn); // auto copy

  /* overrides */
  Rectangle_fn.area = area;         // override
  Rectangle_fn.describe = describe; // override

  /* child only */
  Rectangle_fn.scaledArea = scaledArea;
  Rectangle_fn.diagonal = diagonal;
}

/* auto-run before main */
// __attribute__((constructor)) static void Rectangle_auto(void)
// {
//   Rectangle_init();
// }

/* ctor */
Rectangle *Rectangle_new()
{
  Rectangle *r = NEW(Rectangle);
  r->fn = &Rectangle_fn;

  return r;
}