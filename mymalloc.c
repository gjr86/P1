#include <stdio.h>
#include <stddef.h>

#include "mymalloc.h"


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

void myfree(void *ptr, char *file, int line) {
   if (!initialized) { 
      initialization(); 
      initialized = 1; 
   } 
   if (ptr == NULL) {
      printf("myfree(): ptr is NULL"); 
      return; 
   }
   // get block header 
   Block *block = (Block *)ptr - 1; 

   if (block->free) { 
      printf("myfree(): free at %s:%d\n", file, line); 
      return; 
   } 

   block->free = 1; 

   // coalesce forward 
   while (block->next && block->next->free) { 
      block->size += sizeof(Block) + block->next->size; 
      block->next = block->next->next; 
   } 

   // coalesce backward 
   Block *current = LL; 
   while (current && current->next != block) { 
      current = current->next; 
   } 
   if (current && current->free) { 
      current->size += sizeof(Block) + block->size; 
      current->next = block->next; 
   } 
}