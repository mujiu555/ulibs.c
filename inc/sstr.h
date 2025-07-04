#ifndef ULIB__SSTR_H__
#define ULIB__SSTR_H__

#include "util.h"

// UTF-8
struct Sstr_t {
  struct {
    size_t   len;
    size_t   cap;
    ulib_u8i flag;
  } head;
  ulib_u8i data[];
};

#endif // ! ULIB__SSTR_H__
