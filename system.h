#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

#define SYSTEM_BASE_SP  0xFA0
#define SYSTEM_BASE_PC  0x200

#define REG_V0   regs.V[0x0]
#define REG_V1   regs.V[0x1]
#define REG_V2   regs.V[0x2]
#define REG_V3   regs.V[0x3]
#define REG_V4   regs.V[0x4]
#define REG_V5   regs.V[0x5]
#define REG_V6   regs.V[0x6]
#define REG_V7   regs.V[0x7]
#define REG_V8   regs.V[0x8]
#define REG_V9   regs.V[0x9]
#define REG_VA   regs.V[0xA]
#define REG_VB   regs.V[0xB]
#define REG_VC   regs.V[0xC]
#define REG_VD   regs.V[0xD]
#define REG_VE   regs.V[0xE]
#define REG_VF   regs.V[0xF]  // The VF register doubles as a carry flag

typedef uint8_t reg_t;

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

typedef struct system {
	reg_t     V[0x10];   // CHIP-8 has 16 8-bit data registers named from V0 to VF
	uint16_t  I;      // The address register is used for memory operations
	uint16_t  SP;
	uint16_t  PC;
	uint8_t   delay_timer;
	uint8_t   sound_timer;
	ram_t     ram;
} system_t;

void system_init(void);
size_t system_getSize(void);
uint16_t* system_getRom(void);
uint16_t system_getPC(void);
void system_setPC(uint16_t pc);

#endif /* __SYSTEM_H__ */
