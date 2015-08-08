#ifndef __OP_H__
#define __OP_H__

#include <stdint.h>
#include "system.h"
#include "errors.h"

#define OP_ERR_GENERIC   (OP_ERR_BASE - 1)
#define OP_ERR_BAD_INST  (OP_ERR_BASE - 2)

int op_do(word_t op);
void op_dump(void);
void op_print(uint16_t pc, uint16_t op);
const char* op_status(int status);

#endif /* __OP_H__ */
