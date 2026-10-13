#ifndef CART_H
#define CART_H
#include <stdio.h>
#include <stdint.h>

typedef struct {
	char title[17];
	uint8_t cart_type;
	uint8_t rom_size;
	uint8_t ram_size;
} RomHeader;

int get_file_size(FILE *fp, long *out_size);
int load_rom(FILE *fp, long size, uint8_t **out_rom);
const char *decode_ram_size(uint8_t code);
const char *decode_rom_size(uint8_t code);
const char *decode_cart_type(uint8_t code);

RomHeader parse_header(const uint8_t *buf);

#endif
