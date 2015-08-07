#include <stdio.h>
#include <string.h>
#include "display.h"

#define DEBUG_X_MIN   -1
#define DEBUG_X_MAX   (DEBUG_X_MIN + 88)
#define DEBUG_X_SIZE  (DEBUG_X_MAX - DEBUG_X_MIN)
#define DEBUG_Y_MIN   33
#define DEBUG_Y_MAX   (DEBUG_Y_MIN + 10)
#define DEBUG_Y_SIZE  (DEBUG_Y_MAX - DEBUG_Y_MIN)

bool debug_inited = false;

char debug_buffer[DEBUG_X_MAX * DEBUG_Y_MAX];
int debug_idx = 0;
bool debug_enabled = false;

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

int str_count(char* str, char c)
{
	int i;
	for (i=0; str[i]; str[i]==c ? i++ : *str++);
	return i;
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
	printf("%s", debug_buffer);
	printf("                                            ");
	debug_idx = strlen(debug_buffer);
	display_gotoxy(DEBUG_X_MIN, DEBUG_Y_MAX);
	display_loadCursor();
}
