#ifndef ULIB__ALLOC_H__
#define ULIB__ALLOC_H__

#include <stddef.h>

extern void *(*ulib_alloc)(size_t len);

// TODO: implement a memory pool
// TODO: implement smart pointer, purely c

#endif // !ULIB__ALLOC_H__
