#include <stdio.h>
#include <string.h>
#include "display.h"
#include "str.h"
#include "op.h"

#define DEBUG_X_MIN   -1
#define DEBUG_X_MAX   (DEBUG_X_MIN + 88)
#define DEBUG_X_SIZE  (DEBUG_X_MAX - DEBUG_X_MIN)
#define DEBUG_Y_MIN   33
#define DEBUG_Y_MAX   (DEBUG_Y_MIN + 10)
#define DEBUG_Y_SIZE  (DEBUG_Y_MAX - DEBUG_Y_MIN)

bool debug_inited  = false;
bool debug_enabled = false;
int debug_idx      = 0;

char debug_buffer[DEBUG_X_MAX * DEBUG_Y_MAX];

void debug_op(int num, uint8_t pc, uint8_t op)
{
	printf("%5d] ", num);
	op_print(pc, op);
	printf("\n");
}

void debug_step(void)
{
	display_saveCursor();
	getchar();
	display_loadCursor();
}

void debug_start(void)
{
	if (!debug_enabled)
		return;
	
	if (!debug_inited) {
		display_gotoxy(DEBUG_X_MIN, DEBUG_Y_MIN);
		debug_inited = true;
		return;
	}
}

void debug_clear(void)
{
	display_gotoxy(DEBUG_X_MIN, DEBUG_Y_MIN);
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	printf("                                            \n");
	display_gotoxy(DEBUG_X_MIN, DEBUG_Y_MIN);
}

void debug_end(void)
{
	if (!debug_enabled) {
		printf("%s", debug_buffer);
		debug_idx = 0;
		return;
	}
	
	int count = str_count(debug_buffer, '\n');
	if (count > DEBUG_Y_SIZE) {
		char* next_line = strchr(debug_buffer, '\n') + 1;
		memmove(debug_buffer, next_line, strlen(next_line)+1);
	}
	
	display_saveCursor();
	debug_clear();
	printf("%s", debug_buffer);
	printf("                                            ");
	debug_idx = strlen(debug_buffer);
	display_loadCursor();
}
