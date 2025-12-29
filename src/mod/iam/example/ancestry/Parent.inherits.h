#pragma once

#ifndef iam_example_ancestry_Parent_INHERITS_H
#define iam_example_ancestry_Parent_INHERITS_H

double
iam_example_ancestry_Parent_diet(iam_example_ancestry_Parent * self, double daysLength) {
  return iam_example_ancestry_GrandParent_diet((iam_example_ancestry_GrandParent *)self, daysLength);
}

double
iam_example_ancestry_Parent_eat(iam_example_ancestry_Parent * self, double foodWeight) {
  return iam_example_ancestry_GrandParent_eat((iam_example_ancestry_GrandParent *)self, foodWeight);
}

#endif
