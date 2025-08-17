#ifndef ULIB__ARRAY_H__
#define ULIB__ARRAY_H__

#include "util.h"

struct array_t {
  struct {
    ulib_size size;
    ulib_size len;
    ulib_size cap;
    ulib_u8   flag;
  } head;
  ulib_u8 data[];
};

typedef struct array_t array_t;
typedef array_t       *array;

#endif // !ULIB__ARRAY_H__
