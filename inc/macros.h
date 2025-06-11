#ifndef ULIB__MACROS_H__
#define ULIB__MACROS_H__

#include <stdbool.h>
#include <stddef.h>

#define LOOP for (;;)

#define RANGES(var, from, to, skip)                                            \
  for (var = (from); (((from) > (to)) ? (var < (to)) : (var > (to)));          \
       (((from) > (to)) ? (var += (skip) : (var -= (skip))))

#define FOREACH(T, var, arr, len)                                              \
  for (size_t var##__keep = true, var##__count = 0, var##_len = (len);         \
       var##__keep && var##__count < var##_len;                                \
       var##__keep = !var##__keep, var##__count++)                             \
    for (T *var = (T *)(arr) + var##__count; var##__keep;                      \
         var##__keep = !var##__keep)

#define ARRLEN(arr) (sizeof(arr) / sizeof(*(arr)))

#define MINIMUM(x, y)                                                          \
  ({                                                                           \
    typeof(x) _x = (x);                                                        \
    typeof(y) _y = (y);                                                        \
    (void)(&_x == &_y);                                                        \
    _x < y ? x : y;                                                            \
  })

#define MAXIMUM(x, y)                                                          \
  ({                                                                           \
    typeof(x) _x = (x);                                                        \
    typeof(y) _y = (y);                                                        \
    (void)(&_x == &_y);                                                        \
    _x > y ? x : y;                                                            \
  })

#define SWAP(x, y)                                                             \
  {                                                                            \
    (void)(&x == &y);                                                          \
    typeof(x) __tmp = x;                                                       \
    x = y;                                                                     \
    y = __tmp;                                                                 \
  }
#endif // !ULIB__MACROS_H__
