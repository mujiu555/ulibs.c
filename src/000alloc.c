#include "../inc/alloc.h"
#include <stdlib.h>

void *(*ulib_alloc)(size_t len) = malloc;
