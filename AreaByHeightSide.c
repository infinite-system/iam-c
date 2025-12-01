#include <stdio.h>
#include <math.h>
#include <string.h>

#include "iam.h"
#include "AreaByHeightSide.h"

static double sideArea(void *self, const AreaBySideHeight_Adapter *fn)
{
  return 0.5 * fn->getSide(self) * fn->getHeight(self);
}

static double sideArea2(void *self, const AreaBySideHeight_Adapter *fn)
{
  return AreaBySideHeight_fn.sideArea(self, fn) / 2;
}

AreaBySideHeight_Fn AreaBySideHeight_fn;

/* Mixin initializer */
void AreaBySideHeight_init(void)
{
  AreaBySideHeight_fn.sideArea = sideArea;
  AreaBySideHeight_fn.sideArea2 = sideArea2;
}
