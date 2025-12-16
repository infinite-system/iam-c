#ifndef AREA_BY_SIDE_HEIGHT_H
#define AREA_BY_SIDE_HEIGHT_H

#include "module/iam/core/IAM.h"

void AreaBySideHeight_prototype(void);

__attribute__((constructor)) static void register_AreaBySideHeight(void) {
  iam_register(AreaBySideHeight_prototype);
}

#define AREA_BY_SIDE_HEIGHT_ADAPTER_METHOD_LIST \
  METHOD(double, getHeight)                     \
  METHOD(double, getSide)

#define AREA_BY_SIDE_HEIGHT_METHOD_LIST                        \
  METHOD(double, sideArea, const AreaBySideHeight_Adapter *fn) \
  METHOD(double, sideArea2, const AreaBySideHeight_Adapter *fn)

/* Child struct */
typedef struct AreaBySideHeight_Adapter {
#define METHOD(ret, name, ...) ret (*name)(void *, ##__VA_ARGS__);
  AREA_BY_SIDE_HEIGHT_ADAPTER_METHOD_LIST
#undef METHOD
} AreaBySideHeight_Adapter;

/* AreaBySideHeight vtable */
typedef struct AreaBySideHeight_Fn {
#define METHOD(ret, name, ...) ret (*name)(void *, ##__VA_ARGS__);
  AREA_BY_SIDE_HEIGHT_METHOD_LIST
#undef METHOD
} AreaBySideHeight_Fn;

extern AreaBySideHeight_Fn AreaBySideHeight_fn;

#endif