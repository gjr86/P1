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

run:
	./$(f)

runGrind:
	./$(grind)

changeMake:
	nano Makefile