// clang-format off
#pragma once

#ifndef iam_example_ancestry_GrandParent_H
#define iam_example_ancestry_GrandParent_H

#include "mod/iam/core/iam.h"

/* === FIELDS === */

#define iam_example_ancestry_GrandParent_FIELD_LIST \
  IAMC_FIELD(name, iam_str64) \
  IAMC_FIELD(age, int) \
  IAMC_FIELD(height, int) \
  IAMC_FIELD(weight, double) \
  IAMC_FIELD(gender, char)

typedef struct iam_example_ancestry_GrandParent {
#define IAMC_FIELD(name, type) type name;
  iam_example_ancestry_GrandParent_FIELD_LIST
#undef IAMC_FIELD
} iam_example_ancestry_GrandParent;

/* === METHODS === */

#define iam_example_ancestry_GrandParent_METHOD_LIST(INSTANCE, STATIC, Global, Class) \
  INSTANCE(Global, Class, diet, (Class * self, double daysLength), double) \
  INSTANCE(Global, Class, eat, (Class * self, double foodWeight), double) \
  INSTANCE(Global, Class, describe, (Class * self), void) \
  STATIC(Global, Class, constructor, (Class * self, const char *name, int age, int height, double weight, char gender), Class *) \
  STATIC(Global, Class, new, (const char *name, int age, int height, double weight, char gender), Class *)

IAMC_DECLARE_METHODS(iam_example_ancestry_GrandParent)

#endif
