#ifndef __MAVG_H__
#define __MAVG_H__

#include <stdint.h>

typedef struct MAVG_s
{
    int16_t buffer[256];
    uint8_t bufferpos;
    int32_t sum;
    int32_t avg;
}
MAVG_s;

extern void MAVG_INIT(MAVG_s *ptr);
extern int16_t MAVG_GET(MAVG_s *ptr);
extern int16_t MAVG_UPDATE(MAVG_s *ptr, int16_t value);

#endif
