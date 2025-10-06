 #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <time.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

// Compile with -DLEAK to leak memory
#ifndef LEAK
#define LEAK 0
#endif

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

int main(int argc, char **argv)
{
	char *obj[OBJECTS];
	int sizes[OBJECTS];
	int i, j, errors = 0;

	srand(time(NULL));
	
	// fill memory with objects of variable size
	for (i = 0; i < OBJECTS; i++) {
		sizes[i] = ((rand()%5)+2)*8;
		obj[i] = malloc(sizes[i]);
		if (obj[i] == NULL) {
		    printf("Unable to allocate object %d\n", i);
		    exit(1);
		}
	}
	
	// fill each object with distinct bytes
	for (i = 0; i < OBJECTS; i++) {
		memset(obj[i], i, sizes[i]);
	}

	// randomly reallocate some objects
	int test[] = {8, 12, 43, 55, 63, 24, 24, 7};
	for (i=0; i < 8; i++) {
	    free(obj[test[i]]);
		sizes[test[i]] = ((rand()%5)+2)*8;
		obj[test[i]] = malloc(sizes[test[i]]);
		memset(obj[test[i]], test[i], sizes[test[i]]);
	}

	
	// check that all objects contain the correct bytes
	for (i = 0; i < OBJECTS; i++) {
		for (j = 0; j < sizes[i]; j++) {
			if (obj[i][j] != i) {
				errors++;
				printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
			}
		}
	}

	// free all objects
	if (!LEAK) {
	    for (i = 0; i < OBJECTS; i++) {
		free(obj[i]);
	    }
	}
	
	printf("%d incorrect bytes\n", errors);


    // 1. Test freeing NULL pointer (should not crash)
    printf("Test 1: Freeing NULL pointer\n");
    free(NULL);

    // 2. Test zero-byte allocation (should return NULL or valid pointer)
    printf("Test 2: Allocating 0 bytes\n");
    char *zero = malloc(0);
    if (zero == NULL)
        printf(" - malloc(0) returned NULL (valid)\n");
    else {
        printf(" - malloc(0) returned %p (also valid)\n", zero);
        free(zero);
    }

    // 3. Allocate nearly all heap memory (stress allocator)
    printf("Test 3: Large allocation near heap limit\n");
    char *big = malloc(MEMSIZE - 64);
    if (big)
        printf(" - Large allocation succeeded (%p)\n", big);
    else
        printf(" - Large allocation failed as expected\n");
    free(big);

    // 4. Coalescing test — check if multiple frees merge into one big free block
    printf("Test 4: Coalescing adjacent blocks\n");
    char *a = malloc(128);
    char *b = malloc(128);
    char *c = malloc(128);
    printf(" - Allocated blocks: %p %p %p\n", a, b, c);
    free(b);
    free(a); // should coalesce a+b
    free(c); // should coalesce all three
    char *d = malloc(384);
    if (d)
        printf(" - Successfully coalesced into one large free region\n");
    else
        printf(" - Coalescing failed\n");
    free(d);

	// 5. Fragmentation and reuse
    printf("Test 5: Fragmentation stress test\n");
    #define N 20
    char *blocks[N];
    for (i = 0; i < N; i++)
        blocks[i] = malloc((rand() % 10 + 1) * 8);
    for (i = 0; i < N; i += 2)
        free(blocks[i]);
    for (i = 0; i < N; i += 2)
        blocks[i] = malloc((rand() % 10 + 1) * 8);
    for (i = 0; i < N; i++)
        free(blocks[i]);
    printf(" - Fragmentation test complete\n");

    // 6. Double free detection test
    printf("Test 6: Double free detection\n");
    char *e = malloc(64);
    free(e);
    printf(" - First free done\n");
    printf(" - Attempting second free (should print error)\n");
    free(e); // should trigger error handler
	
	return EXIT_SUCCESS;
}
