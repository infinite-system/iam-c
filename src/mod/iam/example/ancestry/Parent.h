// clang-format off
#pragma once

#ifndef iam_example_ancestry_Parent_H
#define iam_example_ancestry_Parent_H

#include "mod/iam/core/iam.h"
#include "mod/iam/example/ancestry/GrandParent.h"

/* === FIELDS === */

#define iam_example_ancestry_Parent_FIELD_LIST \
  iam_example_ancestry_GrandParent_FIELD_LIST \
  IAMC_FIELD(occupation, iam_str64) \
  IAMC_FIELD(total_worked, double) \
  IAMC_FIELD(total_relaxed, double)


typedef struct iam_example_ancestry_Parent {
#define IAMC_FIELD(name, type) type name;
  iam_example_ancestry_Parent_FIELD_LIST
#undef IAMC_FIELD
} iam_example_ancestry_Parent;

/* === METHODS === */

#define iam_example_ancestry_Parent_METHOD_LIST(INSTANCE, STATIC, Global, Class) \
  iam_example_ancestry_GrandParent_METHOD_LIST(INSTANCE, STATIC, Global, Class) \
  INSTANCE(Global, Class, work, (Class * self, double hours), void) \
  INSTANCE(Global, Class, relax, (Class * self, double hours), void) \
  INSTANCE(Global, Class, set_occupation, (Class * self, iam_str64 new_occupation), void)

IAMC_DECLARE_METHODS(iam_example_ancestry_Parent)

#endif
