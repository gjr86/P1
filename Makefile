f = memtest
f2 = mymalloc.c

all: hello

hello:
	gcc -Wall -Werror -fsanitize=address,undefined -g $(f).c $(f2) -o $(f) -lm

nWarning:
	 gcc -Wall -fsanitize=address,undefined -g $(f).c -o $(f)

std:
	gcc -Wall -Werror -fsanitize=address,undefined -g $(f).c -o $(f)

clear:
	rm -rf $(f)

run:
	./$(f)

access:
	nano $(f).c

changeMake:
	nano Makefile