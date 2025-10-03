#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "mymalloc.h"

int main () {
   void* x;
   for (int i = 0; i <= 119; i++) {
      x = malloc(1);
      printf("Allocate  # %d\n", i);
      free(x);
   }

   void* y[120];
    for (int i = 0; i <= 119; i++) {
      y[i] = malloc(1);
      printf("Allocate  # %d\n", i);
   }
    for (int i = 0; i <= 119; i++) {
      free(y[i]);
      printf("Freed 1 byte  # %d\n", i);
   }
   return 0;
}
