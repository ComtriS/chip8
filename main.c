#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <endian.h>
#include "errors.h"
#include "op.h"
#include "dasm.h"
#include "system.h"

int print_usage(const char* command)
{
	printf("Usage: %s ROM [-d|-D]\n", command);
	printf("  ROM   path to rom image\n");
	printf("  -d    dump disassembly\n");
	printf("  -D    dump disassembly and run\n");
	return ERR_GENERIC;
}

int main(int argc, char** argv)
{
	if (argc < 2 || argc > 3)
		return print_usage(argv[0]);
	
	const char* file = argv[1];
	bool dasm  = (argc == 3 && !strcmp(argv[2], "-d"));
	bool debug = (argc == 3 && !strcmp(argv[2], "-D"));
	
	system_init();
	system_load(file);
	uint16_t* rom = system_getRom();
	size_t size   = system_getSize();
	
	if (dasm)
		dasm_dump();
	else
		system_start(debug);
	
	return SUCCESS;
}
