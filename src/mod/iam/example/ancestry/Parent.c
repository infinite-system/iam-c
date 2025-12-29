#include "mod/iam/example/ancestry/Parent.h"
#include "mod/iam/example/ancestry/Parent.inherits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IAMC_REQUIRE_IMPLEMENTED(iam_example_ancestry_Parent)
IAMC_USING(iam_example_ancestry_GrandParent, GrandParent)

#define Parent_(fn) iam_example_ancestry_Parent_##fn
typedef iam_example_ancestry_Parent Parent;

Parent *
Parent_(constructor)(
  Parent *p, const char *name, int age, int height, double weight, char gender
) {
  GrandParent_constructor((GrandParent *)p, name, age, height, weight, gender);
  iam_strcpy(p->occupation, sizeof p->occupation, "Unemployed");
  p->total_worked = 0.0;
  p->total_relaxed = 0.0;
  return p;
}

Parent *
Parent_(new)(
  const char *name, int age, int height, double weight, char gender
) {
  Parent *p = NEW(Parent);
  return Parent_(constructor)(p, name, age, height, weight, gender);
}

void
Parent_(set_occupation)(Parent *p, iam_str64 new_occupation) {
  iam_strcpy(p->occupation, sizeof p->occupation, new_occupation);
}

void
Parent_(work)(Parent *p, double hours) {
  p->total_worked += hours;
  printf(
    "%s is working for %.2f hours as a %s.\n", p->name, hours, p->occupation
  );
}

void
Parent_(relax)(Parent *p, double hours) {
  p->total_relaxed += hours;
  printf("%s is relaxing for %.2f hours.\n", p->name, hours);
}

void
Parent_(describe)(Parent *p) {
  printf(
    "Parent %s: Age %d, Height %d, Weight %.2f, Occupation %s\n, Worked %.2f "
    "hours, Relaxed %.2f hours\n",
    p->name,
    p->age,
    p->height,
    p->weight,
    p->occupation,
    p->total_worked,
    p->total_relaxed
  );
}

#undef Parent_