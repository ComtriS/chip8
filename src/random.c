#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "random.h"

bool random_inited = false;

void random_init(void)
{
	srand(time(NULL));
	random_inited = true;
}

uint8_t random_byte(void)
{
	if (!random_inited)
		random_init();
	return (uint8_t)rand();
}
