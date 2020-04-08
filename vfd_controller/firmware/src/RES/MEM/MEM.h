#ifndef __MEM_H__
#define __MEM_H__

#include <stdint.h>

extern int8_t   MEM_READ(uint16_t eememOffset, void *eememData, uint16_t size);
extern void     MEM_WRITE(uint16_t eememOffset, void *eememData, uint16_t size);

#endif
