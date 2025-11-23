#ifndef IAM_H
#define IAM_H

#include <stdlib.h>

/* Allocate */
#define NEW(T) (T*)malloc(sizeof(T))

/* SUPER(r, area) */
#define SUPER(CLS, self, METHOD) \
    (((CLS##_Fn*)CLS##_fn.super)->METHOD((void*)(self)))
    
#define INHERIT_METHODS(child_fn, parent_fn) \
    memcpy(&(child_fn), &(parent_fn), sizeof(parent_fn));

#endif