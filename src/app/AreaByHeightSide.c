#include <math.h>
#include <stdio.h>
#include <string.h>

#include "app/AreaByHeightSide.h"

static double sideArea(void *self, const AreaBySideHeight_Adapter *fn) {
  return 0.5 * fn->getSide(self) * fn->getHeight(self);
}

static double sideArea2(void *self, const AreaBySideHeight_Adapter *fn) {
  return AreaBySideHeight_fn.sideArea(self, fn) / 2;
}

AreaBySideHeight_Fn AreaBySideHeight_fn;

#define AREA_BY_SIDE_HEIGHT_IMPLEMENTED \
  X(sideArea)                           \
  X(sideArea2)

void AreaBySideHeight_prototype(void) {
#define X(name) AreaBySideHeight_fn.name = name;
  AREA_BY_SIDE_HEIGHT_IMPLEMENTED
#undef X
}

__attribute__((constructor)) static void register_AreaBySideHeight(void) {
  iam_Kernel_register(AreaBySideHeight_prototype);
}