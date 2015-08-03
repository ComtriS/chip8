#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
#include "errors.h"
#include "op.h"
#include "dasm.h"

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

int print_usage(const char* command)
{
	printf("Usage: %s [ROM]\n", command);
	return ERR_GENERIC;
}

int main(int argc, char** argv)
{
	if (argc != 2)
		return print_usage(argv[0]);
	
	FILE* f = fopen(argv[1], "rb");
	if (!f) {
		printf("error: Couldn't open %s\n", argv[1]);
		exit(1);
	}
	
	size_t size = file_getSize(f);
	
	uint16_t* buf = malloc(size);
	file_load(f, buf, size);
	
	fclose(f);
	
	int i;
	for (i=0; i<size; i++)
		buf[i] = htobe16(buf[i]);
	
	for (i=0; i<size; i++) {
		dasm_op(i + 0x200, buf[i]);
		printf("\n");
	}
	return 0;
}
