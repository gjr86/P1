# Call "make" to compile memtest.c with mymalloc.c, call "make run" to execute memtest, and call "make clear" to remove the compiled binary for memtest.
# Call "make grind" to compile memgrind.c with mymalloc.c, call "make runGrind" to execute memgrind, and call "make clearGrind" to remove the compiled binary for memgrind.
#
# All other commands are for debugging and convenience and can be ignored.
# Descriptions for memtest and memgrind can be found in their functions and in the README. 
#

f = memtest
malloc = mymalloc.c
grind = memgrind

all: hello

hello:
	gcc -Wall -Werror -fsanitize=address,undefined -g $(f).c $(malloc) -o $(f) -lm

grind:
	gcc -Wall -Werror -fsanitize=address,undefined -g $(grind).c $(malloc) -o $(grind) -lm

nWarning:
	 gcc -Wall -fsanitize=address,undefined -g $(f) -o $(f)

std:
	gcc -Wall -Werror -fsanitize=address,undefined -g $(f) -o $(f)

clear:
	rm -rf $(f)

clearGrind:
	rm -rf $(grind)

run:
	./$(f)

runGrind:
	./$(grind)

changeMake:
	nano Makefile