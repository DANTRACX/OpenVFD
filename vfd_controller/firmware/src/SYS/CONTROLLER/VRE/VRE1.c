#include "VRE.h"

void VRE1_INIT(VRE1_s *ptr)
{
    uint8_t counter = 0;

    ptr->bufferpos = 0;
    ptr->sum = 0;

    for(counter = 0; counter < 4; counter++)
    {
        ptr->buffer[counter] = 0;
    }
}

uint16_t VRE1_GET(VRE1_s *ptr)
{
    return (uint16_t)(ptr->sum >> 1);
}

uint16_t VRE1_UPDATE(VRE1_s *ptr, uint16_t value)
{
    ptr->sum = ptr->sum - ptr->buffer[ptr->bufferpos] + value;
    ptr->buffer[ptr->bufferpos] = value;
    ptr->bufferpos = (ptr->bufferpos + 1) & (0x04 - 1);

    return (uint16_t)(ptr->sum >> 1);
}
