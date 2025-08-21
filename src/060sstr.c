#include "sstr.h"
#include "alloc.h"
#include "ept.h"
#include "util.h"
#include <uchar.h>

inline static sstr internal_alloc(ulib_size cap) {
  sstr obj = ulib_alloc(sizeof(sstr_t) + cap * sizeof(ulib_u8));
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  obj->head.len = 0;
  obj->head.cap = cap;
  return obj;
}

// NOTE: Default utf-8
inline static ulib_u32 internal_nextch(char8_t *cstr, int * /* out */ len) {
  // TODO:
  return 0;
}

sstr sstr_init(int cap, char *data) {
  sstr obj = internal_alloc(cap);
  return NULL;
}
