#include "MAVG.h"

void MAVG256_INIT(MAVG256_s *ptr)
{
    uint16_t counter = 0;

    ptr->bufferpos = 0;
    ptr->avg = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 256; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

int16_t MAVG256_GET(MAVG256_s *ptr)
{
    return (int32_t)(ptr->avg);
}

int16_t MAVG256_UPDATE(MAVG256_s *ptr, int16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->avg = ptr->sum / 0x0100;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x0100 - 1);

    return (int32_t)(ptr->avg);
}
