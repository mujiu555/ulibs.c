#ifndef ULIB__SLICEVIEW_H__
#define ULIB__SLICEVIEW_H__

#include <stddef.h>

struct view_t {
  struct {
    size_t len;
    size_t size;
  } head;
  void *begin;
  void *end;
};

typedef struct view_t view_t;

#endif // ! ULIB__SLICEVIEW_H__
