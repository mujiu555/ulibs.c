#include "../inc/ept.h"
#include "../inc/util.h"
#include <stddef.h>
#include <string.h>

/**
 * @brief multiple byte swap, used only for util internally
 *
 * @param a pointer points to where need to be swaped
 * @param b pointer points to where need to be swaped
 * @param len bytes to be swaped
 */
static void util_internal_swap(void *a, void *b, size_t len) {
  if (ept_nullpointer_exception(a) || ept_nullpointer_exception(b)) {
    return;
  }
  if (a == b) {
    // No need to swap
    return;
  }
  ulib_u8i *tmp[len];
  // NOTE: I don't know whether there is problem between big-ending and
  // little-ending
  // aka. potential alignment problem
  memcpy(tmp, a, len);
  memcpy(a, b, len);
  memcpy(b, tmp, len);
  return;
}

/**
 * @brief a method to choose initial pivot for quick sort
 *
 * @param left pointer points to left most of array
 * @param right pointer points to right most of array
 * @param size element size
 * @param cmp comparation method
 * @param a pointer points to variable to be compared
 * @param b pointer points to variable to be compared
 * @param len bytes to be compared
 */
static inline void *internal_pivot(void *left, void *right, size_t size,
                                   int (*cmp)(const void *a, const void *b,
                                              size_t len)) {

  size_t len = (ptrdiff_t)(right - left) / size;
  void *mid = left + (len / 2) * size;
  if (cmp(left, mid, size) > 0) {
    util_internal_swap(left, mid, size);
  }
  if (cmp(left, right, size) > 0) {
    util_internal_swap(left, right, size);
  }
  if (cmp(mid, right, size) > 0) {
    util_internal_swap(mid, right, size);
  }
  return mid;
}

/**
 * @brief lagency partation method for quick sort
 *
 * @param begin pointer points to the pointer points to left most of array,
 * after executation of the method, the pointer will points to the right most of
 * left partation
 * @param end pointer poinsts to the pointer points to right most of array,
 * after executation of the method, the pointer will points to the left most of
 * right partation
 * @param size bytes of elements in the array
 * @param cmp comparation function
 * @param a pointer points to variable to be compared
 * @param b pointer points to variable to be compared
 * @param len bytes to be compared
 */
static void *internal_partation(ulib_u8i **begin, ulib_u8i **end, size_t size,
                                int (*cmp)(const void *a, const void *b,
                                           size_t len)) {
  void *const left = *begin;
  void *const right = *end;

  ulib_u8i *l = left;
  ulib_u8i *r = right;

  // choose initial pivot position
  void *pivot = internal_pivot(l, r, size, cmp);
  util_internal_swap(pivot, l, size);

  ulib_u8i base[size];
  memcpy(base, l, size);

  ulib_u8i *lp = left;
  ulib_u8i *rp = right;

  while (l < r) {
    // *right <= base
    // find number smaller than base and on right part
    while (l < r && cmp(r, &base, size) >= 0) {
      // move the variable same as the base to the outerest side
      if (!cmp(r, &base, size)) {
        util_internal_swap(rp, r, size);
        rp -= size;
      }
      r -= size;
    }
    memcpy(l, r, size); // *left = *right
    // *left >= base
    // find number bigger than base and on left part
    while (l < r && cmp(l, &base, size) <= 0) {
      if (!cmp(l, &base, size)) {
        util_internal_swap(lp, l, size);
        lp += size;
      }
      l += size;
    }
    memcpy(r, l, size); // *right = *left
  }
  memcpy(l, &base, size); // *left = *right

  // move back variables on the left-most and right-most position to pivot
  ulib_u8i *i = left, *j = l - size;
  while (i < j && i < lp) {
    util_internal_swap(i, j, size);
    i += size;
    j -= size;
  }
  *begin = j;

  i = l + size, j = right;
  while (i < j && j > rp) {
    util_internal_swap(i, j, size);
    i += size;
    j -= size;
  }
  *end = i;

  return l;
}

/**
 * @brief lagency quick sort method using recursive
 *
 * @param left pointer points to left most of array
 * @param right pointer points to right most of array
 * @param size bytes of elements in the array
 * @param cmp comparation method
 * @param a pointer points to variable to be compared
 * @param b pointer points to variable to be compared
 * @param len bytes to be compared
 */
static void internal_qsort(void *left, void *right, size_t size,
                           int (*cmp)(const void *a, const void *b,
                                      size_t len)) {
  if (ept_nullpointer_exception(left) || ept_nullpointer_exception(right) ||
      ept_assert(right < left, EPT_OUTOFBOUND) ||
      // size of a variable cannot be negative or zero
      ept_assert(size <= 0, EPT_INVALIDED_VALUE) ||
      ept_nullpointer_exception(cmp)) {
    return;
  }

  if (left >= right) {
    return;
  }

  ulib_u8i *l = left;
  ulib_u8i *r = right;
  void *pivot = internal_partation(&l, &r, size, cmp);

  internal_qsort(left, l, size, cmp);
  internal_qsort(r, right, size, cmp);

  return;
}

void internal_msort(void *arr, size_t len, size_t size,
                    int (*cmp)(const void *a, const void *b, size_t len)) {
  return;
}

void (*ulib_qsort)(void *arr, void *right, size_t size,
                   int (*cmp)(const void *a, const void *b,
                              size_t len)) = internal_qsort;

void (*ulib_msort)(void *arr, size_t len, size_t size,
                   int (*cmp)(const void *a, const void *b,
                              size_t len)) = internal_msort;
