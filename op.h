#ifndef __OP_H__
#define __OP_H__

#include <stdint.h>
#include "errors.h"

#define OP_ERR_GENERIC   (OP_ERR_BASE - 1)
#define OP_ERR_BAD_INST  (OP_ERR_BASE - 2)

typedef uint8_t reg_t;
typedef uint8_t nibble_t;
typedef uint8_t byte_t;
typedef uint16_t word_t;

void op_do(word_t op);

#endif /* __OP_H__ */
