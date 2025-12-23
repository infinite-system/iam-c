#include <stdio.h>

#include "Rectangle.h"
#include "Shape.h"
#include "Triangle.h"
#include "module/iam/core/IAM.h"
#include "module/iam/example/ancestry/IAM_GrandParent.h"

int main() {
  iam_boot();
  Triangle *t = Triangle_new();
  t->w = 10;
  t->h = 51;
  t->sideLength = 6;
  t->multiplier = 3;

  t->fn->describe(t);
  t->fn->parentMethod(t, 42.0);

  printf("----\n");
  Shape *s = Shape_new();
  s->w = 4;
  s->h = 3;

  s->fn->describe(s);

  // printf("----\n");
  // Rectangle *r = Rectangle_new();
  // r->w = 10;
  // r->h = 5;
  // r->multiplier = 5;

  // r->fn->describe(r);
  // printf("[Rectangle] perimeter=%.2f\n", r->fn->perimeter(r));
  // printf("[Rectangle] area=%.2f\n", r->fn->area(r));

  printf("----\n");

  Triangle *t2 = Triangle_new();
  t2->w = 3;
  t2->h = 3;
  t2->sideLength = 6;
  t2->multiplier = 3;

  t2->fn->describe(t2);

  GrandParent *gp = GrandParent_new("John Doe", 75, 180, 80.5, 'M');
  gp->fn->describe(gp);

  return 0;
}