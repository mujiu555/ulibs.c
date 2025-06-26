#include "list.h"
#include "util.h"
#include "alloc.h"
#include "ept.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct linked_list_node_t {
  struct {
    ulib_u8i size;
    size_t   nth;
  } head;
  struct linked_list_node_t *next;
  struct linked_list_node_t *prev;
  ulib_u8i                   data[];
};

typedef struct linked_list_node_t node_t;

static inline void internal_node_add(node_t *prev, node_t *next, node_t *new) {
  next->prev = new;
  new->next  = next;
  new->prev  = prev;
  prev->next = new;
}

static inline void internal_node_del(node_t *begin, node_t *end) {
  node_t *prev = begin->prev;
  node_t *next = end->prev;
  prev->next   = end;
  end->prev    = prev;

  begin->prev = NULL;
  next->next  = NULL;
}

/**
 * @brief get the target element within a double-linked list, according to the index,
 * if not found, return null, otherwise, return the pointer points to data
 *
 * NOTE: This method will update the cache
 * 
 * First find the node near the target index,
 * then travsel the list until meet target index.
 *
 * @param obj double-linked list 
 * @param idx index of desired element
 * @return poinster points to the data
 */
void *list_index(list obj, size_t idx) {
  if (ept_nullpointer_exception(obj) ||
      ept_outofbound_exception(obj->head.length, idx)) {
    return NULL;
  }
  // TODO: More effective indexing concerning obj->end
  for (node_t *n = obj->cache && (obj->cache->head.nth < idx) ? obj->cache
                                                              : obj->begin;
       n != NULL;
       n = n->next) {
    if (n->head.nth == idx) {
      obj->cache = n;
      return n->data;
    }
  }
  return NULL;
}

/**
 * @brief Allocate a new node
 *
 * @param size size of data stored witin the node
 * @return a new node
 */
static node_t *internal_node_alloc(size_t size) {
  node_t *obj = ulib_alloc(sizeof(node_t) + size);
  if (ept_nullpointer_exception(obj)) {
    return obj;
  }
  obj->head.size = size;
  return obj;
}

/**
 * @brief initialize a new node
 *
 * @param size size of stored data
 * @param nth the index of current node within the linked list
 * @param prev pointer points to previous node
 * @param data data stored
 * @return a new node with data
 */
static node_t *
internal_node_init(size_t size, size_t nth, node_t *prev, void *data) {
  if (ept_nullpointer_exception(data)) {
    return NULL;
  }
  node_t *obj   = internal_node_alloc(size);
  obj->head.nth = nth;
  obj->next     = NULL;
  obj->prev     = prev;
  memcpy(obj->data, data, size);
  return obj;
}

/**
 * @brief allocate a new list structure
 *
 * @return a new list
 */
list internal_list_alloc() {
  list obj = ulib_alloc(sizeof(list_t));
  return obj;
}

/**
 * @brief initialize a new list
 *
 * NOTE: the method will not insert initial node by default
 *
 * @return a new list
 */
list list_init() {
  list obj         = internal_list_alloc();
  obj->head.length = 0;
  obj->head.flag   = 0x0;
  obj->begin       = NULL;
  obj->end         = NULL;
  obj->cache       = obj->begin;
  return obj;
}

/**
 * @brief clean up a list structure
 *
 * 1. Check the object is not null already;
 * 2. get nodes and free them all
 * 3. free list structure last.
 * 4. assign list pointer to null, which means the list is null
 *
 * to avoide dingling pointer, in some way...
 *
 * @param obj pointer points to object to be free.
 */
void list_free(list *obj) {
  if (ept_nullpointer_exception(*obj)) {
    return;
  }
  for (node_t *n = (*obj)->begin; n != NULL;) {
    node_t *p = n;
    n         = n->next;
    free(p);
  }
  free(*obj);
  *obj = NULL;
  return;
}

/**
 * @brief create a new list based on the object provided
 * copy constructing like method
 *
 * 1. initialize the list structure
 * 2. fetch & assign begin node of the object to new object
 * 3. travsel following nodes and then clone them to new object
 * 4. update new object's length
 *
 * @param obj object to be clone
 * @return new list whose elements' value are same with original object
 */
list list_clone(list obj) {
  list no   = list_init();
  no->cache = no->begin =
    internal_node_init(obj->begin->head.size, 0, NULL, obj->begin->data);
  for (node_t *n = obj->begin->next; n != NULL && no->cache != NULL;
       no->cache = no->cache->next, n = n->next) {
    no->cache->next =
      internal_node_init(n->head.size, n->head.nth, no->cache, n->data);
  }
  no->head.length = obj->head.length;
  no->end         = no->cache;
  return no;
}

list list_slice(list obj, size_t skip, size_t len) {
  list    no  = internal_list_alloc();
  node_t *n   = obj->begin;
  int     cnt = 0;
  while (n->next != NULL && n->head.nth < skip) {
    n = n->next;
  }
  while (n->next != NULL && n->head.nth < skip + len) {
    list_insert(no, n->data, n->head.size, cnt++);
    n = n->next;
  }
  return no;
}

list list_slice_autofree(list *obj, size_t skip, size_t len) {
  list no = list_slice(*obj, skip, len);
  list_free(obj);
  return no;
}

list list_concat(list a, list b) {
  list    obj = internal_list_alloc();
  node_t *n   = a->begin;
  int     cnt = 0;
  while (n != NULL) {
    list_insert(obj, n->data, n->head.size, cnt++);
    n = n->next;
  }
  n = b->begin;
  while (n != NULL) {
    list_insert(obj, n->data, n->head.size, cnt++);
    n = n->next;
  }
  return obj;
}

list list_concat_autofree(list *a, list *b) {
  (*a)->end->next   = (*b)->begin;
  (*b)->begin->prev = (*a)->end;
  (*a)->end         = (*b)->end;
  (*b)->begin       = NULL;
  (*b)->end         = NULL;
  (*b)->cache       = NULL;
  list_free(b);
  list l = *a;
  *a     = NULL;
  return l;
}

/**
 * @brief insert a new element into list 
 *
 * NOTE: this method will update cache
 *
 * 1. check if it is first insert, (when list have no element inside),
 * if so, insert element as beginning and ending.
 * 2. check if inserted into the beginning, 
 * if so, update beginning to the inserted node,
 * assign prev of original node as new one,
 * and set new one's next to original one
 * cache points to the new one.
 * 3. check if inserted into the ending,
 * if so, update ending to the inserted node,
 * assign next of original node as new one,
 * and set now one's prev to original one
 * cache points to the new one.
 * 4. otherwise,
 * find the nth-th. node, assign its next to new node
 * and assign new node's next to its original next
 * so as the prev of new node.
 * (assign original next's prev to new node and set new node's prev as nth one)
 * cache points to the new one.
 *
 * 5. update nth of each node after new one
 * 6. update length of list
 *
 * @param obj list object
 * @param data data to be added into list
 * @param size size of data
 * @param nth the index of current element
 * @return list itself
 */
list list_insert(list obj, void *data, size_t size, size_t nth) {
  if (ept_outofbound_exception(obj->head.length + 1, nth)) {
    return NULL;
  }
  node_t *n = NULL;
  if (obj->head.length == 0) {
    obj->begin = obj->end = obj->cache =
      internal_node_init(size, nth, NULL, data);
  } else if (nth == 0) {
    n                = obj->begin;
    obj->begin       = internal_node_init(size, nth, NULL, data);
    obj->begin->next = n;
    n->prev          = obj->begin;
    obj->cache       = obj->begin;
  } else if (nth == obj->head.length) {
    n          = obj->end;
    n->next    = internal_node_init(size, nth, n, data);
    obj->end   = n->next;
    obj->cache = obj->end;
  } else {
    n = obj->cache && obj->cache->head.nth < nth ? obj->cache : obj->begin;
    // after execution of following expression, the n->head.nth will always be nth
    while (n->next != NULL && n->head.nth < nth) {
      n = n->next;
    }
    // obj->cache       = n->next;
    // n->next          = internal_node_init(size, nth, n, data);
    // obj->cache->prev = n->next;
    // n->next->next    = obj->cache;
    // obj->cache       = n->next;
    obj->cache = n->next;
    internal_node_add(n, obj->cache, internal_node_init(size, nth, n, data));
  }
  n = obj->cache;
  // NOTE: maybe update only after cache updated
  while (n->next != NULL) {
    n = n->next;
    n->head.nth++;
  }

  obj->head.length++;

  return obj;
}

/**
 * @brief remove some sequencial nodes within the list
 *
 * NOTE: this method will update cache
 *
 * 1. check if remove nodes from beginning to the ending,
 * if so, l points to begin, r points to end
 * clear begin and end
 *   |         to be removed                                                      |
 *   | begin | 1st. | 2nd. | 3rd. | ... | ...                               | end |
 * 2. check if remove from beginning,
 * if so, l points to begin, 
 * find r at the end of removing nodes (the node just not to be remove after those who will be removed)
 * begin points to r
 * r points to the last to be removed one
 * begin->prev assign to null
 *   |         to be removed                   | rest                                 |
 *   | begin | 1st. | 2nd. | 3rd. | ... | nth. | r     | ... |                  | end |
 *   =>
 *   |         to be removed                   |       |
 *   | l    | 1st. | 2nd. | 3rd. | ... | r     | begin | ... |                  | end |
 * n points to the one just after removement
 * 3. check if remove to the ending (one),
 * r points to the end
 * l points to the beginning of removed nodes
 * update l.prev to end
 * end.next = null
 * l.prev = null
 * n points to the one just after removement
 * 4. otherwise,
 * l points to the beginning of removed nodes,
 * r points to the first one who has no need to be removed
 * l.prev.next = r
 * r.prev.next = null
 * r.prev = l.prev
 * l.prev = null
 * n points to the one just after removement
 *
 * 5. free nodes from l to r
 * 6. update nodes from and after n
 * 7. update list length
 *
 *
 * @param obj list object
 * @param skip from which the node is to be removed
 * @param len remove how much nodes
 * @return list itself
 */
list list_remove(list obj, size_t skip, size_t len) {
  if (ept_outofbound_exception(obj->head.length, skip)) {
    return NULL;
  }
  if (ept_outofbound_exception(obj->head.length + 1, skip + len)) {
    return NULL;
  }
  node_t *l = NULL;
  node_t *r = l;
  node_t *n = NULL;
  if (skip == 0 && skip + len == obj->head.length) {
    // Free all
    l          = obj->begin;
    r          = obj->end;
    obj->cache = NULL;
    obj->begin = NULL;
    obj->end   = NULL;
  } else if (skip == 0) {
    // free from beginning
    l = obj->begin;
    r = l;
    // find the end
    while (r->next != NULL && r->head.nth < skip + len) {
      r = r->next;
    }

    obj->begin = r;
    n          = r;
    r          = r->prev;

    r->next = NULL;
    n->prev = NULL;

    obj->cache = n;
  } else if (skip + len == obj->head.length) {
    // free to the end
    r = obj->end;
    l = r;
    while (l->prev != NULL && l->head.nth > skip) {
      l = l->prev;
    }

    n          = l->prev;
    obj->end   = n;
    n->next    = NULL;
    l->prev    = NULL;
    obj->cache = n;

    n = n->next;

  } else {
    // whitin
    l = obj->cache && obj->cache->head.nth < len ? obj->cache : obj->begin;
    while (l->next != NULL && l->head.nth < skip) {
      l = l->next;
    }
    r = l;
    while (r->next != NULL && r->head.nth < skip + len) {
      r = r->next;
    }

    n = r;
    internal_node_del(l, r);
    obj->cache = n;

    //// l.prev.next = r
    //n       = l->prev;
    //n->next = r;
    //// l.prev = null
    //l->prev    = NULL;
    //obj->cache = r->prev;
    //// r.prev = l.prev
    //r->prev = n;
    //r       = obj->cache;
    //// r.prev.next = null
    //r->next = NULL;
    //
    //n          = n->next;
    //obj->cache = n;
  }
  while (n != NULL) {
    n->head.nth -= len;
    n = n->next;
  }
  while (l != NULL) {
    n = l;
    l = l->next;
    free(n);
  }
  obj->head.length -= len;
  return obj;
}

/**
 * @brief index element within the list
 *
 * @param obj list
 * @param idx index
 * @param _ no used, for compatible with the interface of lsort
 */
static void *internal_index_for_sort(const void *obj, size_t idx, void *ctx) {
  _ept__unused(ctx);
  return list_index((list)obj, idx);
}

list list_sort(
  list obj, size_t size, int (*cmp)(const void *a, const void *b, void *len)
) {
  lsort(
    obj, 0, obj->head.length - 1, size, internal_index_for_sort, cmp, &size
  );
  return obj;
}

list list_reverse(list obj) {
  obj->cache  = obj->begin;
  obj->begin  = obj->end;
  obj->end    = obj->cache;
  node_t *n   = obj->begin;
  int     cnt = 0;
  while (n != NULL) {
    obj->cache  = n->next;
    n->next     = n->prev;
    n->prev     = obj->cache;
    n->head.nth = cnt++;
    n           = n->next;
  }
  obj->cache = obj->begin;
  return obj;
}

void *list_find(
  list obj, void *val, int (*cmp)(const void *a, const void *b, size_t len)
) {
  for (node_t *n = obj->begin; n != NULL; n = n->next) {
    if (!cmp(n->data, val, n->head.size)) {
      return n->data;
    }
  }
  return NULL;
}
// TODO:
void *list_findall(list obj);

list list_replcae(list obj, list to, size_t skip, size_t len);

list list_replaceall(list obj, char pattern[]);

list list_regex(list obj, char pattern[]);
