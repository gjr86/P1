#include <stdio.h>
#include <stddef.h>

#include "mymalloc.h"


#define MEMLENGTH 4096
static union {
   char bytes[MEMLENGTH];
   double not_used;
} heap;


//Block structure for linked list
typedef struct Block {
   size_t size; //Size of the block in bytes
   int free; //0 if used, anything else if free (likely 1)
   struct Block* next; //Pointer to another block for linked list structure
} Block;
#define BLOCK_SIZE sizeof(Block) //Size of the block structure in bytes


static int initialized = 0; //Static int to check if initialized due to not being able to run before runtime

static Block* LL = NULL; //Linted List head storage

void checkInitalization() {
   if (!initialized) { //Uses a static variable to ensure initialization before first use
      LL = (Block*)heap.bytes; //Set head of linked list to start of heap
      LL -> size = MEMLENGTH - BLOCK_SIZE; //Size of head is total memory minus block size
      LL -> free = 1; //Mark head as free
      LL -> next = NULL; //Marks head.next as NULL since it's the only block
      initialized = 1; //Set initialized to true so this doesn't run again
      atexit(leakDetector); //Register leak detector to run at program exit
   }
}

void leakDetector() { // Detects memory leaks at program termination (2.2 Leak Detection)
    int count = 0; //How many blocks were never freed
    size_t bytes = 0; //Total size of all blocks that were never freed
    Block* current = LL; //Pointer to head of linked list

    while (current) { //For all blocks in the linked list
        if (!current->free) { //If the block is not free (i.e., it was allocated and not freed)
            count++;
            bytes += current->size;
        }
        current = current->next; // Move to the next block in the list
    }

    if (count > 0) { //Prints error if there was a memory leak
        fprintf(stderr, "mymalloc: %d bytes leaked in %d objects.\n", (int)bytes, count);  // Print leak summary using standard output
    }
}


void *mymalloc(size_t byte, char* filename, int lineNo) {
   checkInitalization();

   byte = (byte + 7) & ~7; // Align to 8 bytes
   Block* current = LL; //Pointer to the head of the linked list

   while (current) { //For all blocks
      if (current -> free && current -> size >= byte) { //Check if block is free and large enough for request
         if (current -> size >= byte + BLOCK_SIZE + 8) { //If the block is large enough to split, do so
            Block* new = (Block*)((char*)current + BLOCK_SIZE + byte); //Create new block after allocated space
            new -> size = current -> size - byte - BLOCK_SIZE; //Set new block size to remaining space
            new -> free = 1; //Mark new block as free
            new -> next = current -> next; //Set new block next to current next
            current -> next = new; //Set current next to new block
            printf("mymalloc: Allocated %d bytes at %p from %s:%d\n", (int)byte, (void*)(current + 1), filename, lineNo); //Allocation success message
         }
         current -> free = 0; //Mark block as used
         return (void*)(current + 1); //Return pointer to memory after the block header
      }
      current = current -> next; //Move to next block
   }
   fprintf(stderr, "malloc: Unable to allocate %d bytes (%s:%d)\n", (int)byte, filename, lineNo); //Allocation failed error check (2.1 Detectable Errors)
   return NULL;
}


void illegalFree(char* file, int line) { //Handles inappropriate free calls (2.1 Detectable Errors)
   fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
}

void myfree(void *ptr, char *file, int line) {
   checkInitalization();
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