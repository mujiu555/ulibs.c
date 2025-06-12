#ifndef ULIB__UTIL_H__
#define ULIB__UTIL_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t ulib_u8i; // unsigned 8-bit integer
typedef uint16_t ulib_u16i;
typedef uint32_t ulib_u32i;

int ulib_cmp(const void *a, const void *b, void *len);

// TODO: reimplement qsort
extern void (*ulib_qsort)(void *left, void *right, size_t size,
                          int (*cmp)(const void *a, const void *b, void *ctx),
                          void *ctx);

// TODO: reimplement msort
extern void (*ulib_msort)(void *arr, size_t len, size_t size,
                          int (*cmp)(const void *a, const void *b, void *ctx),
                          void *ctx);

#endif /* ifndef ULIB__UTIL_H__ */
