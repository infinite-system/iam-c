#ifndef GrandParent_H
#define GrandParent_H

#include "module/iam/core/iam.h"

/* === FIELDS === */

#define iam_example_ancestry_GrandParent_FIELD_LIST                            \
  FIELD(name, iam_str64)                                                       \
  FIELD(age, int)                                                              \
  FIELD(height, int)                                                           \
  FIELD(weight, double)                                                        \
  FIELD(gender, char)

typedef struct iam_example_ancestry_GrandParent {
#define FIELD(name, type) type name;
  iam_example_ancestry_GrandParent_FIELD_LIST
#undef FIELD
} iam_example_ancestry_GrandParent;

/* === METHODS === */

#define iam_example_ancestry_GrandParent_METHOD_LIST(                          \
  INST, STAT, Global, Class                                                    \
)                                                                              \
  INST(Global, Class, diet, double, (Class * self, double daysLength))         \
  INST(Global, Class, eat, double, (Class * self, double foodWeight))          \
  INST(Global, Class, describe, void, (Class * self))                          \
  STAT(                                                                        \
    Global,                                                                    \
    Class,                                                                     \
    constructor,                                                               \
    Class *,                                                                   \
    (Class * self,                                                             \
     const char *name,                                                         \
     int age,                                                                  \
     int height,                                                               \
     double weight,                                                            \
     char gender)                                                              \
  )                                                                            \
  STAT(                                                                        \
    Global,                                                                    \
    Class,                                                                     \
    new,                                                                       \
    Class *,                                                                   \
    (const char *name, int age, int height, double weight, char gender)        \
  )

IAMC_DECLARE_METHODS(iam_example_ancestry_GrandParent)

#endif
