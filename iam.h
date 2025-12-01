#ifndef IAM_H
#define IAM_H

#include <stdlib.h>

typedef void (*InitFn)(void);

void iam_register(InitFn fn);
void iam_boot(void);

/* Allocate */
#define NEW(T) (T*)malloc(sizeof(T))
    
#define INHERIT_METHODS(child_fn, parent_fn) \
    memcpy(&(child_fn), &(parent_fn), sizeof(parent_fn));

#endif