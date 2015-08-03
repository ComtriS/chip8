#include <stdio.h>
#include <stdint.h>
#include "system.h"

void dasm_op(uint16_t pc, uint16_t op)
{
	printf("%04X %04X  ", pc + SYSTEM_BASE_PC, op);
	uint8_t nibble1 = op >> 12 & 0xF;
	uint8_t nibble2 = op >> 8  & 0xF;
	uint8_t nibble3 = op >> 4  & 0xF;
	uint8_t nibble4 = op >> 0  & 0xF;
	uint8_t byte1   = op >> 8  & 0xFF;
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
