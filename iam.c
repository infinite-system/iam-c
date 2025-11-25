// iam.c
#include "iam.h"

static InitFn registry[2048];
static size_t registry_count = 0;

void iam_register(InitFn fn) {
    registry[registry_count++] = fn;
}

void iam_boot(void) {
    for (size_t i = 0; i < registry_count; i++)
        registry[i]();
}
