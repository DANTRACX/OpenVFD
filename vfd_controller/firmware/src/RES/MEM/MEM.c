#include "MEM.h"

int8_t MEM_READ64(uint8_t int64addr, uint64_t *data)
{
    uint8_t counter = 0;
    uint16_t primary_memaddr = ((uint16_t)int64addr) * 8;
    uint64_t primary_data = 0;
    uint64_t secondary_data = 0;
    uint16_t secondary_memaddr = 2048 + primary_memaddr;

    cli();

    for(counter = 0; counter < 8; counter++)
    {
        primary_data = primary_data << 8;

        while(EECR & (1 << EEPE));

        EEARL = (uint8_t)(primary_memaddr >> 0);
        EEARH = (uint8_t)(primary_memaddr >> 8);
        EECR |= (1 << EERE);

        primary_data = primary_data | EEDR;
        primary_memaddr++;
    }

    for(counter = 0; counter < 8; counter++)
    {
        secondary_data = secondary_data << 8;

        while(EECR & (1 << EEPE));

        EEARL = (uint8_t)(secondary_memaddr >> 0);
        EEARH = (uint8_t)(secondary_memaddr >> 8);
        EECR |= (1 << EERE);

        secondary_data = secondary_data | EEDR;
        secondary_memaddr++;
    }

    sei();

    if(primary_data != secondary_data)
    {
        *data = 0;
        return -1;
    }

    *data = primary_data;
    return 0;
}

uint8_t MEM_WRITE64(uint8_t int64addr, uint64_t data)
{
    uint8_t counter = 0;
    uint16_t primary_memaddr = ((uint16_t)int64addr) * 8;
    uint64_t primary_data = 0;
    uint64_t secondary_data = 0;
    uint16_t secondary_memaddr = 2048 + primary_memaddr;

    cli();

    for(counter = 0; counter < 8; counter++)
    {
        primary_data = primary_data << 8;

        while(EECR & (1 << EEPE));

        EEARL = (uint8_t)(primary_memaddr >> 0);
        EEARH = (uint8_t)(primary_memaddr >> 8);
        EECR |= (1 << EERE);

        primary_data = primary_data | EEDR;
        primary_memaddr++;
    }

    if(primary_data != data)
    {
        primary_data = data;

        for(counter = 0; counter < 8; counter++)
        {
            while(EECR & (1 << EEPE));

            EEARL = (uint8_t)(primary_memaddr >> 0);
            EEARH = (uint8_t)(primary_memaddr >> 8);
            EEDR = (uint8_t)primary_data;
            EECR |= (1 << EEMPE);
            EECR |= (1 << EEPE);

            primary_data = primary_data >> 8;
            primary_memaddr--;
        }
    }

    for(counter = 0; counter < 8; counter++)
    {
        secondary_data = secondary_data << 8;

        while(EECR & (1 << EEPE));

        EEARL = (uint8_t)(secondary_memaddr >> 0);
        EEARH = (uint8_t)(secondary_memaddr >> 8);
        EECR |= (1 << EERE);

        secondary_data = secondary_data | EEDR;
        secondary_memaddr++;
    }

    if(secondary_data != data)
    {
        secondary_data = data;

        for(counter = 0; counter < 8; counter++)
        {
            while(EECR & (1 << EEPE));

            EEARL = (uint8_t)(secondary_memaddr >> 0);
            EEARH = (uint8_t)(secondary_memaddr >> 8);
            EEDR = (uint8_t)secondary_data;
            EECR |= (1 << EEMPE);
            EECR |= (1 << EEPE);

            secondary_data = secondary_data >> 8;
            secondary_memaddr--;
        }
    }

    sei();
    return 0;
}
