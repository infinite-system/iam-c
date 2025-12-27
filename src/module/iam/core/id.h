// iam_Id.h
#pragma once

#ifndef iam_Id_h
#define iam_Id_h

#include <stdint.h>

/* 64-bit generational handle */
typedef uint64_t iam_Id;

#define iam_Id_invalid ((iam_Id)0)

/* Bit layout invariant:
 *  - low  32 bits: index
 *  - high 32 bits: generation
 */

static inline int
iam_Id_is_valid(iam_Id id) {
  return id != iam_Id_invalid;
}

static inline uint32_t
iam_Id_index(iam_Id id) {
  return (uint32_t)(id & 0xFFFFFFFFu);
}

static inline uint32_t
iam_Id_generation(iam_Id id) {
  return (uint32_t)(id >> 32);
}

static inline iam_Id
iam_Id_pack(uint32_t index, uint32_t generation) {
  return ((iam_Id)generation << 32) | (iam_Id)index;
}

#endif /* iam_Id_h */