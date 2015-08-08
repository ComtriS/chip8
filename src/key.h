#ifndef __KEY_H__
#define __KEY_H__

#include <stdint.h>

bool    key_pressed (uint8_t key);
uint8_t key_get     (void);
void    key_clear   (void);
int     key_init    (void);
void    key_deinit  (void);

#endif /* __KEY_H__ */
