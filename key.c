#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include "system.h"
#include "errors.h"

bool key_waiting = false;
uint8_t key_received;
pthread_cond_t key_cond;
pthread_mutex_t key_mutex;
pthread_t key_thread_handle;

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
	pthread_mutex_lock(&key_mutex);
	pthread_cond_wait(&key_cond, &key_mutex);
	
	chip8.keys[key_received] = 0;
	uint8_t key = key_received;
	
	pthread_mutex_unlock(&key_mutex);
	
	return key;
}

void* key_thread(void* arg)
{
	while (1) {
		char c = getchar();
		uint8_t key = strtol((char[]){c, 0}, NULL, 16);
		
		if (key < 0x0 || key > 0xF)
			continue;
		if (key == 0x0 && c != '0')
			continue;
		
		pthread_mutex_lock(&key_mutex);
		
		chip8.keys[key] = 1;
		key_received = key;
		
		pthread_cond_signal(&key_cond);
		pthread_mutex_unlock(&key_mutex);
	}
}

void key_clear(void)
{
	memset(chip8.keys, 0, sizeof(chip8.keys));
}

int key_init(void)
{
	if (pthread_cond_init(&key_cond, NULL) != 0) {
		fprintf(stderr, "ERROR: pthread_cond_init\n");
		return ERR_GENERIC;
	}
	
	if (pthread_mutex_init(&key_mutex, NULL) != 0) {
		fprintf(stderr, "ERROR: pthread_mutex_init\n");
		return ERR_GENERIC;
	}
	
	if (pthread_create(&key_thread_handle, NULL, key_thread, NULL) != 0) {
		fprintf(stderr, "ERROR: pthread_create\n");
		return ERR_GENERIC;
	}
	
	key_clear();
	key_mode(1);
	return SUCCESS;
}

void key_deinit(void)
{
	pthread_cond_destroy(&key_cond);
	pthread_mutex_destroy(&key_mutex);
	pthread_cancel(key_thread_handle);
	key_mode(0);
}
