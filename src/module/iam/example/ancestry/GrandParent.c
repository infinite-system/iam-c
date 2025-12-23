#include "module/iam/example/ancestry/GrandParent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

iam_example_ancestry_GrandParent_Fn iam_example_ancestry_GrandParent_fn;
static iam_example_ancestry_GrandParent *
constructor(void *self, const char *name, int age, int height, double weight, char gender) {
  iam_example_ancestry_GrandParent *gp = self;
  gp->fn = &iam_example_ancestry_GrandParent_fn;
  iam_strcpy(gp->name, sizeof gp->name, name);
  gp->age = age;
  gp->height = height;
  gp->weight = weight;
  gp->gender = gender;
  return gp;
}

static void
eat(void *self, double foodWeight) {
  iam_example_ancestry_GrandParent *gp = self;
  gp->weight += foodWeight * 0.25;
}

static void
diet(void *self, double daysLength) {
  iam_example_ancestry_GrandParent *gp = self;
  gp->weight -= daysLength * 0.1;
}

static void
describe(void *self) {
  iam_example_ancestry_GrandParent *gp = self;
  printf("GrandParent %s: Age %d, Height %d, Weight %.2f, Gender %c\n",
         gp->name,
         gp->age,
         gp->height,
         gp->weight,
         gp->gender);
}

#define iam_example_ancestry_GrandParent_IMPLEMENTED_METHODS                                                 \
  /* Core geometry */                                                                                        \
  X(constructor)                                                                                             \
  X(eat)                                                                                                     \
  X(diet)                                                                                                    \
  X(describe)

/* Class prototype */
void
iam_example_ancestry_GrandParent_prototype(void) {
#define X(name) iam_example_ancestry_GrandParent_fn.name = name;
  iam_example_ancestry_GrandParent_IMPLEMENTED_METHODS
#undef X
}

__attribute__((constructor)) static void
register_iam_example_ancestry_GrandParent(void) {
  iam_Kernel_register(iam_example_ancestry_GrandParent_prototype);
}

iam_example_ancestry_GrandParent *
iam_example_ancestry_GrandParent_new(const char *name, int age, int height, double weight, char gender) {
  void *self = NEW(iam_example_ancestry_GrandParent);
  return iam_example_ancestry_GrandParent_fn.constructor(self, name, age, height, weight, gender);
}