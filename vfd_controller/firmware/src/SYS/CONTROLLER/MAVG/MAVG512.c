#include "MAVG.h"

void MAVG512_INIT(MAVG512_s *ptr)
{
    uint16_t counter = 0;

    ptr->bufferpos = 0;
    ptr->avg = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 512; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

int16_t MAVG512_GET(MAVG512_s *ptr)
{
    return (int32_t)(ptr->avg);
}

int16_t MAVG512_UPDATE(MAVG512_s *ptr, int16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->avg = ptr->sum / 0x0200;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x0200 - 1);

    return (int32_t)(ptr->avg);
}
