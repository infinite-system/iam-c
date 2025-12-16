#include "IAM_Hash.h"
#include <assert.h>

/* === PRIVATE INSTANCE METHODS === */

static inline IAM_HashEntry *_findSlot(IAM_Hash *self, const char *key) {
  /* Small, static, borrowed hash */
  if (self->cap <= IAM_Hash_static._getLinearLimit()) {
    for (size_t i = 0; i < self->cap; i++) {
      IAM_HashEntry *entry = &self->entries[i];

      if (!entry->key || strcmp(entry->key, key) == 0) {
        return entry;
      }
    }
    return NULL;
  }

  /* Large, owned, pool-backed hash */
  /* Placeholder for real hashing (open addressing / robin hood) */
  return self->fn->_findSlotHashed(self, key);
}

static inline IAM_HashEntry *_findSlotHashed(IAM_Hash *self, const char *key) {
/**
 * Invariant:
 *  This method fires only when the hash is in 'large' mode,
 *  if cap is greater than IAM_HASH_LINEAR_LIMIT,
 *  then cap MUST BE the power of two binary number or 8/16/32/64/128/256/512/1024/2048/4096/8192/16384/32768/65536...
 */
#ifndef NDEBUG
  assert(self->cap > IAM_Hash_static._getLinearLimit());
  assert(IAM_Hash_static._isPowerOfTwo(self->cap));
#endif
  if (self->count == self->cap) {
    return NULL;
  }
  size_t mask = self->cap - 1; /* cap must be power of two */
  size_t h = IAM_Hash_static._getKeySize(key) & mask;
  for (;;) {
    IAM_HashEntry *entry = &self->entries[h];
    if (!entry->key || strcmp(entry->key, key) == 0) {
      return entry;
    }
    h = (h + 1) & mask;
  }
}

/** === PUBLIC INSTANCE METHODS === */

static void init(IAM_Hash *self, IAM_HashEntry *entries, size_t cap) {
  self->entries = entries;
  self->cap = cap;
  self->count = 0;
  memset(entries, 0, sizeof(IAM_HashEntry) * cap);
}

static int setInt(IAM_Hash *self, const char *key, int value) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry) {
    return 0;
  }
  if (!entry->key) {
    self->count++;
  }
  entry->key = key;
  entry->type = IAM_Hash_TYPE_INT;
  entry->value.$integer = value;
  return 1;
}

static int setDouble(IAM_Hash *self, const char *key, double value) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry) {
    return 0;
  }
  if (!entry->key) {
    self->count++;
  }
  entry->key = key;
  entry->type = IAM_Hash_TYPE_DOUBLE;
  entry->value.$double = value;
  return 1;
}

static int setPtr(IAM_Hash *self, const char *key, void *value) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry) {
    return 0;
  }
  if (!entry->key) {
    self->count++;
  }
  entry->key = key;
  entry->type = IAM_Hash_TYPE_PTR;
  entry->value.$pointer = value;
  return 1;
}

static int setStr(IAM_Hash *self, const char *key, const char *value) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry) {
    return 0;
  }
  if (!entry->key) {
    self->count++;
  }
  entry->key = key;
  entry->type = IAM_Hash_TYPE_STR;
  entry->value.$string = value;
  return 1;
}

static int getInt(IAM_Hash *self, const char *key, int *out) {
  if (!out) {
    return 0;
  }

  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry || !entry->key) {
    return 0;
  }

  if (entry->type != IAM_Hash_TYPE_INT) {
    return 0;
  }

  *out = entry->value.$integer;
  return 1;
}

static int getDouble(IAM_Hash *self, const char *key, double *out) {
  if (!out) {
    return 0;
  }

  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry || !entry->key) {
    return 0;
  }

  if (entry->type != IAM_Hash_TYPE_DOUBLE) {
    return 0;
  }

  *out = entry->value.$double;
  return 1;
}

static int getPtr(IAM_Hash *self, const char *key, void **out) {
  if (!out) {
    return 0;
  }

  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry || !entry->key) {
    return 0;
  }

  if (entry->type != IAM_Hash_TYPE_PTR) {
    return 0;
  }

  *out = entry->value.$pointer;
  return 1;
}

static int getStr(IAM_Hash *self, const char *key, const char **out) {
  if (!out) {
    return 0;
  }

  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  if (!entry || !entry->key) {
    return 0;
  }

  if (entry->type != IAM_Hash_TYPE_STR) {
    return 0;
  }

  *out = entry->value.$string;
  return 1;
}

static int hasInt(IAM_Hash *self, const char *key) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  return entry && entry->key && entry->type == IAM_Hash_TYPE_INT;
}

static int hasDouble(IAM_Hash *self, const char *key) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  return entry && entry->key && entry->type == IAM_Hash_TYPE_DOUBLE;
}

static int hasPtr(IAM_Hash *self, const char *key) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  return entry && entry->key && entry->type == IAM_Hash_TYPE_PTR;
}

static int hasStr(IAM_Hash *self, const char *key) {
  IAM_HashEntry *entry = self->fn->_findSlot(self, key);
  return entry && entry->key && entry->type == IAM_Hash_TYPE_STR;
}

/* === STATIC PRIVATE METHODS === */

static inline int _static_getDefaultCap() {
  return IAM_HASH_DEFAULT_CAP;
}

static inline int _static_getLinearLimit() {
  return IAM_HASH_LINEAR_LIMIT;
}

static size_t _static_getKeySize(const char *s) {
  size_t h = 1469598103934665603ULL; // FNV offset
  while (*s) {
    h ^= (unsigned char)*s++;
    h *= 1099511628211ULL;
  }
  return h;
}

static inline int _static_isPowerOfTwo(size_t x) {
  return x && ((x & (x - 1)) == 0);
}

/* === STATIC PUBLIC METHODS === */

static void static_init(IAM_Hash *hash, IAM_HashEntry *entries, size_t cap) {
#ifndef NDEBUG
  if (cap > IAM_Hash_static._getLinearLimit()) {
    assert(IAM_Hash_static._isPowerOfTwo(cap));
  }
#endif
  hash->fn = &IAM_Hash_fn;
  IAM_Hash_fn.init(hash, entries, cap);
}

static int static_setDouble(IAM_Hash *hash, const char *key, double value) {
  return hash->fn->setDouble(hash, key, value);
}

static int static_setInt(IAM_Hash *hash, const char *key, int value) {
  return hash->fn->setInt(hash, key, value);
}

static int static_setPtr(IAM_Hash *hash, const char *key, void *value) {
  return hash->fn->setPtr(hash, key, value);
}

static int static_setStr(IAM_Hash *hash, const char *key, const char *value) {
  return hash->fn->setStr(hash, key, value);
}

static int static_getDouble(IAM_Hash *hash, const char *key, double *out) {
  return hash->fn->getDouble(hash, key, out);
}

static int static_getInt(IAM_Hash *hash, const char *key, int *out) {
  return hash->fn->getInt(hash, key, out);
}

static int static_getPtr(IAM_Hash *hash, const char *key, void **out) {
  return hash->fn->getPtr(hash, key, out);
}

static int static_getStr(IAM_Hash *hash, const char *key, const char **out) {
  return hash->fn->getStr(hash, key, out);
}

static int static_hasDouble(IAM_Hash *hash, const char *key) {
  return hash->fn->hasDouble(hash, key);
}

static int static_hasInt(IAM_Hash *hash, const char *key) {
  return hash->fn->hasInt(hash, key);
}

static int static_hasPtr(IAM_Hash *hash, const char *key) {
  return hash->fn->hasPtr(hash, key);
}

static int static_hasStr(IAM_Hash *hash, const char *key) {
  return hash->fn->hasStr(hash, key);
}

static IAM_Hash *static_new(size_t cap) {
#ifndef NDEBUG
  if (cap > IAM_Hash_static._getLinearLimit()) {
    assert(IAM_Hash_static._isPowerOfTwo(cap));
  }
#endif
  IAM_Hash *hash = malloc(sizeof(IAM_Hash));
  hash->fn = &IAM_Hash_fn;
  hash->entries = malloc(sizeof(IAM_HashEntry) * cap);
  hash->cap = cap;
  memset(hash->entries, 0, sizeof(IAM_HashEntry) * cap);
  return hash;
}

/** === CLASS INSTANCE NEW === */

IAM_Hash *IAM_Hash_new(size_t cap) {
  return IAM_Hash_static.new(cap);
}

/* === CLASS INSTANCE PROTOTYPE === */

IAM_Hash_Fn IAM_Hash_fn = {
  /* === PRIVATE INSTANCE METHODS === */
  ._findSlot = _findSlot,
  ._findSlotHashed = _findSlotHashed,

  /* === PUBLIC INSTANCE METHODS === */
  .init = init,
  /** Setters */
  .setInt = setInt,
  .setDouble = setDouble,
  .setPtr = setPtr,
  .setStr = setStr,
  /** Getters */
  .getInt = getInt,
  .getDouble = getDouble,
  .getPtr = getPtr,
  .getStr = getStr,
  /** Has Checks */
  .hasInt = hasInt,
  .hasDouble = hasDouble,
  .hasPtr = hasPtr,
  .hasStr = hasStr
};

/* === CLASS STATIC PROTOTYPE === */

IAM_Hash_Static_Fn IAM_Hash_static = {
  /* === PRIVATE STATIC METHODS === */
  ._getDefaultCap = _static_getDefaultCap,
  ._getLinearLimit = _static_getLinearLimit,
  ._getKeySize = _static_getKeySize,
  ._isPowerOfTwo = _static_isPowerOfTwo,
  /* === PUBLIC STATIC METHODS === */
  .init = static_init,
  .new = static_new,
  /* Setters */
  .setDouble = static_setDouble,
  .setInt = static_setInt,
  .setPtr = static_setPtr,
  .setStr = static_setStr,
  /* Getters */
  .getDouble = static_getDouble,
  .getInt = static_getInt,
  .getPtr = static_getPtr,
  .getStr = static_getStr,
  /* Has Checks */
  .hasDouble = static_hasDouble,
  .hasInt = static_hasInt,
  .hasPtr = static_hasPtr,
  .hasStr = static_hasStr
};
