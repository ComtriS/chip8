#include <time.h>

clock_t clock_time(void)
{
	return clock();
}

clock_t clock_diff(clock_t t1, clock_t t2)
{
	return (t1 > t2) ? t1 - t2 : t2 - t1;
}
