#include "mymalloc.h"
#include "stddef.h"

#define MEMLENGTH 4096
static union {
   char bytes[MEMLENGTH];
   double not_used;
} heap;