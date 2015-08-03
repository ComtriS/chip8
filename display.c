#include <stdio.h>
#include <stdbool.h>

#define DISPLAY_ESC   0x1B
#define DISPLAY_BLOCK 0xB2

void display_gotoxy(int x, int y)
{
	printf("%c[%d;%df", DISPLAY_ESC, y, x);
}

void display_clear(void)
{
	printf("%c[2J", DISPLAY_ESC);
	printf("%c[0;0H", DISPLAY_ESC);
}

void display_set(int x, int y, bool value)
{
	display_gotoxy(x, y);
	printf("%c", value ? DISPLAY_BLOCK : ' ');
}
