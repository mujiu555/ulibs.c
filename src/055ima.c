#include "../inc/alloc.h"
#include "../inc/ept.h"
#include "../inc/ima.h"
#include "../inc/macros.h"
#include "../inc/util.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

inline static ima_t *internal_alloc(ulib_size len, ulib_size size) {
  ima_t *o = ulib_alloc(sizeof(ima_t) + len * size);
  if (ept_nullpointer_exception(o)) {
    return o;
  }
  o->head.len  = len;
  o->head.cap  = len * size;
  o->head.size = size;
  return o;
}

ima ima_init(ulib_size len, ulib_size size, void const *args) {
  ima_t *obj = internal_alloc(len, size);
  memcpy(obj->data, args, len * size);
  return obj;
}

void ima_free(ima_t **obj) {
  if (ept_nullpointer_exception(*obj)) {
    return;
  }
  free(*obj);
  *obj = NULL;
}

ima ima_clone(ima obj) {
  return ima_init(obj->head.len, obj->head.size, obj->data);
}

ima ima_concat(ima a, ima b) {
  if (ept_assert(a->head.size != b->head.size, EPT_INVALIDED_VALUE)) {
    return NULL;
  }
  ima_t *obj = internal_alloc(a->head.len + b->head.len, a->head.size);
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  memcpy(obj->data, a->data, a->head.cap);
  memcpy(obj->data + a->head.cap, b->data, b->head.cap);
  return obj;
}

ima ima_concat_autofree(ima *pa, ima *pb) {
  ima a = *pa;
  ima b = *pb;
  if (ept_assert(a->head.size != b->head.size, EPT_INVALIDED_VALUE)) {
    return NULL;
  }
  ima_t *obj = internal_alloc(a->head.len + b->head.len, a->head.size);
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  memcpy(obj->data, a->data, a->head.cap);
  memcpy(obj->data + a->head.cap, b->data, b->head.cap);
  ima_free((ima_t **)pa);
  ima_free((ima_t **)pb);
  return obj;
}

ima ima_slice(ima obj, ulib_size skip, ulib_size len) {
  if (ept_outofbound_exception(obj->head.len, skip + len)) {
    return NULL;
  }
  ima n = ima_init(len, obj->head.size, obj->data + skip);
  if (ept_nullpointer_exception(n)) {
    return n;
  }
  return n;
}

ima ima_remove(ima obj, ulib_size skip, ulib_size len) {
  if (ept_outofbound_exception(obj->head.len, skip + len)) {
    return NULL;
  }
  ima_t *n = internal_alloc(obj->head.len - len, obj->head.size);
  if (ept_nullpointer_exception(n)) {
    return n;
  }
  // 0 -- skip
  memcpy(n->data, obj->data, skip * obj->head.size);
  // skip + len -- len
  memcpy(
    n->data + skip * obj->head.size,
    obj->data + (skip + len) * obj->head.size,
    (obj->head.len - skip - len) * obj->head.size
  );
  return n;
}

ima ima_multiple(ima obj, ulib_size times) {
  ima_t *n = internal_alloc(obj->head.len * times, obj->head.size);
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  RANGES(ulib_size, i, 0, times, 1) {
    memcpy(n->data + i * obj->head.cap, obj->data, obj->head.cap);
  }
  return n;
}

ima ima_reverse(ima obj) {
  ima_t *no = internal_alloc(obj->head.len, obj->head.size);
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  for (ulib_size i = 0; i < obj->head.cap; i += obj->head.size) {
    memcpy(
      no->data + i,
      obj->data + (obj->head.cap - i - obj->head.size),
      obj->head.size
    );
  }
  return no;
}

ima ima_sort(ima obj, int (*cmp)(const void *a, const void *b, void *len)) {
  ima_t *no = internal_alloc(obj->head.len, obj->head.size);
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  memcpy(no->data, obj->data, obj->head.cap);
  csort(
    no->data,
    no->data + (no->head.len - 1) * no->head.size,
    no->head.size,
    cmp,
    &no->head.size
  );
  return no;
}

void const *ima_find(
  ima obj, void *val, int (*cmp)(const void *a, const void *b, ulib_size len)
) {
  for (ulib_size i = 0; i < obj->head.len; i++) {
    void const *p = obj->data + i * obj->head.size;
    if (!cmp(p, val, obj->head.size)) {
      return p;
    }
  }
  return NULL;
}

void const *ima_findall(
  ima obj, char pattern[],
  int (*cmp)(const void *a, const void *b, ulib_size len)
) {
  // TODO:
  return NULL;
}

ima ima_replace(ima obj, ima to, ulib_size skip, ulib_size len) {
  if (ept_assert(obj->head.size != to->head.size, EPT_INVALIDED_VALUE)) {
    return NULL;
  }
  ima_t *no =
    internal_alloc(obj->head.size - len + to->head.len, obj->head.size);
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  memcpy(no->data, obj->data, skip * obj->head.size);
  memcpy(no->data + obj->head.size * skip, to->data, to->head.cap);
  memcpy(
    no->data + obj->head.size * skip + to->head.cap,
    obj->data + obj->head.size * (skip + len),
    obj->head.size * (obj->head.len - skip - len)
  );
  return NULL;
}

ima ima_replaceall(
  ima obj, ima pattern, ima to,
  int (*cmp)(const void *a, const void *b, ulib_size len)
) {
  // TODO:
  return NULL;
}

ima ima_regex(ima obj, char regexp[]) {
  // TODO:
  return NULL;
}
