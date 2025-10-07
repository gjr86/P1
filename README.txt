Names: 
   Angelo Maniacci - amm928
   Gabriel Rodriguez - gjr86

Test Plans:
   For Test 4 on memgrind, the test is to randomly allocate memory of different sizes to a linked list strucutre or randomly remove either the head or tail, then free the entire list at the end.
   For Test 5 on memgrind, the test is to randomly allocate memory of variable sizes of either 16 or 24 bytes (8 byte header + the size of the node) to a BST and to randomly remove the minimum, then free the BST at the end.

   In memtest.c:
      Test 1: free a NULL pointer
      Test 2: allocate, then free, size 0 object
      Test 3: allocate, then free, one object near max size
      Test 4: check that chunks properly coalesce
      Test 5: allocate and free blocks of random size
      Test 6: try freeing twice (should give an error)

Descriptions: 
   memtest.c tests:
      Test 1: verify freeing NULL is safely ignored
      Test 2: check behavior of zero-byte allocation and deallocation
      Test 3: validate large allocation near the memory limit
      Test 4: confirm adjacent free blocks coalesce correctly
      Test 5: randomized allocations/frees to simulate general workload
      Test 6: ensure double frees trigger proper error handling

Design Notes: 
   Both tests designed for memgrind were made with the purpose of stressing mymalloc and myfree by making them randomly allocate and deallocate different sizes of bytes. We stress tested the capabilities of the functions, but made sure to not stress the limited memory of the 4096 bytes we have at our disposal.
   The tests in memtest.c are designed to test possible edge cases that could cause an error (operations that likely won't be called but could still cause an error)