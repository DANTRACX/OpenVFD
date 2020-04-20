#ifndef __MAVG_H__
#define __MAVG_H__

#include <stdint.h>

//#define MAVG_BUFFERSIZE   64
//#define MAVG_BUFFERSIZE  128
//#define MAVG_BUFFERSIZE  256
#define MAVG_BUFFERSIZE  512
//#define MAVG_BUFFERSIZE 1024
//#define MAVG_BUFFERSIZE 2048
//#define MAVG_BUFFERSIZE 4096

typedef struct MAVG_s
{
        uint16_t    buffer[MAVG_BUFFERSIZE];
        uint16_t    bufferpos;
        uint64_t    sum;
        uint64_t    avg;
}
MAVG_s;

extern  void        MAVG_INIT(MAVG_s *ptr);
extern  int16_t     MAVG_GET(MAVG_s *ptr);
extern  uint16_t    MAVG_UPDATE(MAVG_s *ptr, uint16_t value);

#endif
