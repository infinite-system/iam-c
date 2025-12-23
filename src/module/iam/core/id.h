// iam_id.h
#ifndef iam_id_h
#define iam_id_h

#include <stdint.h>

/* 64-bit generational handle */
typedef uint64_t iam_id;

#define iam_id_invalid ((iam_id)0)

/* Bit layout invariant:
 *  - low  32 bits: index
 *  - high 32 bits: generation
 */

static inline int
iam_id_isValid(iam_id id) {
  return id != iam_id_invalid;
}

static inline uint32_t
iam_id_index(iam_id id) {
  return (uint32_t)(id & 0xFFFFFFFFu);
}

static inline uint32_t
iam_id_generation(iam_id id) {
  return (uint32_t)(id >> 32);
}

static inline iam_id
iam_id_pack(uint32_t index, uint32_t generation) {
  return ((iam_id)generation << 32) | (iam_id)index;
}

#endif /* iam_id_h */