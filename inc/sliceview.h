#ifndef ULIB__SLICEVIEW_H__
#define ULIB__SLICEVIEW_H__

#include <stddef.h>
#include "util.h"

struct view_t {
  struct {
    ulib_size len;
    ulib_size size;
  } head;
  void *begin;
  void *end;
};

typedef struct view_t view_t;

#endif // ! ULIB__SLICEVIEW_H__
