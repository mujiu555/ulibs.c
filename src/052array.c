#include "array.h"
#include "util.h"

static inline array internal_alloc(ulib_size len, ulib_size size) {
  array obj = ulib_alloc(sizeof(array_t) + len * size);
  return obj;
}

array array_init() { }
