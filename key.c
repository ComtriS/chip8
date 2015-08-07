#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// special stuff:
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

int kbhit(void)
{
	fd_set rdfs;
	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);
 	
	struct timeval tv = {0};
	select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}

void key_mode(int dir)
{
	static struct termios oldt, newt;
 	
	if (dir == 1) {
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~( ICANON | ECHO );
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	} else {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	}
}

void key_test(void)
{
	key_mode(1);
	
	while (!kbhit())
		putchar('.');
	printf("%s: %c\n", __func__, getchar());
 	
	key_mode(0);
}

bool key_pressed(uint8_t key)
{
	if (kbhit()) {
		char c = getchar();
		if (key == strtol((char[]){c, 0}, NULL, 16))
			return true;
	}
	return false;
}

uint8_t key_get(void)
{
	while (1) {
		char c = getchar();
		uint8_t key = strtol((char[]){c, 0}, NULL, 16);
		
		if (key >= 0x0 && key <= 0xF)
			return key;
	}
}