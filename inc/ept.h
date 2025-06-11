#ifndef ULIB__EPT_H__
#define ULIB__EPT_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

enum eptno_t {
  EPT_NORMAL,
  EPT_OPTIONAL,
  EPT_NULLPTR,
  EPT_OUTOFBOUND,
  EPT_INVALIDED_VALUE,
};

extern enum eptno_t eptno;

inline enum eptno_t ept_nullpointer_exception(const void *p) {
  return (p == NULL) || errno ? (eptno = EPT_NULLPTR) : (eptno = EPT_NORMAL);
}

inline enum eptno_t ept_outofbound_exception(size_t boundary, size_t size) {
  return size >= boundary ? (eptno = EPT_OUTOFBOUND) : (eptno = EPT_NORMAL);
}

inline enum eptno_t ept_assert(bool c, enum eptno_t VAL) {
  return c ? (eptno = VAL) : (eptno = EPT_NORMAL);
}
#endif // !ULIB__EPT_H__
