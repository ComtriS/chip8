#include <stdio.h>
#include "system.h"
#include "str.h"
#include "op.h"
#include "display.h"
#include "debug.h"
#include "random.h"
#include "key.h"
#include "font.h"
#include "stack.h"

void op_print(uint16_t pc, uint16_t op)
{
	printf("%04X %04X  ", pc, op);
	uint8_t nibble1 = op >> 12 & 0xF;
	uint8_t nibble2 = op >> 8  & 0xF;
	uint8_t nibble3 = op >> 4  & 0xF;
	uint8_t nibble4 = op >> 0  & 0xF;
	uint8_t byte2   = op >> 0  & 0xFF;
	uint16_t nnn    = op & 0xFFF;
	
	switch (nibble1) {
		case 0x0: 
			switch (byte2) {
				case 0xE0: printf("%-10s", "CLS"); return;
				case 0xEE: printf("%-10s", "RTS"); return;
			}
			break;
		case 0x1: printf("%-10s $%03X",        "JUMP",    nnn);              return;
		case 0x2: printf("%-10s $%03X",        "CALL",    nnn);              return;
		case 0x3: printf("%-10s V%01X,#$%02X", "SKIP.EQ", nibble2, byte2);   return;
		case 0x4: printf("%-10s V%01X,#$%02X", "SKIP.NE", nibble2, byte2);   return;
		case 0x5: printf("%-10s V%01X,V%01X",  "SKIP.EQ", nibble2, nibble3); return;
		case 0x6: printf("%-10s V%01X,#$%02X", "MVI",     nibble2, byte2);   return;
		case 0x7: printf("%-10s V%01X,#$%02X", "ADI",     nibble2, byte2);   return;
		case 0x8:
			switch (nibble4) {
				case 0x0: printf("%-10s V%01X,V%01X",       "MOV.", nibble2, nibble3);          return;
				case 0x1: printf("%-10s V%01X,V%01X",       "OR. ", nibble2, nibble3);          return;
				case 0x2: printf("%-10s V%01X,V%01X",       "AND.", nibble2, nibble3);          return;
				case 0x3: printf("%-10s V%01X,V%01X",       "XOR.", nibble2, nibble3);          return;
				case 0x4: printf("%-10s V%01X,V%01X",       "ADD.", nibble2, nibble3);          return;
				case 0x5: printf("%-10s V%01X,V%01X,V%01X", "SUB.", nibble2, nibble2, nibble3); return;
				case 0x6: printf("%-10s V%01X,V%01X",       "SHR.", nibble2, nibble3);          return;
				case 0x7: printf("%-10s V%01X,V%01X,V%01X", "SUB.", nibble2, nibble3, nibble3); return;
				case 0xe: printf("%-10s V%01X,V%01X",       "SHL.", nibble2, nibble3);          return;
			}
			break;
		case 0x9: printf("%-10s V%01X,V%01X",        "SKIP.NE", nibble2, nibble3);          return;
		case 0xa: printf("%-10s I,#$%03X",           "MVI",     nnn);                       return;
		case 0xb: printf("%-10s $%03X(V0)",          "JUMP",    nnn);                       return;
		case 0xc: printf("%-10s V%01X,#$%02X",       "RNDMSK",  nibble2, byte2);            return;
		case 0xd: printf("%-10s V%01X,V%01X,#$%01X", "SPRITE",  nibble2, nibble3, nibble4); return;
		case 0xe: 
			switch (byte2) {
				case 0x9E: printf("%-10s V%01X", "SKIPKEY.Y", nibble2); return;
				case 0xA1: printf("%-10s V%01X", "SKIPKEY.N", nibble2); return;
			}
			break;
		case 0xf: 
			switch (byte2) {
				case 0x07: printf("%-10s V%01X,DELAY",  "MOV",        nibble2); return;
				case 0x0a: printf("%-10s V%01X",        "KEY",        nibble2); return;
				case 0x15: printf("%-10s DELAY,V%01X",  "MOV",        nibble2); return;
				case 0x18: printf("%-10s SOUND,V%01X",  "MOV",        nibble2); return;
				case 0x1e: printf("%-10s I,V%01X",      "ADI",        nibble2); return;
				case 0x29: printf("%-10s I,V%01X",      "SPRITECHAR", nibble2); return;
				case 0x33: printf("%-10s (I),V%01X",    "MOVBCD",     nibble2); return;
				case 0x55: printf("%-10s (I),V0-V%01X", "MOVM",       nibble2); return;
				case 0x65: printf("%-10s V0-V%01X,(I)", "MOVM",       nibble2); return;
			}
			break;
	}
	
	printf("**ERROR**");
}

void op_dump(void)
{
	uint16_t* rom = system_getRom();
	size_t size   = system_getSize();
	
	int i;
	for (i=0; i<size; i++) {
		op_print(i + SYSTEM_BASE_PC, rom[i]);
		printf("\n");
	}
}

static int op_error(word_t op)
{
	printf("ERROR: instruction not recognized (0x%04X)\n", op);
	return OP_ERR_BAD_INST;
}

const char* op_status(int status)
{
	switch (status) {
		case OP_ERR_GENERIC:      return STR(OP_ERR_GENERIC);
		case OP_ERR_BAD_INST:     return STR(OP_ERR_BAD_INST);
		case ERR_NOT_IMPLEMENTED: return STR(ERR_NOT_IMPLEMENTED);
		case SUCCESS:             return STR(SUCCESS);
	}
	static char buf[256];
	sprintf(buf, "%d", status);
	return buf;
}

// 00E0: Clears the screen
static int op_00E0(void)
{
	display_clear();
	return SUCCESS;
}

// 00EE: Returns from a subroutine
static int op_00EE(void)
{
	// load PC from stack
	chip8.PC = stack_pop();
	
	// to avoid later increment
	system_decPC();
	return SUCCESS;
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
	
	if (chip8.PC == nnn)
		system_halt();  // infinite loop detected
	
	chip8.PC = nnn;
	
	// to avoid later increment
	system_decPC();
	return SUCCESS;
}

// 2NNN: Calls subroutine at NNN
static int op_2XXX(word_t op)
{
	int nnn = (op >> 0) & 0xFFF;
	
	// save next PC on stack
	system_incPC();
	stack_push(chip8.PC);
	
	// set PC to subroutine
	chip8.PC = nnn;
	
	// to avoid later increment
	system_decPC();
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
		case 0x0: v[x]  = v[y]; break; // 8XY0	Sets VX to the value of VY.
		case 0x1: v[x] |= v[y]; break; // 8XY1	Sets VX to VX or VY.
		case 0x2: v[x] &= v[y]; break; // 8XY2	Sets VX to VX and VY.
		case 0x3: v[x] ^= v[y]; break; // 8XY3	Sets VX to VX xor VY.
		
		case 0x4:   // 8XY4	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			REG_VF = (v[x] > 0xFF - v[y]) ? 1 : 0;
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
			REG_VF = v[x] >> 7;
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
	
	// to avoid later increment
	system_decPC();
	return SUCCESS;
}

// CXNN: Sets VX to a random number, masked by NN
static int op_CXXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int nn = (op >> 0) & 0xFF;
	
	chip8.V[x] = random_byte() & nn;
	
	return SUCCESS;
}

// DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and
// a height of N pixels (1 to 15). Wraps around the screen. Each row of 8 pixels
// is read as bit-coded (with the most significant bit of each byte displayed on
// the left) starting from memory location I; I value doesn’t change after the
// execution of this instruction. All drawing is XOR drawing (i.e. it toggles
// the screen pixels) VF is set to 1 if any screen pixels are flipped from set
// to unset when the sprite is drawn, and to 0 if that doesn’t happen.
static int op_DXXX(word_t op)
{
	int x  = (op >> 8) & 0xF;
	int y  = (op >> 4) & 0xF;
	int n  = (op >> 0) & 0xF;
	
	uint8_t* sprite = &chip8.ram.bytes[chip8.I];
	
	REG_VF = 0;
	
	int i;
	for (i=0; i<n; i++) {
		uint8_t line = sprite[i];
		bool unset = display_drawLine(chip8.V[x], chip8.V[y]+i, line);
		if (unset)
			REG_VF = 1;
	}
	
	return SUCCESS;
}

// EX9E: Skips the next instruction if the key stored in VX is pressed
static int op_EX9E(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	if (key_pressed(chip8.V[x]))
		system_incPC();
	
	return SUCCESS;
}

// EXA1: Skips the next instruction if the key stored in VX isn't pressed
static int op_EXA1(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	if (!key_pressed(chip8.V[x]))
		system_incPC();
	
	return SUCCESS;
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
	int x  = (op >> 8) & 0xF;
	
	chip8.V[x] = chip8.delay_timer;
	
	return SUCCESS;
}

// FX0A: A key press is awaited, and then stored in VX
static int op_FX0A(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	chip8.V[x] = key_get();
	
	return SUCCESS;
}

// FX15: Sets the delay timer to VX
static int op_FX15(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	chip8.delay_timer = chip8.V[x];
	
	return SUCCESS;
}

// FX18: Sets the sound timer to VX
static int op_FX18(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	chip8.sound_timer = chip8.V[x];
	
	return SUCCESS;
}

// FX1E: Adds VX to I
static int op_FX1E(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	chip8.I += chip8.V[x];
	
	return SUCCESS;
}

// FX29: Sets I to the location of the sprite for the character in VX.
// Characters 0-F (in hexadecimal) are represented by a 4x5 font
static int op_FX29(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	uint8_t font_addr = chip8.ram.bytes - chip8.ram.font;
	chip8.I = font_addr + FONT_BYTES_PER_CHAR * chip8.V[x];
	
	return SUCCESS;
}

// FX33: Stores the hundreds digit of VX in memory at location I,
// the tens digit at location I+1, the ones digit at location I+2
static int op_FX33(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	chip8.ram.bytes[chip8.I+0] = (chip8.V[x] / 100) % 10;
	chip8.ram.bytes[chip8.I+1] = (chip8.V[x] / 10)  % 10;
	chip8.ram.bytes[chip8.I+2] = (chip8.V[x] / 1)   % 10;
	
	return SUCCESS;
}

// FX55: Stores V0 to VX in memory starting at address I
static int op_FX55(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	uint8_t* ram = chip8.ram.bytes;
	ram += chip8.I;
	
	int i;
	for (i=0; i<=x; i++)
		ram[i] = chip8.V[i];
	
	return SUCCESS;
}

// FX65: Fills V0 to VX with values from memory starting at address I
static int op_FX65(word_t op)
{
	int x  = (op >> 8) & 0xF;
	
	uint8_t* ram = chip8.ram.bytes;
	ram += chip8.I;
	
	int i;
	for (i=0; i<=x; i++)
		chip8.V[i] = ram[i];
	
	return SUCCESS;
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
	
	return OP_ERR_BAD_INST;
}

int op_do(word_t op)
{
	static int op_count = 0;

	int status = op_XXXX(op);
	op_count++;
	
	system_incPC();
	system_decTimers(op_count);
	return status;
}
