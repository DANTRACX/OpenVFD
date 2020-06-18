#include "VRE.h"

void VRE3_INIT(VRE3_s *ptr)
{
    uint8_t counter = 0;

    ptr->bufferpos = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 64; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

uint16_t VRE3_GET(VRE3_s *ptr)
{
    return (uint16_t)(ptr->sum >> 3);
}

uint16_t VRE3_UPDATE(VRE3_s *ptr, uint16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x40 - 1);

    return (uint16_t)(ptr->sum >> 3);
}
