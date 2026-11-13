#ifndef BUS_H
#define BUS_H
#include <stdio.h>
#include <stdint.h>

int bus_read8(uint16_t raw_addr, 
			  uint8_t *rom_buffer, 
			  uint8_t *out_byte);

#endif
