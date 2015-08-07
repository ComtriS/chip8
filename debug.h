#ifndef __DEBUG_H__
#define __DEBUG_H__

void debug_start(void);
void debug_end(void);

extern char debug_buffer[];
extern int debug_idx;
extern bool debug_enabled;

#define printf(...)                                                  \
	do {                                                             \
		debug_start();                                               \
		debug_idx += sprintf(&debug_buffer[debug_idx], __VA_ARGS__); \
		debug_end();                                                 \
	} while (0)

#endif /* __DEBUG_H__ */
