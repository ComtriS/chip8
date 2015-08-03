#ifndef __RAM_H__
#define __RAM_H__

#include <stdint.h>

// CHIP-8 has 4096 (0x1000) memory locations, each of which is 8 bits (a byte)
// Bytes 0x000-0x1FF: originally reserved for interpreter; now used for font data
// Bytes 0x200-0xE9F: program space
// Bytes 0xEA0-0xEFF: reserved for call stack, internal use, and other variables
// Bytes 0xF00-0xFFF: reserved for display refresh
typedef union ram {
	uint8_t bytes[0x1000];
	struct {
		uint8_t font   [0x200];
		uint8_t program[0xCA0];
		uint8_t stack  [0x60];
		uint8_t display[0x100];
	};
} ram_t;

extern ram_t ram;

#endif /* __RAM_H__ */
