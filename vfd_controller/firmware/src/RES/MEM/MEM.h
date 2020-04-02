#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

extern int8_t MEM_READ64(uint8_t int64addr, uint64_t *data);
extern uint8_t MEM_WRITE64(uint8_t int64addr, uint64_t data);

#endif
