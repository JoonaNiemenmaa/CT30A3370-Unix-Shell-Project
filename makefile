wish: wish.o vector.o path.o
	gcc wish.o vector.o path.o -g -o wish
wish.o: wish.c vector.h path.h
	gcc -g -c wish.c -o wish.o -std=c99 -Wall -pedantic
vector.o: vector.c vector.h
	gcc -g -c vector.c -o vector.o -std=c99 -Wall -pedantic
path.o: path.c path.h vector.h
	gcc -g -c path.c -o path.o -std=c99 -Wall -pedantic
