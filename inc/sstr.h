#ifndef ULIB__SSTR_H__
#define ULIB__SSTR_H__

#include "util.h"

// UTF-8
struct Sstr_t {
  struct {
    ulib_size len;
    ulib_size cap;
    ulib_u8   flag;
  } head;
  ulib_u8 data[];
};

typedef struct Sstr_t sstr_t;
typedef sstr_t       *sstr;

#endif // ! ULIB__SSTR_H__
