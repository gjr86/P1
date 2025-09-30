#include "mymalloc.h"
#include "stddef.h"

#define MEMLENGTH 4096
static union {
   char bytes[MEMLENGTH];
   double not_used;
} heap;



void* mymalloc(size_t size, char *file, int line) {

   return NULL;
}

void myfree(void *ptr, char *file, int line) {
    /*
    mark the block pointed to by ptr as free
    here we will coalesce free blocks if possible
    */
   return;
}