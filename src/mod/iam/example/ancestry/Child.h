// clang-format off
#pragma once

#ifndef iam_example_ancestry_Child_H
#define iam_example_ancestry_Child_H

#include "mod/iam/core/iam.h"
#include "mod/iam/example/ancestry/Parent.h"

/* === FIELDS === */
#define iam_example_ancestry_Child_FIELD_LIST \
  iam_example_ancestry_Parent_FIELD_LIST

typedef struct iam_example_ancestry_Child {
#define IAMC_FIELD(name, type) type name;
  iam_example_ancestry_Child_FIELD_LIST
#undef IAMC_FIELD
} iam_example_ancestry_Child;

/* === METHODS === */

#define iam_example_ancestry_Child_METHOD_LIST(INSTANCE, STATIC, Global, Class) \
  iam_example_ancestry_Parent_METHOD_LIST(INSTANCE, STATIC, Global, Class) 

IAMC_DECLARE_METHODS(iam_example_ancestry_Child)

#endif
