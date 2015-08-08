#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdbool.h>
#include <stdint.h>

void display_init(void);
void display_gotoxy(int x, int y);
void display_clear(void);
void display_set(int x, int y, bool value);
bool display_drawLine(int x, int y, uint8_t line);
void display_saveCursor(void);
void display_loadCursor(void);

#endif /* __DISPLAY_H__ */
