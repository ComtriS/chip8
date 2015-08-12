#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "op.h"
#include "display.h"
#include "debug.h"
#include "font.h"
#include "key.h"

system_t  chip8    = {{0}};
uint16_t* rom_bin  = NULL;
size_t    rom_size = 0;

#define BYTES_PER_PC    2
#define TICKS_PER_INST  (CLOCKS_PER_SEC / 60.0f)

int system_init(void)
{
	memset(&chip8, 0, sizeof(chip8));
	chip8.SP = SYSTEM_BASE_SP;
	chip8.PC = SYSTEM_BASE_PC;
	memcpy(chip8.ram.font, font_chars, sizeof(font_chars));
	display_init();
	return key_init();
}

int system_load(const char* rom)
{
	FILE* file = fopen(rom, "rb");
	if (!file) {
		fprintf(stderr, "ERROR: Couldn't open %s\n", rom);
		return ERR_GENERIC;
	}
	
	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	if (file_size > sizeof(chip8.ram.program)) {
		fclose(file);
		return ERR_GENERIC;
	}
	
	rom_bin = malloc(file_size);
	fread(rom_bin, 1, file_size, file);
	fclose(file);
	
	rom_size = file_size / BYTES_PER_PC;
	
	memcpy(chip8.ram.program, rom_bin, file_size);
	return SUCCESS;
}

size_t system_getSize(void)
{
	return rom_size;
}

uint16_t* system_getRom(void)
{
	return rom_bin;
}

void system_incPC(void)
{
	chip8.PC += SYSTEM_INST_SIZE;
}

void system_decPC(void)
{
	chip8.PC -= SYSTEM_INST_SIZE;
}

word_t system_nextOp(void)
{
	return be16toh(*(word_t*)&chip8.ram.bytes[chip8.PC]);
}

void system_start(bool debug, bool step)
{
	debug_enabled      = debug || step;
	clock_t start_time = clock();
	int total_inst     = 0;
	
	while (1) {
		clock_t expected_time = start_time + (total_inst * TICKS_PER_INST);
		clock_t diff_time;
		
		do {
			diff_time = (clock() - expected_time);
		} while (diff_time < TICKS_PER_INST);
		
		int inst_count = diff_time / TICKS_PER_INST;
		
		while (inst_count--) {
			word_t op = system_nextOp();
			
			if (debug)
				debug_op(total_inst, chip8.PC, op);
			
			int status = op_do(op);
			
			if (status != SUCCESS) {
				// NOTE: this is generally not an issue, as most ROMs deliberately cause
				// an infinite loop when the intention is to signal program termination
				fprintf(stderr, "ERROR: %s\n", op_status(status));
				return;
			}
			
			if (step)
				debug_step();
			
			total_inst++;
		}
	}
}

void system_halt(void)
{
	system_destroy();
	exit(1);
}

void system_destroy(void)
{
	key_deinit();
	free(rom_bin);
	rom_bin = NULL;
}
