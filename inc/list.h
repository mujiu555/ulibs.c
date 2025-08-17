#ifndef ULIB__LIST_H__
#define ULIB__LIST_H__

#include "util.h"

struct linked_list_node_t;

struct linked_list_t {
  struct {
    ulib_size length;
    ulib_u8   flag;
  } head;
  struct linked_list_node_t *begin;
  struct linked_list_node_t *end;
  struct linked_list_node_t *cache;
};

typedef struct linked_list_t list_t;
typedef list_t              *list;

void *list_index(list obj, ulib_size idx);

list list_init();
void list_free(list *obj);
list list_clone(list obj);

// TODO:
list list_slice(list obj, ulib_size skip, ulib_size len);
list list_concat(list a, list b);
// NOTE: this will update cache!!!
list list_insert(list obj, void *data, ulib_size size, ulib_size nth);
// NOTE: this will update cache!!!
list list_remove(list obj, ulib_size skip, ulib_size len);

list list_sort(
  list obj, ulib_size size, int (*cmp)(const void *a, const void *b, void *len)
);
list list_reverse(list obj);

void *list_find(
  list obj, void *val, int (*cmp)(const void *a, const void *b, ulib_size len)
);
// TODO:
void *list_findall(list obj);

list list_replcae(list obj, list to, ulib_size skip, ulib_size len);

list list_replaceall(list obj, char pattern[]);

list list_regex(list obj, char pattern[]);

#endif // !ULIB__LIST_H__
