// IAM_Helpers.h
#ifndef IAM_Helpers_H
#define IAM_Helpers_H
#include <stddef.h> // preferred for size_t

typedef char IAM_Str16[16];
typedef char IAM_Str64[64];
typedef char IAM_Str256[256];

static inline void iam_strcpy(char *dst, size_t cap, const char *src) {
  if (!src) {
    src = "";
  }
  size_t i = 0;
  for (; i + 1 < cap && src[i]; ++i) {
    dst[i] = src[i];
  }
  if (cap) {
    dst[i] = '\0';
  }
}
#endif