#pragma once

#ifndef IAM_EXAMPLE_ANCESTRY_CHILD_INHERITS_H
#define IAM_EXAMPLE_ANCESTRY_CHILD_INHERITS_H

void
iam_example_ancestry_Child_describe(iam_example_ancestry_Child * self) {
  iam_example_ancestry_Parent_describe((iam_example_ancestry_Parent *)self);
}

double
iam_example_ancestry_Child_diet(iam_example_ancestry_Child * self, double daysLength) {
  return iam_example_ancestry_Parent_diet((iam_example_ancestry_Parent *)self, daysLength);
}

double
iam_example_ancestry_Child_eat(iam_example_ancestry_Child * self, double foodWeight) {
  return iam_example_ancestry_Parent_eat((iam_example_ancestry_Parent *)self, foodWeight);
}

void
iam_example_ancestry_Child_relax(iam_example_ancestry_Child * self, double hours) {
  iam_example_ancestry_Parent_relax((iam_example_ancestry_Parent *)self, hours);
}

void
iam_example_ancestry_Child_set_occupation(iam_example_ancestry_Child * self, iam_str64 new_occupation) {
  iam_example_ancestry_Parent_set_occupation((iam_example_ancestry_Parent *)self, new_occupation);
}

void
iam_example_ancestry_Child_work(iam_example_ancestry_Child * self, double hours) {
  iam_example_ancestry_Parent_work((iam_example_ancestry_Parent *)self, hours);
}

iam_example_ancestry_Child *
iam_example_ancestry_Child_constructor(iam_example_ancestry_Child * self, const char *name, int age, int height, double weight, char gender) {
  iam_example_ancestry_Parent_constructor((iam_example_ancestry_Parent *)self, name, age, height, weight, gender);
  return self;
}

#endif
