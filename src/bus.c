#include <stdint.h>
#include <stdio.h>

#define ROM_UPPER 0x7FFF 

int bus_read8(uint16_t raw_addr, 
			  uint8_t *rom_buffer, 
			  uint8_t *out_byte) 
{
	uint8_t byte;
	if (raw_addr <= ROM_UPPER) {
		byte = rom_buffer[raw_addr];
		*out_byte = byte;
		return 0;
	}
	else return 1;
}
