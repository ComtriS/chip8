#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "system.h"
#include "errors.h"

// special stuff:
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

// special stuff:
#include <pthread.h>

void key_mode(int dir)
{
	static struct termios oldt, newt;
 	
	if (dir == 1) {
		//system("/bin/stty raw");
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~( ICANON | ECHO );
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	} else {
		//system("/bin/stty cooked");
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	}
}

bool key_pressed(uint8_t key)
{
	if (chip8.keys[key]) {
		chip8.keys[key] = false;
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

void* key_thread(void* arg)
{
	while (1) {
		uint8_t key = key_get();
		chip8.keys[key] = 1;
	}
}

void key_clear(void)
{
	memset(chip8.keys, 0, sizeof(chip8.keys));
}

int key_init(void)
{
	key_clear();
	key_mode(1);
	
	pthread_t thread;
	if (pthread_create(&thread, NULL, key_thread, NULL)) {
		fprintf(stderr, "ERROR: couldn't create thread\n");
		return ERR_GENERIC;
	}
	
	return SUCCESS;
}

void key_deinit(void)
{
	key_mode(0);
}
