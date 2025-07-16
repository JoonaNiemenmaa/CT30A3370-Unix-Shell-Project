wish: wish.o vector.o
	gcc wish.o vector.o -o wish
wish.o: wish.c vector.h
	gcc -c wish.c -o wish.o -std=c99 -Wall -pedantic
vector.o: vector.c vector.h
	gcc -c vector.c -o vector.o -std=c99 -Wall -pedantic
