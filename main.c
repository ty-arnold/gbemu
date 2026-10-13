#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "cart.h"

int main(int argc, char *argv[]) {

	//saftey check on input param
	if (argc < 2) {
		fprintf(stderr, "usage: %s <romfile.gb>\n", argv[0]);
		return 1;
	}

	FILE *fp;
	char *filename = argv[1];
	long filesize;
	uint8_t *rom;

	// open the file, read + binary
	fp = fopen(filename, "rb");

	// check argument and make sure its not null
	if (fp == NULL) {
	    perror(filename);
		return 1;
	}

	if (get_file_size(fp, &filesize) != 0) {
		return 1;
	}
	if (load_rom(fp, filesize, &rom) != 0) {
		return 1;
	}
	RomHeader h = parse_header(rom);

	printf("Title:		%s\n", h.title);
	printf("Cartridge Type:	%s\n", decode_cart_type(h.cart_type));
	printf("ROM Size:	%s\n", decode_rom_size(h.rom_size));
	printf("RAM Size:	%s\n", decode_ram_size(h.ram_size));

	free(rom);
	fclose(fp);
	return 0;
}
