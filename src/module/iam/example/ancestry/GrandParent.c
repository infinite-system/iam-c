#include "module/iam/example/ancestry/GrandParent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IAMC_REQUIRE_IMPLEMENTED(iam_example_ancestry_GrandParent)

typedef iam_example_ancestry_GrandParent GrandParent;

GrandParent *
iam_example_ancestry_GrandParent_constructor(
  GrandParent *gp,
  const char *name,
  int age,
  int height,
  double weight,
  char gender
) {
  iam_strcpy(gp->name, sizeof gp->name, name);
  gp->age = age;
  gp->height = height;
  gp->weight = weight;
  gp->gender = gender;
  return gp;
}

GrandParent *
iam_example_ancestry_GrandParent_new(
  const char *name, int age, int height, double weight, char gender
) {
  GrandParent *gp = NEW(GrandParent);
  return iam_example_ancestry_GrandParent_constructor(
    gp, name, age, height, weight, gender
  );
}

double
iam_example_ancestry_GrandParent_eat(GrandParent *gp, double foodWeight) {
  gp->weight += foodWeight * 0.25;
  return gp->weight;
}

double
iam_example_ancestry_GrandParent_diet(GrandParent *gp, double daysLength) {
  gp->weight -= daysLength * 0.15;
  return gp->weight;
}

void
iam_example_ancestry_GrandParent_describe(GrandParent *gp) {
  printf(
    "GrandParent %s: Age %d, Height %d, Weight %.2f, Gender %c\n",
    gp->name,
    gp->age,
    gp->height,
    gp->weight,
    gp->gender
  );
}
