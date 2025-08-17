#ifndef ULIB__EPT_H__
#define ULIB__EPT_H__

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "util.h"

enum eptno_t {
  EPT_NORMAL,
  EPT_OPTIONAL,
  EPT_NULLPTR,
  EPT_OUTOFBOUND,
  EPT_INVALIDED_VALUE,
};

extern enum eptno_t eptno;

inline static enum eptno_t ept_nullpointer_exception(const void *p) {
  return (p == NULL) || errno ? (eptno = EPT_NULLPTR) : (eptno = EPT_NORMAL);
}

inline static enum eptno_t
ept_outofbound_exception(ulib_size boundary, size_t size) {
  return size >= boundary ? (eptno = EPT_OUTOFBOUND) : (eptno = EPT_NORMAL);
}

inline static enum eptno_t ept_assert(bool c, enum eptno_t VAL) {
  return c ? (eptno = VAL) : (eptno = EPT_NORMAL);
}

// TODO: implement basic error handling instructions, based on setjump & longjump
void ept_throw(void *);
void ept_backtrace();

#define _ept__try_
#define _ept__catch_

#define _ept__unused(x) (void)(x)

// TODO: maybe introduce `libunwind` otherwise

#endif // !ULIB__EPT_H__
