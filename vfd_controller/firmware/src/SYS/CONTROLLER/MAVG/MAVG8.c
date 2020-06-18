#include "MAVG.h"

void MAVG8_INIT(MAVG8_s *ptr)
{
    uint8_t counter = 0;

    ptr->bufferpos = 0;
    ptr->avg = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 8; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

int16_t MAVG8_GET(MAVG8_s *ptr)
{
    return (int32_t)(ptr->avg);
}

int16_t MAVG8_UPDATE(MAVG8_s *ptr, int16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->avg = ptr->sum / 0x08;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x08 - 1);

    return (int32_t)(ptr->avg);
}
