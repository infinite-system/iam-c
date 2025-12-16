// IAM_Id.h
#ifndef IAM_Id_H
#define IAM_Id_H

#include <stdint.h>

/* 64-bit generational handle */
typedef uint64_t IAM_Id;

#define IAM_Id_INVALID ((IAM_Id)0)

/* Bit layout invariant:
 *  - low  32 bits: index
 *  - high 32 bits: generation
 */

static inline int IAM_Id_isValid(IAM_Id id) {
  return id != IAM_Id_INVALID;
}

static inline uint32_t IAM_Id_index(IAM_Id id) {
  return (uint32_t)(id & 0xFFFFFFFFu);
}

static inline uint32_t IAM_Id_generation(IAM_Id id) {
  return (uint32_t)(id >> 32);
}

static inline IAM_Id IAM_Id_pack(uint32_t index, uint32_t generation) {
  return ((IAM_Id)generation << 32) | (IAM_Id)index;
}

#endif /* IAM_Id_H */