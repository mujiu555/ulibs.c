#include "ept.h"
#include "util.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

int ulib_cmp(const void *a, const void *b, void *len) {
  ulib_size size = *(ulib_size *)len;
#ifdef LITTLE_ENDIAN
  int flag = ((int)*(int8_t *)a > 0) ? 1 : -1;
  int diff = (int)*(int8_t *)a - (int)*(int8_t *)b;
#elif defined(BIG_ENDIAN)
  int flag = ((int)*(int8_t *)(a + size - 1) > 0) ? 1 : -1;
  int diff = (int)*(int8_t *)(a + size - 1) - (int)*(int8_t *)(b + size - 1);
#endif
  if (diff > 0) {
    return 1;
  } else if (diff < 0) {
    return -1;
  }
#if defined(LITTLE_ENDIAN)
  for (ulib_size i = 1; i < size; i++) {
#elif defined(BIG_ENDIAN)
  for (int i = size - 2; i >= 0; i--) {
#endif
    diff = flag * ((int)*(ulib_u8 *)(a + i) - (int)*(ulib_u8 *)(b + i));
    if (diff > 0) {
      return 1;
    } else if (diff < 0) {
      return -1;
    }
  }
  return 0;
}

/**
 * @brief multiple byte swap, used only for util internally
 *
 * @param a pointer points to where need to be swaped
 * @param b pointer points to where need to be swaped
 * @param len bytes to be swaped
 */
static void util_internal_swap(void *a, void *b, ulib_size len) {
  if (ept_nullpointer_exception(a) || ept_nullpointer_exception(b)) {
    return;
  }
  if (a == b) {
    // No need to swap
    return;
  }
  ulib_u8 *tmp[len];
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
 * @param ctx
 */
static inline void *internal_continous_pivot(
  void *left, void *right, ulib_size size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  ulib_size len = (ptrdiff_t)(right - left) / size + 1;
  void     *mid = left + (len / 2) * size;
  if (cmp(left, mid, ctx) > 0) {
    util_internal_swap(left, mid, size);
  }
  if (cmp(left, right, ctx) > 0) {
    util_internal_swap(left, right, size);
  }
  if (cmp(mid, right, ctx) > 0) {
    util_internal_swap(mid, right, size);
  }
  return mid;
}

static inline ulib_size internal_linear_pivot(
  const void *obj, ulib_size left, ulib_size right, ulib_size size,
  void *(*index)(const void *obj, ulib_size index, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  ulib_size len = right - left;
  ulib_size mid = left + len / 2;

  if (cmp(index(obj, left, ctx), index(obj, mid, ctx), ctx) > 0) {
    util_internal_swap(index(obj, left, ctx), index(obj, mid, ctx), size);
  }
  if (cmp(index(obj, left, ctx), index(obj, right, ctx), ctx) > 0) {
    util_internal_swap(index(obj, left, ctx), index(obj, right, ctx), size);
  }
  if (cmp(index(obj, mid, ctx), index(obj, right, ctx), ctx) > 0) {
    util_internal_swap(index(obj, mid, ctx), index(obj, right, ctx), size);
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
 * @param ctx
 */
static void *internal_continous_partation(
  ulib_u8 **begin, ulib_u8 **end, ulib_size size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  void *const left  = *begin;
  void *const right = *end;

  ulib_u8 *l = left;
  ulib_u8 *r = right;

  // choose initial pivot position
  void *pivot = internal_continous_pivot(l, r, size, cmp, ctx);
  util_internal_swap(pivot, l, size);

  ulib_u8 base[size];
  memcpy(base, l, size);

  ulib_u8 *lp = left;
  ulib_u8 *rp = right;

  while (l < r) {
    // *right <= base
    // find number smaller than base and on right part
    while (l < r && cmp(r, &base, ctx) >= 0) {
      // move the variable same as the base to the outerest side
      if (!cmp(r, &base, ctx)) {
        util_internal_swap(rp, r, size);
        rp -= size;
      }
      r -= size;
    }
    memcpy(l, r, size); // *left = *right
    // *left >= base
    // find number bigger than base and on left part
    while (l < r && cmp(l, &base, ctx) <= 0) {
      if (!cmp(l, &base, ctx)) {
        util_internal_swap(lp, l, size);
        lp += size;
      }
      l += size;
    }
    memcpy(r, l, size); // *right = *left
  }
  memcpy(l, &base, size); // *left = *right

  // move back variables on the left-most and right-most position to pivot
  ulib_u8 *i = left, *j = l - size;
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

static ulib_size internal_linear_partation(
  void *obj, ulib_size *begin, ulib_size *end, ulib_size size,
  void *(*index)(const void *obj, ulib_size idx, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  ulib_size const left  = *begin;
  ulib_size const right = *end;

  ulib_size l = left;
  ulib_size r = right;

  // choose initial pivot position
  ulib_size pivot = internal_linear_pivot(obj, l, r, size, index, cmp, ctx);
  util_internal_swap(index(obj, pivot, ctx), index(obj, l, ctx), size);

  ulib_u8 base[size];
  memcpy(base, index(obj, l, ctx), size);

  ulib_size lp = left;
  ulib_size rp = right;

  while (l < r) {
    // *right <= base
    // find number smaller than base and on right part
    while (l < r && cmp(index(obj, r, ctx), &base, ctx) >= 0) {
      // move the variable same as the base to the outerest side
      if (!cmp(index(obj, r, ctx), &base, ctx)) {
        util_internal_swap(index(obj, rp, ctx), index(obj, r, ctx), size);
        rp--;
      }
      r--;
    }
    memcpy(index(obj, l, ctx), index(obj, r, ctx), size); // *left = *right
    // *left >= base
    // find number bigger than base and on left part
    while (l < r && cmp(index(obj, l, ctx), &base, ctx) <= 0) {
      if (!cmp(index(obj, l, ctx), &base, ctx)) {
        util_internal_swap(index(obj, lp, ctx), index(obj, l, ctx), size);
        lp++;
      }
      l++;
    }
    memcpy(index(obj, r, ctx), index(obj, l, ctx), size); // *right = *left
  }
  memcpy(index(obj, l, ctx), &base, size); // *left = *right

  // move back variables on the left-most and right-most position to pivot
  ulib_size i = left, j = l ? l - 1 : 0;
  while (i < j && i < lp) {
    util_internal_swap(index(obj, i, ctx), index(obj, j, ctx), size);
    i++;
    j--;
  }
  *begin = j;

  i = l + 1, j = right;
  while (i < j && j > rp) {
    util_internal_swap(index(obj, i, ctx), index(obj, j, ctx), size);
    i++;
    j--;
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
 * @param ctx
 */
static void internal_continous_qsort(
  void *left, void *right, ulib_size size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  if (ept_nullpointer_exception(left)             //
      || ept_nullpointer_exception(right)         //
      || ept_assert(right < left, EPT_OUTOFBOUND) //
      // size of a variable cannot be negative or zero
      || ept_assert(size <= 0, EPT_INVALIDED_VALUE) //
      || ept_nullpointer_exception(cmp)) {
    return;
  }

  if (left >= right) {
    return;
  }

  ulib_u8 *l = left;
  ulib_u8 *r = right;
  //void     *pivot = internal_continous_partation(&l, &r, size, cmp, ctx);
  internal_continous_partation(&l, &r, size, cmp, ctx);

  internal_continous_qsort(left, l, size, cmp, ctx);
  internal_continous_qsort(r, right, size, cmp, ctx);

  return;
}

static void internal_linear_qsort(
  void *obj, ulib_size left, ulib_size right, ulib_size size,
  void *(*index)(const void *obj, ulib_size idx, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  if (ept_assert(right < left, EPT_OUTOFBOUND) //
      || ept_nullpointer_exception(index)      //
      || ept_nullpointer_exception(cmp)) {
    return;
  }

  if (left >= right) {
    return;
  }

  ulib_size l = left;
  ulib_size r = right;
  // ulib_size pivot = internal_linear_partation(obj, &l, &r, size, index, cmp, ctx);
  internal_linear_partation(obj, &l, &r, size, index, cmp, ctx);

  internal_linear_qsort(obj, left, l, size, index, cmp, ctx);
  internal_linear_qsort(obj, r, right, size, index, cmp, ctx);

  return;
}

void *internal_continous_bsearch(
  void *begin, void *end, void *target, ulib_size size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  if (ept_assert(end < begin, EPT_OUTOFBOUND)) {
    return NULL;
  }
  while (begin <= end) {
    ulib_size len = (ptrdiff_t)(end - begin) / size + 1;
    ulib_u8  *mid = begin + len / 2 * size;

    const int r = cmp(mid, target, ctx);

    if (!r) {
      return mid;
    } else if (r > 0) {
      end = mid - size;
    } else {
      begin = mid + size;
    }
  }
  return NULL;
}

void *internal_linear_bsearch(
  void *obj, void *target, ulib_size begin, ulib_size end,
  void *(*index)(const void *obj, ulib_size, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) {
  if (ept_assert(begin > end, EPT_INVALIDED_VALUE)) {
    return NULL;
  }
  while (begin <= end) {
    ulib_size len = (end - begin) + 1;
    ulib_size mid = begin + len / 2;

    const int r = cmp(index(obj, mid, ctx), target, ctx);

    if (!r) {
      return index(obj, mid, ctx);
    } else if (r > 0) {
      end = mid - 1;
    } else {
      begin = mid + 1;
    }
  }
  return NULL;
}

void (*csort)(
  void *begin, void *end, ulib_size size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) = internal_continous_qsort;

void *(*ulib_continuous_bsearch)(
  void *begin, void *end, void *target, ulib_size size,
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) = internal_continous_bsearch;

void (*lsort)(
  void *obj, ulib_size begin, ulib_size end, ulib_size size,
  void *(*index)(const void *obj, ulib_size idx, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) = internal_linear_qsort;

void *(*lbsearch)(
  void *obj, void *target, ulib_size begin, ulib_size end,
  void *(*index)(const void *obj, ulib_size len, void *ctx),
  int (*cmp)(const void *a, const void *b, void *ctx), void *ctx
) = internal_linear_bsearch;
