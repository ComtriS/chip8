#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include "system.h"
#include "errors.h"

system_t chip8 = {0};
uint16_t* rom_bin = NULL;
size_t rom_size;

#define BYTES_PER_PC   2

size_t file_getSize(FILE* f)
{
	fseek(f, 0, SEEK_END);
	size_t fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	return fsize;
}

int file_load(FILE* f, void* buf, size_t size)
{
	fread(buf, 1, size, f);
}

void system_init(void)
{
	memset(&chip8, 0, sizeof(chip8));
	chip8.SP = SYSTEM_BASE_SP;
	chip8.PC = SYSTEM_BASE_PC;
}

int system_load(const char* rom)
{
	FILE* file = fopen(rom, "rb");
	if (!file) {
		printf("ERROR: Couldn't open %s\n", rom);
		return ERR_GENERIC;
	}
	
	size_t file_size = file_getSize(file);
	
	rom_bin = malloc(file_size);
	file_load(file, rom_bin, file_size);
	fclose(file);
	
	rom_size = file_size / BYTES_PER_PC;
	
	int i;
	for (i=0; i<rom_size; i++)
		rom_bin[i] = be16toh(rom_bin[i]);
}

size_t system_getSize(void)
{
	return rom_size;
}

uint16_t* system_getRom(void)
{
	return rom_bin;
}

void system_setPC(uint16_t pc)
{
	chip8.PC = pc;
}

uint16_t system_getPC(void)
{
	return chip8.PC;
}

int system_destroy(void)
{
	free(rom_bin);
	rom_bin = NULL;
}
