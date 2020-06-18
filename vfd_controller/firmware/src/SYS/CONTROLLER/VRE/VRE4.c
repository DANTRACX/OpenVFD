#include "VRE.h"

void VRE4_INIT(VRE4_s *ptr)
{
    uint16_t counter = 0;

    ptr->bufferpos = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 256; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

uint16_t VRE4_GET(VRE4_s *ptr)
{
    return (uint16_t)(ptr->sum >> 4);
}

uint16_t VRE4_UPDATE(VRE4_s *ptr, uint16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x01FF - 1);

    return (uint16_t)(ptr->sum >> 4);
}
