#ifndef ULIB__MAP_H__
#define ULIB__MAP_H__

#include "../inc/util.h"
#include <stddef.h>

struct ordered_pair_t {
  ulib_size id;
  void     *key;
  void     *val;
};

struct ordered_map_t {
  struct {
    ulib_size size;
    ulib_size cap;
  } head;
  ulib_u8 container[];
};

#endif // !ULIB__MAP_H__
