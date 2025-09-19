f = 

all: hello

hello:
	gcc -Wall -Werror -fsanitize=address,undefined -g $(f).c -o $(f).out -lm

nWarning:
	 gcc -Wall -fsanitize=address,undefined -g $(f).c -o $(f).out

std:
	gcc -Wall -Werror -fsanitize=address,undefined -g $(f).c -o $(f).out

clear:
	rm -rf $(f).out

run:
	./$(f).out

access:
	nano $(f).c

changeMake:
	nano Makefile