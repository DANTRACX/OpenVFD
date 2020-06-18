#include "VRE.h"

void VRE2_INIT(VRE2_s *ptr)
{
    uint8_t counter = 0;

    ptr->bufferpos = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 16; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

uint16_t VRE2_GET(VRE2_s *ptr)
{
    return (uint16_t)(ptr->sum >> 2);
}

uint16_t VRE2_UPDATE(VRE2_s *ptr, uint16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x10 - 1);

    return (uint16_t)(ptr->sum >> 2);
}
