#include "cart.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// define locations for data
#define HEADER_END 0x150
#define TITLE 0x134
#define CART_TYPE 0x147
#define ROM_SIZE 0x148
#define RAM_SIZE 0x149

const char *decode_ram_size(uint8_t code) {
	switch (code) {
		case 0x00: return "0 Bytes, No Ram";
		case 0x01: return "Unused";
		case 0x02: return "8 KiB, 1 Bank";
		case 0x03: return "32 KiB, 4 banks of 8 KiB each";
		case 0x04: return "128 KiB, 16 banks of 8 KiB each";
		case 0x05: return "64 KiB, 8 banks of 8 KiB each";
		default:   return "unknown";
	}
}

const char *decode_rom_size(uint8_t code) {
	switch (code) {
		case 0x00: return "32 KiB, 2 Banks";
		case 0x01: return "64 KiB, 4 Banks";
		case 0x02: return "128 KiB, 8 Banks";
		case 0x03: return "256 KiB, 16 Banks";
		case 0x04: return "512 KiB, 32 Banks";
		case 0x05: return "1 MiB, 64 Banks";
		case 0x06: return "2 MiB, 128 Banks";
		case 0x07: return "4 MiB, 256 Banks";
		case 0x08: return "8 MiB, 512 Banks";
		case 0x52: return "1.1 MiB, 72 Banks";
		case 0x53: return "1.2 MiB, 80 Banks";
		case 0x54: return "1.5 MiB, 96 Banks";
		default:   return "unknown";
	}
}

const char *decode_cart_type(uint8_t code) {
	switch (code) {
		case 0x00: return "ROM ONLY";
		case 0x01: return "MBC1";
		case 0x02: return "MBC1+RAM";
		case 0x03: return "MBC1+RAM+BATTERY";
		case 0x05: return "MBC2";
		case 0x06: return "MBC2+BATTERY";
		case 0x08: return "ROM+RAM";
		case 0x09: return "ROM+RAM+BATTERY";
		case 0x0B: return "MMM01";
		case 0x0C: return "MMM01+RAM";
		case 0x0D: return "MMM01+RAM+BATTERY";
		case 0x0F: return "MBC3+TIMER+BATTERY";
		case 0x10: return "MBC3+TIMER+RAM+BATTERY";
		case 0x11: return "MBC3";
		case 0x12: return "MBC3+RAM";
		case 0x13: return "MBC3+RAM+BATTERY";
		case 0x19: return "MBC5";
		case 0x1A: return "MBC5+RAM";
		case 0x1B: return "MBC5+RAM+BATTERY";
		case 0x1C: return "MBC5+RUMBLE";
		case 0x1D: return "MBC5+RUMBLE+RAM";
		case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
		case 0x20: return "MBC6";
		case 0x22: return "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
		case 0xFC: return "POCKET CAMERA";
		case 0xFD: return "BANDAI TAMA5";
		case 0xFE: return "HuC3";
		case 0xFF: return "HuC1+RAM+BATTERY";
		default:   return "unknown";
	}
}

RomHeader parse_header(const uint8_t *buf)
{
	RomHeader h;
	for (int i = 0; i <16; i++) {
		uint8_t ch = buf[TITLE + i];
		h.title[i] = ch;
	}
	h.title[16] = '\0';
	h.cart_type = buf[CART_TYPE];
	h.rom_size = buf[ROM_SIZE];
	h.ram_size = buf[RAM_SIZE];
	return h;
}

int get_file_size(FILE *fp, long *out_size) {
	long pre = ftell(fp);
	if (pre == -1) return -1;

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	if (size == -1) return -1;

	fseek(fp, pre, SEEK_SET);
	*out_size = size;
	return 0;
}

int load_rom(FILE *fp, long size, uint8_t **out_rom) {
	uint8_t *rom = malloc(size);
	if (rom == NULL) return 1;

	size_t n = fread(rom, 1, size, fp);
	if (n != (size_t)size) {
		free(rom);
		return 1;
	}

	*out_rom = rom;
	return 0;
}
