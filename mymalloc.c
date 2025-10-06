#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "mymalloc.h"


//Heap structure for memory allocation
#define MEMLENGTH 4096
static union {
   char bytes[MEMLENGTH];
   double not_used;
} heap;


//Block structure
typedef struct Block {
   size_t size;
} Block;


//Shorthand for alignment and size of block structure
#define BLOCK_SIZE sizeof(Block) //Size of the block structure in bytes

//Shorthand for manipulating block metadata
#define IS_FREE(b)      ((b)->size & 1) //Checks if block b is free
#define SET_FREE(b)     ((b)->size |= 1) //Marks block b as free
#define SET_USED(b)     ((b)->size &= ~1UL) //Marks block b as used
#define GET_SIZE(b)     ((b)->size & ~1UL) //Gets size of block b
#define SET_SIZE(b, s)  ((b)->size = (s) | (IS_FREE(b))) //Sets size of block b to s while preserving free status


static int initialized = 0; //Static int to check if initialized due to not being able to run before runtime

static Block* mem = NULL; //Pointer for head of memory


void leakDetector() { // Detects memory leaks at program termination (2.2 Leak Detection)
    int count = 0; //How many blocks were never freed
    size_t bytes = 0; //Total size of all blocks that were never freed
    Block* current = mem; //Pointer to head

    while ((char*)current < (char*)mem + MEMLENGTH) { //For all blocks in the heap
        if (!IS_FREE(current)) { //If the block is not free
            count++;
            bytes += GET_SIZE(current);
        }
        current = (Block *)((char *)current + BLOCK_SIZE + GET_SIZE(current)); // Move to the next block in the list
    }

    if (count > 0) { //Prints error if there was a memory leak
        fprintf(stderr, "mymalloc: %d bytes leaked in %d objects.\n", (int)bytes, count);  // Print leak summary using standard output
    }
}

void checkInitalization() {
   if (!initialized) { //Uses a static variable to ensure initialization before first use
      mem = (Block*)heap.bytes; //Set head of linked list to start of heap
      mem -> size = MEMLENGTH - BLOCK_SIZE; //Size of head is total memory minus block size
      SET_FREE(mem); //Mark head as free
      initialized = 1; //Set initialized to true so this doesn't run again
      atexit(leakDetector); //Register leak detector to run at program exit
   }
}


void *mymalloc(size_t byte, char* filename, int lineNo) {
   checkInitalization();

   byte = ((byte) + 7) & ~7; // Aligns up to the closest multiple of 8 bytes (Any multiple of 8 stays the same, and any non-multiple of 8 adds up to 7 bytes of padding to get it to the next multiple of 8)
   Block* current = mem; //Pointer to the head of the linked list

   while ((char*)current < (char*)mem + MEMLENGTH) { //For all blocks
      if (IS_FREE(current) && GET_SIZE(current) >= byte) { //Check if block is free and large enough for request
         if (GET_SIZE(current) - byte >= BLOCK_SIZE + 8) { //If the block is large enough to split, do so
            Block* new = (Block*)((char*)current + BLOCK_SIZE + byte); //Create new block after allocated space
            new -> size = GET_SIZE(current) - byte - BLOCK_SIZE; //Set new block size to remaining space
            SET_FREE(new); //Mark new block as free
            current -> size = byte; //Set current next to new block
         }
         SET_USED(current); //Mark current block as used
         printf("mymalloc: Allocated %d bytes at %p from %s:%d\n", (int)byte, (void*)(current + 1), filename, lineNo); //Allocation success message
         return (void*)((char*)current + BLOCK_SIZE); //Return pointer to memory after the block header
      }
      current = (Block*)((char*)current + BLOCK_SIZE + GET_SIZE(current)); //Move to next block
   }
   fprintf(stderr, "malloc: Unable to allocate %d bytes (%s:%d)\n", (int)byte, filename, lineNo); //Allocation failed error check (2.1 Detectable Errors)
   return NULL;
}


void illegalFree(char* file, int line) { //Handles inappropriate free calls (2.1 Detectable Errors)
   fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
   exit(2);
}

void myfree(void *ptr, char *file, int line) {
   checkInitalization();
   if (ptr == NULL) {
      printf("free: ptr is NULL\n"); 
      return; 
   }

   if ((char*)ptr < (char*)mem + BLOCK_SIZE || (char*)ptr >= (char*)mem + MEMLENGTH) { //Illegal free check for out of bounds pointer (2.1 Detectable Errors)
      illegalFree(file, line); 
   }

   // get block header 
   Block* block = mem;
   while ((char*)block < (char*)mem + MEMLENGTH) {
      if ((char*)block + BLOCK_SIZE == ptr) {
         if (IS_FREE(block)) { //Double free check (2.1 Detectable Errors)
            illegalFree(file, line);
            return; 
         } 

         SET_FREE(block); 

         // coalesce forward
         Block *next = (Block *)((char *)block + BLOCK_SIZE + GET_SIZE(block));
         while ((char*)next < (char*)mem + MEMLENGTH && IS_FREE(next)) { 
            SET_SIZE(block, GET_SIZE(block) + BLOCK_SIZE + GET_SIZE(next));
            next = (Block*)((char*)block + BLOCK_SIZE + GET_SIZE(block));
         } 

         // coalesce backward 
         Block *current = mem; 
         Block* prev = NULL;
         while ((char*)current < (char*)block) {
            prev = current;
            current = (Block*)((char*)current + BLOCK_SIZE + GET_SIZE(current));
         }
         if (prev && IS_FREE(prev)) {
            SET_SIZE(prev, GET_SIZE(prev) + BLOCK_SIZE + GET_SIZE(block));
            return;
         }
         return;
      }
      block = (Block*)((char*)block + BLOCK_SIZE + GET_SIZE(block));
   }
   illegalFree(file, line); //Illegal free check from somewhere other than the header of a block (2.1 Detectable Errors)
}