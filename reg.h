#ifndef __REG_H__
#define __REG_H__

#include <stdint.h>

typedef uint8_t reg_t;

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

typedef struct regs {
	reg_t V[0x10];   // CHIP-8 has 16 8-bit data registers named from V0 to VF
	uint16_t I;      // The address register is used for memory operations
	uint16_t SP;
	uint16_t PC;
	uint8_t delay_timer;
	uint8_t sound_timer;
} regs_t;

extern regs_t regs;

#endif /* __REG_H__ */
