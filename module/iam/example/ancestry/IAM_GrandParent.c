#include "IAM_GrandParent.h"
#include <stdio.h>
#include <string.h>

static GrandParent *constructor(void *self, const char *name, int age, int height, double weight, char gender) {
  GrandParent *gp = self;
  gp->fn = &GrandParent_fn;
  iam_strcpy(gp->name, sizeof gp->name, name);
  gp->age = age;
  gp->height = height;
  gp->weight = weight;
  gp->gender = gender;
  return gp;
}

static void eat(void *self, double foodWeight) {
  GrandParent *gp = self;
  gp->weight += foodWeight * 0.25;
}

static void diet(void *self, double daysLength) {
  GrandParent *gp = self;
  gp->weight -= daysLength * 0.1;
}

static void describe(void *self) {
  GrandParent *gp = self;
  printf("GrandParent %s: Age %d, Height %d, Weight %.2f, Gender %c\n",
         gp->name,
         gp->age,
         gp->height,
         gp->weight,
         gp->gender);
}

GrandParent_Fn GrandParent_fn;

#define GrandParent_IMPLEMENTED_METHODS \
  /* Core geometry */                   \
  X(constructor)                        \
  X(eat)                                \
  X(diet)                               \
  X(describe)

/* Class prototype */
void GrandParent_prototype(void) {
#define X(name) GrandParent_fn.name = name;
  GrandParent_IMPLEMENTED_METHODS
#undef X
}

__attribute__((constructor)) static void register_GrandParent(void) {
  iam_register(GrandParent_prototype);
}

GrandParent *GrandParent_new(const char *name, int age, int height, double weight, char gender) {
  void *self = NEW(GrandParent);
  return GrandParent_fn.constructor(self, name, age, height, weight, gender);
}