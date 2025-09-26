#include "mymalloc.h"
#include "stddef.h"



#define MEMLENGTH 4096
static union {
   char bytes[MEMLENGTH];
   double not_used;
} heap;


typedef struct Block {
   size_t size;
   int free;
   struct Block* next;
} Block;
#define BLOCK_SIZE sizeof(Block)


static int initialized = 0;

static Block* LL = NULL;
void initialization() {
   LL = (Block*)heap.bytes;
   LL -> size = MEMLENGTH - BLOCK_SIZE;
   LL -> free = 1;
   LL -> next = NULL;
}


void* mymalloc(size_t byte, char* filename, char* lineNo) {
   if (!initialized) {
      initialization();
      initialized = 1;
   }
   byte = (byte + 7) & ~7;
   Block* current = LL;

   while (current) {
      if (current -> free && current -> size >= byte) {
         if (current -> size >= byte + BLOCK_SIZE + 1) {
            Block* new = (Block*)((char*)current + BLOCK_SIZE + byte);
            new -> size = current -> size - byte - BLOCK_SIZE;
            new -> free = 1;
            new -> next = current -> next;
            current -> next = new;
         }
         current -> free = 0;
         return (void*)(current + 1);
      }
      current = current -> next;
   }
   return NULL;
}