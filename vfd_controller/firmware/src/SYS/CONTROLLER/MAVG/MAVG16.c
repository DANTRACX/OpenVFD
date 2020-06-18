#include "MAVG.h"

void MAVG16_INIT(MAVG16_s *ptr)
{
    uint8_t counter = 0;

    ptr->bufferpos = 0;
    ptr->avg = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 16; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

int16_t MAVG16_GET(MAVG16_s *ptr)
{
    return (int32_t)(ptr->avg);
}

int16_t MAVG16_UPDATE(MAVG16_s *ptr, int16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->avg = ptr->sum / 0x10;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x10 - 1);

    return (int32_t)(ptr->avg);
}
