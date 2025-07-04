#ifndef ULIB__MAP_H__
#define ULIB__MAP_H__

#include "../inc/util.h"
#include <stddef.h>

struct ordered_pair_t {
  size_t id;
  void *key;
  void *val;
};

struct ordered_map_t {
  struct {
    size_t size;
    size_t cap;
  } head;
  ulib_u8i container[];
};

#endif // !ULIB__MAP_H__
