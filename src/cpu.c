
#include <stdint.h>
#include <stdio.h>
#include "cpu.h"

void set_flag(uint8_t *f, uint8_t flag) {
	*f = *f | flag; // sets specified flag with OR
};
void clear_flag(uint8_t *f, uint8_t flag) {
	*f = *f & ~flag; // invert flag then AND: specified flag gets cleared
};
int get_flag(uint8_t *f, uint8_t flag) {
	if ((*f & flag) != 0) // if the specified flag is set, return 1 (true) else 0 (false)
		return 1;
	else 
		return 0;
};
