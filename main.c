#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cart.h"
#include "bus.h"
#include "cpu.h"

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
	rom_header h = parse_header(rom);

	printf("Title:		%s\n", h.title);
	printf("Cartridge Type:	%s\n", decode_cart_type(h.cart_type));
	printf("ROM Size:	%s\n", decode_rom_size(h.rom_size));
	printf("RAM Size:	%s\n", decode_ram_size(h.ram_size));

	uint8_t test;
	if (bus_read8(0x0100, rom, &test) != 0) {
		return 1;
	}

	printf("Byte return:	%02x\n", test);

	reg_pair pair;
	pair.pair = 0x1234;
	printf("high = %02x, low = %02x\n", pair.high, pair.low);

	uint8_t f = 0;
	set_flag(&f, FLAG_Z);
	printf("set_flag = %02x\n", f);

	clear_flag(&f, FLAG_Z);
	printf("clear_flag = %02x\n", f);

	printf("get_flag = %d\n", get_flag(&f, FLAG_Z));

	reg_state test_state;
	test_state.af.low = 0;
	set_flag(&test_state.af.low, FLAG_Z);
	printf("set_flag = %02x\n", test_state.af.low);

	//cleanup
	free(rom);
	fclose(fp);
	return 0;
}
