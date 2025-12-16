#ifndef IAM_Hash_H
#define IAM_Hash_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* === MACROS === */

#define HASH_INIT(hash_field, entries_array) \
  IAM_Hash_static.init(                      \
      &(hash_field),                         \
      (entries_array),                       \
      sizeof(entries_array) / sizeof((entries_array)[0]))

#define HASH_SET(hash, key, value, Type) \
  IAM_Hash_static.set##Type(&(hash), (key), (value))

#define HASH_GET(hash, key, Type) \
  IAM_Hash_static.get##Type(&(hash), (key))

#define HASH_HAS(hash, key, Type) \
  IAM_Hash_static.has##Type(&(hash), (key))

/* === DEFAULTS === */

#define IAM_HASH_DEFAULT_CAP 32
#define IAM_HASH_LINEAR_LIMIT 64

/* === HASH TYPES === */

typedef enum {
  IAM_Hash_TYPE_NONE = 0,
  IAM_Hash_TYPE_INT,
  IAM_Hash_TYPE_DOUBLE,
  IAM_Hash_TYPE_PTR,
  IAM_Hash_TYPE_STR
} IAM_HashType;

/* === HASH ENTRY === */

typedef struct {
  const char *key;
  IAM_HashType type;
  union {
    int $integer;
    double $double;
    void *$pointer;
    const char *$string;
  } value;
} IAM_HashEntry;

/* === HASH INSTANCE === */

#define IAM_Hash_FIELD_LIST         \
  X_FIELD(entries, IAM_HashEntry *) \
  X_FIELD(cap, size_t)              \
  X_FIELD(count, size_t)

typedef struct IAM_Hash {
  struct IAM_Hash_Fn *fn;
#define X_FIELD(name, type) type name;
  IAM_Hash_FIELD_LIST
#undef X_FIELD
} IAM_Hash;

#define IAM_Hash_METHOD_LIST                                \
  METHOD(IAM_HashEntry *, _findSlot, const char *key)       \
  METHOD(IAM_HashEntry *, _findSlotHashed, const char *key) \
  METHOD(void, init, IAM_HashEntry *entries, size_t cap)    \
  METHOD(int, setInt, const char *key, int value)           \
  METHOD(int, setDouble, const char *key, double value)     \
  METHOD(int, setPtr, const char *key, void *value)         \
  METHOD(int, setStr, const char *key, const char *value)   \
  METHOD(int, getInt, const char *key, int *out)            \
  METHOD(int, getDouble, const char *key, double *out)      \
  METHOD(int, getPtr, const char *key, void **out)          \
  METHOD(int, getStr, const char *key, const char **out)    \
  METHOD(int, hasInt, const char *key)                      \
  METHOD(int, hasDouble, const char *key)                   \
  METHOD(int, hasPtr, const char *key)                      \
  METHOD(int, hasStr, const char *key)

typedef struct IAM_Hash_Fn {
#define METHOD(ret, name, ...) ret (*name)(IAM_Hash * self, ##__VA_ARGS__);
  IAM_Hash_METHOD_LIST
#undef METHOD
} IAM_Hash_Fn;

extern IAM_Hash_Fn IAM_Hash_fn;

/* === STATIC METHODS === */

#define IAM_Hash_STATIC_METHOD_LIST                                       \
  METHOD(int, _getDefaultCap)                                             \
  METHOD(int, _getLinearLimit)                                            \
  METHOD(size_t, _getKeySize, const char *key)                            \
  METHOD(int, _isPowerOfTwo, size_t x)                                    \
  METHOD(void, init, IAM_Hash *hash, IAM_HashEntry *entries, size_t cap)  \
  METHOD(IAM_Hash *, new, size_t cap)                                     \
  METHOD(int, setInt, IAM_Hash *hash, const char *key, int value)         \
  METHOD(int, setDouble, IAM_Hash *hash, const char *key, double value)   \
  METHOD(int, setPtr, IAM_Hash *hash, const char *key, void *value)       \
  METHOD(int, setStr, IAM_Hash *hash, const char *key, const char *value) \
  METHOD(int, getInt, IAM_Hash *hash, const char *key, int *out)          \
  METHOD(int, getDouble, IAM_Hash *hash, const char *key, double *out)    \
  METHOD(int, getPtr, IAM_Hash *hash, const char *key, void **out)        \
  METHOD(int, getStr, IAM_Hash *hash, const char *key, const char **out)  \
  METHOD(int, hasInt, IAM_Hash *hash, const char *key)                    \
  METHOD(int, hasDouble, IAM_Hash *hash, const char *key)                 \
  METHOD(int, hasPtr, IAM_Hash *hash, const char *key)                    \
  METHOD(int, hasStr, IAM_Hash *hash, const char *key)

typedef struct IAM_Hash_Static_Fn {
#define METHOD(ret, name, ...) ret (*name)(__VA_ARGS__);
  IAM_Hash_STATIC_METHOD_LIST
#undef METHOD
} IAM_Hash_Static_Fn;

extern IAM_Hash_Static_Fn IAM_Hash_static;
#endif