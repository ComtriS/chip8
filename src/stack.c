#include <stdint.h>
#include "system.h"

void stack_push(uint16_t data)
{
	// push stack
	chip8.SP -= SYSTEM_INST_SIZE;
	
	// save on stack
	chip8.ram.bytes[chip8.SP+0] = data >> 8;
	chip8.ram.bytes[chip8.SP+1] = data & 0xFF;
}

uint16_t stack_pop(void)
{
	// load from stack
	uint16_t data = chip8.ram.bytes[chip8.SP+0];
	data <<= 8;
	data |= chip8.ram.bytes[chip8.SP+1];
	
	// pop stack
	chip8.SP += SYSTEM_INST_SIZE;
	
	return data;
}
