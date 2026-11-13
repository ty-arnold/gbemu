CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

.PHONY: all clean run

all: build/gbemu

clean:
	rm -rf build

run: build/gbemu
	build/gbemu roms/Tetris.gb

build/gbemu: build/main.o build/cart.o build/bus.o build/cpu.o | build
	$(CC) -o build/gbemu build/main.o build/cart.o build/bus.o build/cpu.o

build:
	mkdir -p build

build/main.o: src/main.c include/cart.h include/bus.h include/cpu.h | build
	$(CC) $(CFLAGS) -c  src/main.c -o build/main.o

build/cart.o: src/cart.c include/cart.h | build
	$(CC) $(CFLAGS) -c  src/cart.c -o build/cart.o

build/bus.o: src/bus.c include/bus.h | build
	$(CC) $(CFLAGS) -c src/bus.c -o build/bus.o

build/cpu.o: src/cpu.c include/cpu.h | build
	$(CC) $(CFLAGS) -c src/cpu.c -o build/cpu.o

