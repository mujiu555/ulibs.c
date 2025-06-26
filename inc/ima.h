#ifndef ULIB__IMMUTABLE_ARRAY_H__
#define ULIB__IMMUTABLE_ARRAY_H__

#include <stddef.h>
#include <stdlib.h>

#include "util.h"

struct ImmutableArray_t {
  struct {
    size_t   len; // total length of instances
    size_t   cap; // total instances the struct can store
    ulib_u8i size;
    ulib_u8i flag;
  } head;
  ulib_u8i data[];
};
typedef struct ImmutableArray_t        ima_t;
typedef struct ImmutableArray_t const *ima;

inline void const *ima_visit(ima obj, size_t idx) {
  return obj->data + obj->head.size * idx;
}

ima  ima_init(size_t len, size_t size, void const *args);
void ima_free(ima_t **obj);
ima  ima_clone(ima obj);

ima ima_concat(ima a, ima b);
ima ima_concat_autofree(ima *pa, ima *pb);
ima ima_slice(ima a, size_t skip, size_t len);
ima ima_remove(ima obj, size_t skip, size_t len);
ima ima_multiple(ima obj, size_t times);

ima ima_sort(ima obj, int (*cmp)(const void *a, const void *b, void *len));
ima ima_reverse(ima obj);

#endif /* ifndef ULIB__IMMUTABLE_ARRAY_H__ */
