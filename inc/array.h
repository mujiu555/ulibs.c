#ifndef ULIB__ARRAY_H__
#define ULIB__ARRAY_H__

#include "util.h"
struct array_t {
  struct {
    size_t   size;
    size_t   len;
    size_t   cap;
    ulib_u8i flag;
  } head;
  ulib_u8i data[];
};
#endif // !ULIB__ARRAY_H__
