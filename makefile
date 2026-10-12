gbemu: main.o cart.o
	gcc -o gbemu main.o cart.o

main.o: main.c cart.h
	gcc -c main.c

cart.o: cart.c cart.h
	gcc -c cart.c


