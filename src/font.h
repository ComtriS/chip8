#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>

#define FONT_BYTES_PER_CHAR    5
#define FONT_CHAR_COUNT        16
#define FONT_SIZE              (FONT_BYTES_PER_CHAR * FONT_CHAR_COUNT)

extern uint8_t font_chars[FONT_SIZE];

#endif /* __FONT_H__ */
