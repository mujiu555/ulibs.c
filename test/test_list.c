#include "macros.h"
#include "ept.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void plist(list obj) {
  for (size_t i = 0; i < obj->head.length; i++) {
    printf("%4d ", *(int *)list_index(obj, i));
  }
  puts("");
}

int main(int argc, char *argv[]) {
  _ept__unused(argc);
  _ept__unused(argv);
  int a[] = {21, 32, 13, 7, 1, 21, 3, 2, 6, 9, 5, 26, 13};

  list l = list_init();
  FOREACH(int, i, a, ARRLEN(a)) { list_insert(l, i, sizeof(int), 0); }
  plist(l);

  plist(list_remove(l, 0, 1));
  plist(list_remove(l, 3, 2));
  plist(list_remove(l, 7, 1));

  list_sort(l, sizeof(int), ulib_cmp);
  plist(l);

  list_reverse(l);
  plist(l);

  return EXIT_SUCCESS;
}
