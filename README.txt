Names: 
   Angelo Maniacci - amm928
   Gabriel Rodriguez - 

Test Plans:
   For Test 4 on memgrind, the test is to randomly allocate memory of different sizes to a linked list strucutre or randomly remove either the head or tail, then free the entire list at the end.
   For Test 5 on memgrind, the test is to randomly allocate memory of variable sizes of either 16 or 24 bytes (8 byte header + the size of the node) to a BST and to randomly remove the minimum, then free the BST at the end.

Descriptions: 


Design Notes: 
   Both tests designed for memgrind were made with the purpose of stressing mymalloc and myfree by making them randomly allocate and deallocate different sizes of bytes. We stress tested the capabilities of the functions, but made sure to not stress the limited memory of the 4096 bytes we have at our disposal.