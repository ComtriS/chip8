#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
#include "errors.h"
#include "op.h"
#include "dasm.h"
#include "system.h"

int print_usage(const char* command)
{
	printf("Usage: %s [ROM]\n", command);
	return ERR_GENERIC;
}

int main(int argc, char** argv)
{
	if (argc != 2)
		return print_usage(argv[0]);
	
	const char* file = argv[1];
	
	system_init();
	system_load(file);
	uint16_t* rom = system_getRom();
	size_t size   = system_getSize();
	
	int i;
	for (i=0; i<size; i++) {
		dasm_op(i, rom[i]);
		printf("\n");
	}
	return 0;
}
