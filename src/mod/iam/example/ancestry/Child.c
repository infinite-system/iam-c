#include "mod/iam/example/ancestry/Child.h"
#include "mod/iam/example/ancestry/Child.inherits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

IAMC_REQUIRE_IMPLEMENTED(iam_example_ancestry_Child)
IAMC_USING(iam_example_ancestry_Parent, Parent)

#define Child_(fn) iam_example_ancestry_Child_##fn
typedef iam_example_ancestry_Child Child;

Child *
Child_(new)(const char *name, int age, int height, double weight, char gender) {
  Child *c = NEW(Child);
  return Child_(constructor)(c, name, age, height, weight, gender);
}