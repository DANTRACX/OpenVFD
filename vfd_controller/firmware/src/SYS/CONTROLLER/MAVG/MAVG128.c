#include "MAVG.h"

void MAVG128_INIT(MAVG128_s *ptr)
{
    uint8_t counter = 0;

    ptr->bufferpos = 0;
    ptr->avg = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 128; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

int16_t MAVG128_GET(MAVG128_s *ptr)
{
    return (int32_t)(ptr->avg);
}

int16_t MAVG128_UPDATE(MAVG128_s *ptr, int16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->avg = ptr->sum / 0x80;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x80 - 1);

    return (int32_t)(ptr->avg);
}
