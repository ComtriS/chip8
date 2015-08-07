#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <time.h>

// CLOCKS_PER_SEC
clock_t clock_time(void);
clock_t clock_diff(clock_t t1, clock_t t2);

#endif /* __CLOCK_H__ */
