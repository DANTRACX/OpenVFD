#include "VRE.h"

void VRE5_INIT(VRE5_s *ptr)
{
    uint16_t counter = 0;

    ptr->bufferpos = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 1024; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

int16_t VRE5_GET(VRE5_s *ptr)
{
    return (uint16_t)(ptr->sum >> 5);
}

int16_t VRE5_UPDATE(VRE5_s *ptr, int16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x0400 - 1);

    return (int16_t)(ptr->sum >> 5);
}
