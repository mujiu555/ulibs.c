#ifndef ULIB__LIST_H__
#define ULIB__LIST_H__

#include "util.h"

struct linked_list_node_t;

struct linked_list_t {
  struct {
    size_t   length;
    ulib_u8i flag;
  } head;
  struct linked_list_node_t *begin;
  struct linked_list_node_t *end;
  struct linked_list_node_t *cache;
};

typedef struct linked_list_t list_t;
typedef list_t              *list;

void *list_index(list obj, size_t idx);

list list_init();
void list_free(list *obj);
list list_clone(list obj);

// TODO:
list list_slice(list obj, size_t skip, size_t len);
list list_concat(list a, list b);
// NOTE: this will update cache!!!
list list_insert(list obj, void *data, size_t size, size_t nth);
// NOTE: this will update cache!!!
list list_remove(list obj, size_t skip, size_t len);

list list_sort(
  list obj, size_t size, int (*cmp)(const void *a, const void *b, void *len)
);
list list_reverse(list obj);

void *list_find(
  list obj, void *val, int (*cmp)(const void *a, const void *b, size_t len)
);
// TODO:
void *list_findall(list obj);

list list_replcae(list obj, list to, size_t skip, size_t len);

list list_replaceall(list obj, char pattern[]);

list list_regex(list obj, char pattern[]);

#endif // !ULIB__LIST_H__
