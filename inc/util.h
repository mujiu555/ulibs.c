#ifndef ULIB__UTIL_H__
#define ULIB__UTIL_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <uchar.h>

typedef int8_t  ulib_i8;
typedef int16_t ulib_i16;
typedef int32_t ulib_i32;
typedef int64_t ulib_i64;

typedef uint8_t  ulib_u8;  // unsigned 8-bit integer
typedef uint16_t ulib_u16; // same...
typedef uint32_t ulib_u32;
typedef uint64_t ulib_u64;

typedef char8_t  ulib_char8;
typedef char16_t ulib_char16;
typedef char32_t ulib_char32;

typedef size_t ulib_size;

int ulib_cmp(const void *a, const void *b, void *len);

// TODO: reimplement qsort, with non-recursion method
extern void (*csort)(
  void *begin, void *end, size_t size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
);

extern void (*lsort)(
  void *obj, size_t begin, size_t end, size_t size,
  void *(*index)(const void *obj, size_t idx, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
);

extern void *(*cbsearch)(
  void *begin, void *end, void *target, size_t size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
);

extern void *(*lbsearch)(
  void *obj, void *target, size_t begin, size_t end,
  void *(*index)(const void *obj, size_t len, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
);

extern void *(*match)(
  void *str, void *pattern, size_t size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
);

#endif /* ifndef ULIB__UTIL_H__ */
