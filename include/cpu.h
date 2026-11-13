#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <stdint.h>

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

// joins 2 registers. the first is high. ex: AB, A is high B is low
typedef union {
	uint16_t pair;
	struct {
		uint8_t low;
		uint8_t high;
	};
} reg_pair;

typedef struct {
	reg_pair bc;
	reg_pair de;
	reg_pair hl;
	reg_pair af;
} reg_state;

void set_flag(uint8_t *f, 
			uint8_t flag);

void clear_flag(uint8_t *f, 
				uint8_t flag);

int get_flag(uint8_t *f, 
				uint8_t flag);

#endif

