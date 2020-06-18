#ifndef __VRE_H__
#define __VRE_H__

#include <stdint.h>

typedef struct VRE1_s
{
        uint16_t    buffer[4];
        uint8_t     bufferpos;
        uint64_t    sum;
}
VRE1_s;

typedef struct VRE2_s
{
        uint16_t    buffer[16];
        uint8_t     bufferpos;
        uint64_t    sum;
}
VRE2_s;

typedef struct VRE3_s
{
        uint16_t    buffer[64];
        uint8_t     bufferpos;
        uint64_t    sum;
}
VRE3_s;

typedef struct VRE4_s
{
        uint16_t    buffer[256];
        uint16_t    bufferpos;
        uint64_t    sum;
        uint64_t    avg;
}
VRE4_s;

typedef struct VRE5_s
{
        int16_t     buffer[1024];
        uint16_t    bufferpos;
        int64_t     sum;
}
VRE5_s;

extern  void        VRE1_INIT(VRE1_s *ptr);
extern  uint16_t    VRE1_GET(VRE1_s *ptr);
extern  uint16_t    VRE1_UPDATE(VRE1_s *ptr, uint16_t value);
extern  void        VRE2_INIT(VRE2_s *ptr);
extern  uint16_t    VRE2_GET(VRE2_s *ptr);
extern  uint16_t    VRE2_UPDATE(VRE2_s *ptr, uint16_t value);
extern  void        VRE3_INIT(VRE3_s *ptr);
extern  uint16_t    VRE3_GET(VRE3_s *ptr);
extern  uint16_t    VRE3_UPDATE(VRE3_s *ptr, uint16_t value);
extern  void        VRE4_INIT(VRE4_s *ptr);
extern  uint16_t    VRE4_GET(VRE4_s *ptr);
extern  uint16_t    VRE4_UPDATE(VRE4_s *ptr, uint16_t value);
extern  void        VRE5_INIT(VRE5_s *ptr);
extern  int16_t     VRE5_GET(VRE5_s *ptr);
extern  int16_t     VRE5_UPDATE(VRE5_s *ptr, int16_t value);

#endif
