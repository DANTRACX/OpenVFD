#ifndef __MAVG_H__
#define __MAVG_H__

#include <stdint.h>

typedef struct MAVG8_s
{
        int16_t     buffer[8];
        uint8_t     bufferpos;
        int64_t     sum;
        int64_t     avg;
}
MAVG8_s;

typedef struct MAVG16_s
{
        int16_t     buffer[16];
        uint8_t     bufferpos;
        int64_t     sum;
        int64_t     avg;
}
MAVG16_s;

typedef struct MAVG32_s
{
        int16_t     buffer[32];
        uint8_t     bufferpos;
        int64_t     sum;
        int64_t     avg;
}
MAVG32_s;

typedef struct MAVG64_s
{
        int16_t     buffer[64];
        uint8_t     bufferpos;
        int64_t     sum;
        int64_t     avg;
}
MAVG64_s;

typedef struct MAVG128_s
{
        int16_t     buffer[128];
        uint8_t     bufferpos;
        int64_t     sum;
        int64_t     avg;
}
MAVG128_s;

typedef struct MAVG256_s
{
        int16_t     buffer[256];
        uint16_t    bufferpos;
        int64_t     sum;
        int64_t     avg;
}
MAVG256_s;

typedef struct MAVG512_s
{
        int16_t     buffer[512];
        uint16_t    bufferpos;
        int64_t     sum;
        int64_t     avg;
}
MAVG512_s;

extern  void        MAVG8_INIT(MAVG8_s *ptr);
extern  int16_t     MAVG8_GET(MAVG8_s *ptr);
extern  int16_t     MAVG8_UPDATE(MAVG8_s *ptr, int16_t value);
extern  void        MAVG16_INIT(MAVG16_s *ptr);
extern  int16_t     MAVG16_GET(MAVG16_s *ptr);
extern  int16_t     MAVG16_UPDATE(MAVG16_s *ptr, int16_t value);
extern  void        MAVG32_INIT(MAVG32_s *ptr);
extern  int16_t     MAVG32_GET(MAVG32_s *ptr);
extern  int16_t     MAVG32_UPDATE(MAVG32_s *ptr, int16_t value);
extern  void        MAVG64_INIT(MAVG64_s *ptr);
extern  int16_t     MAVG64_GET(MAVG64_s *ptr);
extern  int16_t     MAVG64_UPDATE(MAVG64_s *ptr, int16_t value);
extern  void        MAVG128_INIT(MAVG128_s *ptr);
extern  int16_t     MAVG128_GET(MAVG128_s *ptr);
extern  int16_t     MAVG128_UPDATE(MAVG128_s *ptr, int16_t value);
extern  void        MAVG256_INIT(MAVG256_s *ptr);
extern  int16_t     MAVG256_GET(MAVG256_s *ptr);
extern  int16_t     MAVG256_UPDATE(MAVG256_s *ptr, int16_t value);
extern  void        MAVG512_INIT(MAVG512_s *ptr);
extern  int16_t     MAVG512_GET(MAVG512_s *ptr);
extern  int16_t     MAVG512_UPDATE(MAVG512_s *ptr, int16_t value);

#endif
