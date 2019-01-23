#ifndef __DEBUG_H__
#define __DEBUG_H__

void debug_start(void);
void debug_end(void);
void debug_step(void);
void debug_op(int num, uint8_t pc, uint8_t op);

extern char debug_buffer[];
extern int debug_idx;
extern bool debug_enabled;

#define printf(...)                                                  \
	do {                                                             \
		debug_start();                                               \
		debug_idx += sprintf(&debug_buffer[debug_idx], __VA_ARGS__); \
		debug_idx += sprintf(&debug_buffer[debug_idx], "\n");        \
		debug_end();                                                 \
	} while (0)

#endif /* __DEBUG_H__ */
