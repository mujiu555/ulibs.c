#ifndef ULIB__MACROS_H__
#define ULIB__MACROS_H__

#include <stdbool.h>
#include <stddef.h>

#define LOOP for (;;)

#define RANGES(T, var, from, to, skip)                                         \
  for (T v##from##__from = (from), v##to##__to = (to), v##var##__keep = true;  \
       v##var##__keep; v##var##__keep = !v##var##__keep)                       \
    for (T var = v##from##__from;                                              \
         v##from##__from < v##to##__to ? var < v##to##__to                     \
                                       : var > v##to##__to;                    \
         var += skip)

#define FOREACH(T, var, arr, len)                                              \
  for (size_t v##var##__keep = true, v##var##__count = 0,                      \
              v##var##_len = (len);                                            \
       v##var##__keep && v##var##__count < v##var##_len;                       \
       v##var##__keep = !v##var##__keep, v##var##__count++)                    \
    for (T *var = (T *)(arr) + v##var##__count; v##var##__keep;                \
         v##var##__keep = !v##var##__keep)

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
