#include <stdio.h>
#include "system.h"
#include "str.h"
#include "op.h"

static int op_error(word_t op)
{
	printf("ERROR: instruction not recognized (0x%04X)\n", op);
	return OP_ERR_BAD_INST;
}

static void op_print(word_t op)
{
	printf("0x%04X", op);
}

static const char* op_status(int status)
{
	switch (status) {
		case OP_ERR_GENERIC:      return STR(OP_ERR_GENERIC);
		case OP_ERR_BAD_INST:     return STR(OP_ERR_BAD_INST);
		case ERR_NOT_IMPLEMENTED: return STR(ERR_NOT_IMPLEMENTED);
		case SUCCESS:             return STR(SUCCESS);
	}
}

// 00E0: Clears the screen
static int op_00E0()
{
	// screen_clear();
	return ERR_NOT_IMPLEMENTED;
}

// 00EE: Returns from a subroutine
static int op_00EE()
{
	// subr_return();
	return ERR_NOT_IMPLEMENTED;
}

// 0NNN: Calls RCA 1802 program at address NNN
static int op_0NNN(word_t op)
{
	// uint16_t nnn = op & 0xFFF;
	return ERR_NOT_IMPLEMENTED;
}

// 0XXX: Handles instructions that start with '0'
static int op_0XXX(word_t op)
{
	switch (op) {
		case 0x00E0:  return op_00E0();
		case 0x00EE:  return op_00EE();
		default:      return op_0NNN(op);
	}
}

// 1NNN: Jumps to address NNN
static int op_1XXX(word_t op)
{
	int nnn = (op >> 0) & 0xFFF;
	
	chip8.PC = nnn;
	
	system_decPC();    // to avoid later increment
	return SUCCESS;
}

// 2NNN: Calls subroutine at NNN
static int op_2XXX(word_t op)
{
	int nnn = (op >> 0) & 0xFFF;
	
	chip8.SP -= SYSTEM_INST_SIZE;
	chip8.PC += SYSTEM_INST_SIZE;
	
	// save stack pointer on stack
	chip8.ram.bytes[chip8.SP-0] = chip8.PC >> 8;
	chip8.ram.bytes[chip8.SP-1] = chip8.PC & 0xFF;
	
	chip8.PC = nnn;
	
	system_decPC();    // to avoid later increment
	return SUCCESS;
}

// 3XNN: Skips the next instruction if VX equals NN
static int op_3XXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int nn = (op >> 0) & 0xFF;
	
	if (chip8.V[x] == nn)
		system_incPC();
	
	return SUCCESS;
}

// 4XNN: Skips the next instruction if VX doesn't equal NN
static int op_4XXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int nn = (op >> 0) & 0xFF;
	
	if (chip8.V[x] != nn)
		system_incPC();
	
	return SUCCESS;
}

// 5XY0: Skips the next instruction if VX equals VY
static int op_5XXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int y  = (op >> 4) & 0xF;
	
	if (chip8.V[x] == chip8.V[y])
		system_incPC();
	
	return SUCCESS;
}

// 6XNN: Sets VX to NN
static int op_6XXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int nn = (op >> 0) & 0xFF;
	
	chip8.V[x] = nn;
	
	return SUCCESS;
}

// 7XNN: Adds NN to VX
static int op_7XXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int nn = (op >> 0) & 0xFF;
	
	chip8.V[x] += nn;
	
	return SUCCESS;
}

// 8XYZ: Performs ALU instruction Z on VX and VY
static int op_8XXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int y  = (op >> 4) & 0xF;
	int z  = (op >> 0) & 0xF;
	
	reg_t* v = chip8.V;
	
	switch (z) {
		case 0x0: v[x]  = v[y]; return; // 8XY0	Sets VX to the value of VY.
		case 0x1: v[x] |= v[y]; return; // 8XY1	Sets VX to VX or VY.
		case 0x2: v[x] &= v[y]; return; // 8XY2	Sets VX to VX and VY.
		case 0x3: v[x] ^= v[y]; return; // 8XY3	Sets VX to VX xor VY.
		
		case 0x4:   // 8XY4	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			REG_VF = (v[x] > 0xFF - v[y]) ? 1 : 0;   // TODO: magic number
			v[x] += v[y];
			break;
		case 0x5:   // 8XY5	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			REG_VF = (v[x] < v[y]) ? 1 : 0;
			v[x] -= v[y];
			break;
		case 0x6:   // 8XY6	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
			REG_VF = v[x] & 0x1;
			v[x] >>= 1;
			break;
		case 0x7:   // 8XY7	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			REG_VF = (v[y] < v[x]) ? 1 : 0;
			v[x] = v[y] - v[x];
			break;
		case 0xE:   // 8XYE	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
			REG_VF = v[x] >> 7;   // TODO: magic number
			v[x] <<= 1;
			break;
		default:
			return ERR_NOT_IMPLEMENTED;
	}
	
	return SUCCESS;
}

// 9XY0: Skips the next instruction if VX doesn't equal VY
static int op_9XXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int y  = (op >> 4) & 0xF;
	
	if (chip8.V[x] != chip8.V[y])
		system_incPC();
	
	return SUCCESS;
}

// ANNN: Sets I to the address NNN
static int op_AXXX(word_t op)
{
	int nnn = (op >> 0) & 0xFFF;
	
	chip8.I = nnn;
	
	return SUCCESS;
}

// BNNN: Jumps to the address NNN plus V0
static int op_BXXX(word_t op)
{
	int nnn = (op >> 0) & 0xFFF;
	
	chip8.PC = nnn + chip8.V[0];
	
	system_decPC();    // to avoid later increment
	return SUCCESS;
}

// CXNN: Sets VX to a random number, masked by NN
static int op_CXXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int nn = (op >> 0) & 0xFF;
	
	chip8.V[x] = random_u8() & nn;
	
	return SUCCESS;
}

// DXYN: Sprites stored in memory at location in index register (I), maximum
// 8bits wide. Wraps around the screen. If when drawn, clears a pixel, register
// VF is set to 1 otherwise it is zero. All drawing is XOR drawing (i.e. it
// toggles the screen pixels)
static int op_DXXX(word_t op)
{
	return ERR_NOT_IMPLEMENTED;
}

// EX9E: Skips the next instruction if the key stored in VX is pressed
static int op_EX9E(word_t op)
{
	// return inst_skip(key_isOn(v[x]));
	return ERR_NOT_IMPLEMENTED;
}

// EXA1: Skips the next instruction if the key stored in VX isn't pressed
static int op_EXA1(word_t op)
{
	// return inst_skip(key_isOff(v[x]));
	return ERR_NOT_IMPLEMENTED;
}

// EXXX: Handles instructions that start with 'E'
static int op_EXXX(word_t op)
{
	switch (op & 0xFF) {
		case 0x9E: return op_EX9E(op);
		case 0xA1: return op_EXA1(op);
		default:   return op_error(op);
	}
}

// FX07: Sets VX to the value of the delay timer
static int op_FX07(word_t op)
{
	// return set(&v[n[2]], time_delay); 
	return ERR_NOT_IMPLEMENTED;
}

// FX0A: A key press is awaited, and then stored in VX
static int op_FX0A(word_t op)
{
	// return key_get(&v[n[2]]);         
	return ERR_NOT_IMPLEMENTED;
}

// FX15: Sets the delay timer to VX
static int op_FX15(word_t op)
{
	// return set(&time_delay, v[n[2]]); 
	return ERR_NOT_IMPLEMENTED;
}

// FX18: Sets the sound timer to VX
static int op_FX18(word_t op)
{
	// return set(&time_sound, v[n[2]]); 
	return ERR_NOT_IMPLEMENTED;
}

// FX1E: Adds VX to I
static int op_FX1E(word_t op)
{
	// return add(&reg_i, v[n[2]]);   
	return ERR_NOT_IMPLEMENTED;
}

#define FONT_BASE 0x0

// FX29: Sets I to the location of the sprite for the character in VX.
// Characters 0-F (in hexadecimal) are represented by a 4x5 font
static int op_FX29(word_t op)
{
	// int reg = op & 0x0F00 >> 8;
	// reg_i = FONT_BASE + (v[reg] * 5);
	return ERR_NOT_IMPLEMENTED;                     
}

// FX33: Stores the Binary-coded decimal representation of VX, with the most
// significant of three digits at the address in I, the middle digit at I plus
// 1, and the least significant digit at I plus 2. (In other words, take the
// decimal representation of VX, place the hundreds digit in memory at location
// in I, the tens digit at location I+1, and the ones digit at location I+2
static int op_FX33(word_t op)
{
	return ERR_NOT_IMPLEMENTED;
}

// FX55: Stores V0 to VX in memory starting at address I
static int op_FX55(word_t op)
{
	// return ram_save(v[n[2]]);         
	return ERR_NOT_IMPLEMENTED;
}

// FX65: Fills V0 to VX with values from memory starting at address I
static int op_FX65(word_t op)
{
	// return ram_load(v[n[2]]);         
	return ERR_NOT_IMPLEMENTED;
}

// FXXX: Handles instructions that start with 'F'
static int op_FXXX(word_t op)
{
	switch (op & 0xFF) {
		case 0x07: return op_FX07(op);
		case 0x0A: return op_FX0A(op);
		case 0x15: return op_FX15(op);
		case 0x18: return op_FX18(op);
		case 0x1E: return op_FX1E(op);
		case 0x29: return op_FX29(op);
		case 0x33: return op_FX33(op);
		case 0x55: return op_FX55(op);
		case 0x65: return op_FX65(op);
		default:   return op_error(op);
	}
}

// XXXX: Handles an instruction
static int op_XXXX(word_t op)
{
	uint8_t top_nibble = op >> 12;
	
	switch (top_nibble) {
		case 0x0: return op_0XXX(op);
		case 0x1: return op_1XXX(op); 
		case 0x2: return op_2XXX(op); 
		case 0x3: return op_3XXX(op); 
		case 0x4: return op_4XXX(op); 
		case 0x5: return op_5XXX(op); 
		case 0x6: return op_6XXX(op); 
		case 0x7: return op_7XXX(op); 
		case 0x8: return op_8XXX(op); 
		case 0x9: return op_9XXX(op); 
		case 0xA: return op_AXXX(op); 
		case 0xB: return op_BXXX(op); 
		case 0xC: return op_CXXX(op); 
		case 0xD: return op_DXXX(op); 
		case 0xE: return op_EXXX(op); 
		case 0xF: return op_FXXX(op); 
	}
}

void op_do(word_t op)
{
	int status = op_XXXX(op);
	system_incPC();
	op_print(op);
	printf(" => %s\n", op_status(status));
}
