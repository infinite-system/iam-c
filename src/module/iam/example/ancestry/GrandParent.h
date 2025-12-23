#ifndef GrandParent_H
#define GrandParent_H

#include "module/iam/core/iam.h"

void
iam_example_ancestry_GrandParent_prototype(void);

/* X-macro field lists */
#define iam_example_ancestry_GrandParent_FIELD_LIST                                                          \
  X(name, iam_str64)                                                                                         \
  X(age, int)                                                                                                \
  X(height, int)                                                                                             \
  X(weight, double)                                                                                          \
  X(gender, char)

/* X-macro method lists */
#define iam_example_ancestry_GrandParent_METHOD_LIST                                                         \
  METHOD(iam_example_ancestry_GrandParent *, constructor, const char *, int, int, double, char)              \
  METHOD(void, eat, double)                                                                                  \
  METHOD(void, diet, double)                                                                                 \
  METHOD(void, describe)

/* Generate struct */
typedef struct iam_example_ancestry_GrandParent {
  struct iam_example_ancestry_GrandParent_Fn *fn;
/* fields */
#define X(name, type) type name;
  iam_example_ancestry_GrandParent_FIELD_LIST
#undef X
} iam_example_ancestry_GrandParent;

/* vtable */
typedef struct iam_example_ancestry_GrandParent_Fn {
#define METHOD(ret, name, ...) ret (*name)(void *, ##__VA_ARGS__);
  iam_example_ancestry_GrandParent_METHOD_LIST
#undef METHOD
} iam_example_ancestry_GrandParent_Fn;

extern iam_example_ancestry_GrandParent_Fn iam_example_ancestry_GrandParent_fn;

/* ctor */
iam_example_ancestry_GrandParent *
iam_example_ancestry_GrandParent_new(const char *name, int age, int height, double weight, char gender);

#endif
