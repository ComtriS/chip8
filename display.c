#include <string.h>
#include <stdio.h>
#include "font.h"
#include "system.h"

// Display resolution is 64×32 pixels, and color is monochrome. Graphics are
// drawn to the screen solely by drawing sprites, which are 8 pixels wide and
// may be from 1 to 15 pixels in height. Sprite pixels that are set flip the
// color of the corresponding screen pixel, while unset sprite pixels do
// nothing. The carry flag (VF) is set to 1 if any screen pixels are flipped
// from set to unset when a sprite is drawn and set to 0 otherwise.

#define DISPLAY_ESC     0x1B
#define DISPLAY_BLOCK   "%c%c%c", 0xe2, 0x96, 0x88
#define DISPLAY_BLANK   " "

#define DISPLAY_MAX_X   64
#define DISPLAY_MAX_Y   32

#define DISPLAY_UD   "%c%c%c", 0xe2, 0x94, 0x83
#define DISPLAY_LR   "%c%c%c", 0xe2, 0x94, 0x81
#define DISPLAY_UL   "%c%c%c", 0xe2, 0x94, 0x8f
#define DISPLAY_UR   "%c%c%c", 0xe2, 0x94, 0x93
#define DISPLAY_DL   "%c%c%c", 0xe2, 0x94, 0x97
#define DISPLAY_DR   "%c%c%c", 0xe2, 0x94, 0x9b

bool display_screen[DISPLAY_MAX_X][DISPLAY_MAX_Y] = {{0}};
bool display_inited = false;

void display_gotoxy(int x, int y)
{
	if (!display_inited)
		return;
	
	printf("%c[%d;%df", DISPLAY_ESC, y+2, x+2);
}

void display_saveCursor(void)
{
	printf("%c7", DISPLAY_ESC);
}

void display_loadCursor(void)
{
	printf("%c8", DISPLAY_ESC);
}

void display_drawBox(int x1, int x2, int y1, int y2)
{
	if (!display_inited)
		return;
	
	// get around the limits of gotoxy
	x1--; x2--; y1--; y2--;
	
	// draw top line
	int i;
	display_gotoxy(x1, y1++);
	printf(DISPLAY_UL);
	for (i=x1; i<x2-1; i++)
		printf(DISPLAY_LR);
	printf(DISPLAY_UR);
	
	// draw left and right lines
	while (y1 < y2) {
		display_gotoxy(x1, y1);
		printf(DISPLAY_UD);
		
		display_gotoxy(x2, y1);
		printf(DISPLAY_UD);
		
		y1++;
	}
	
	// draw bottom line
	display_gotoxy(x1, y1++);
	printf(DISPLAY_DL);
	for (i=x1; i<x2-1; i++)
		printf(DISPLAY_LR);
	printf(DISPLAY_DR);
}

void display_update(int x, int y)
{
	if (!display_inited)
		return;
	
	display_gotoxy(x, y);
	bool value = display_screen[x][y];
	if (value)
		printf(DISPLAY_BLOCK);
	else
		printf(DISPLAY_BLANK);
	fflush(stdout);
}

void display_clear(void)
{
	if (!display_inited)
		return;
	
	printf("%c[2J", DISPLAY_ESC);
	printf("%c[1;1H", DISPLAY_ESC);
	memset(display_screen, 0, sizeof(display_screen));
	display_drawBox(0, DISPLAY_MAX_X+1, 0, DISPLAY_MAX_Y+1);
	fflush(stdout);
}

void display_init(void)
{
	display_inited = true;
	memcpy(chip8.ram.font, font_chars, sizeof(font_chars));
	display_clear();
}

void display_set(int x, int y, bool value)
{
	if (!display_inited)
		return;
	
	display_screen[x][y] = value;
	display_update(x, y);
}

bool display_get(int x, int y)
{
	return display_screen[x][y];
}

void display_toggle(int x, int y)
{
	if (!display_inited)
		return;
	
	display_screen[x][y] ^= 1;
	display_update(x, y);
}

uint8_t bit_reverse(uint8_t b)
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

bool display_drawLine(int x, int y, uint8_t line)
{
	bool unset = false;
	
	if (!display_inited)
		return unset;
	
	if (y < 0 || y >= DISPLAY_MAX_Y)
		return unset;
	
	line = bit_reverse(line);
	
	while (line) {
		
		if (x < 0 || x >= DISPLAY_MAX_X)
			break;
		
		if (line & 1) {
			if (display_get(x, y))
				unset = true;
			display_toggle(x, y);
		}
		
		x++;
		line >>= 1;
	}
	
	return unset;
}
