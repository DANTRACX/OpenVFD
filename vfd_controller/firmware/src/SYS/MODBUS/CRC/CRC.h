#ifndef __CRC_H__
#define __CRC_H__

#include <stdint.h>

extern void CRC16(char *data, uint8_t size, char *crcLow, char *crcHigh);

#endif
