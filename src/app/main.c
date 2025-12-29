#include <stdio.h>

#include "app/Rectangle.h"
#include "app/Shape.h"
#include "app/Triangle.h"
#include "mod/iam/core/iam.h"
#include "mod/iam/example/ancestry/Child.h"
#include "mod/iam/example/ancestry/GrandParent.h"
#include "mod/iam/example/ancestry/Parent.h"

IAMC_USING(iam_Kernel, Kernel)
IAMC_USING(iam_example_ancestry_GrandParent, GrandParent)
IAMC_USING(iam_example_ancestry_Parent, Parent)
IAMC_USING(iam_example_ancestry_Child, Child)

int
main() {
  Kernel_init();
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
  GrandParent_describe(gp);

  Parent *p = Parent_new("Jane Doe", 50, 165, 60.0, 'F');
  Parent_diet(p, 7.0);
  Parent_describe(p);

  Child *c = Child_new("Child Doe", 30, 150, 50.0, 'M');
  Child_work(c, 8.0);
  Child_relax(c, 2.0);
  Child_describe(c);

  return 0;
}