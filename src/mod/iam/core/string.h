#pragma once

// IAMC_Helpers.h
#ifndef iam_string_H
#define iam_string_H
#include <stddef.h> // preferred for size_t

typedef char iam_str16[16];
typedef char iam_str64[64];
typedef char iam_str256[256];

static inline void
iam_strcpy(char *dst, size_t cap, const char *src) {
  if (!src)
    src = "";

  size_t i = 0;
  for (; i + 1 < cap && src[i]; ++i)
    dst[i] = src[i];

  if (cap)
    dst[i] = '\0';
}
#endif