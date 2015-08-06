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
	printf("Usage: %s ROM [-d|-D|-s]\n", command);
	printf("  ROM   path to rom image\n");
	printf("  -d    dump disassembly\n");
	printf("  -D    dump disassembly while running\n");
	printf("  -s    step through code\n");
	return ERR_GENERIC;
}

int main(int argc, char** argv)
{
	if (argc < 2)
		return print_usage(argv[0]);
	
	const char* file = argv[1];
	
	bool dasm  = false;
	bool debug = false;
	bool step  = false;
	
	int i;
	for (i=0; i<argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'd': dasm  = true; break;
				case 'D': debug = true; break;
				case 's': step  = true; break;
			}
		}
	}
	
	system_init();
	system_load(file);
	
	if (dasm) {
		dasm_dump();
		return SUCCESS;
	}
	
	system_start(debug, step);
	return SUCCESS;
}
