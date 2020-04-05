#include "MEM.h"
#include "../RS232/RS232.h"

int8_t MEM_READ64(uint8_t int64addr, uint64_t *data)
{
    uint8_t counter = 0;
    uint16_t primary_memaddr = ((uint16_t)int64addr) * 8;
    uint64_t primary_data = 0;
    uint64_t secondary_data = 0;
    uint16_t secondary_memaddr = 2048 + primary_memaddr;

    /* disable all interrupts during eeprom read */
    cli();

    /* read all 8 bytes for addressed uint64 in primary storage space */
    for(counter = 0; counter < 8; counter++)
    {
        /* shift received values to the left */
        primary_data = primary_data << 8;

        /* wait for eeprom ready */
        while(EECR & (1 << EEPE));

        /* set cell address and start read */
        EEARL = (uint8_t)(primary_memaddr >> 0);
        EEARH = (uint8_t)(primary_memaddr >> 8);
        EECR |= (1 << EERE);

        /* concat read value and increment cell pointer */
        primary_data = primary_data | EEDR;
        primary_memaddr++;
    }

    /* read all 8 bytes for addressed uint64 in secondary storage space */
    for(counter = 0; counter < 8; counter++)
    {
        /* shift received values to the left */
        secondary_data = secondary_data << 8;

        /* wait for eeprom ready */
        while(EECR & (1 << EEPE));

        /* set cell address and start read */
        EEARL = (uint8_t)(secondary_memaddr >> 0);
        EEARH = (uint8_t)(secondary_memaddr >> 8);
        EECR |= (1 << EERE);

        /* concat read value and increment cell pointer */
        secondary_data = secondary_data | EEDR;
        secondary_memaddr++;
    }

    /* reenable all interrupts */
    sei();

    /* compare primary with secondary data - return error on missmatch */
    if(primary_data != secondary_data)
    {
        *data = 0;
        return -1;
    }

    /* return valid uint64_t value */
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

    /* disable all interrupts during eeprom read */
    cli();

    /* read all 8 bytes for addressed uint64 in primary storage space */
    for(counter = 0; counter < 8; counter++)
    {
        /* shift received values to the left */
        primary_data = primary_data << 8;

        /* wait for eeprom ready */
        while(EECR & (1 << EEPE));

        /* set cell address and start read */
        EEARL = (uint8_t)(primary_memaddr >> 0);
        EEARH = (uint8_t)(primary_memaddr >> 8);
        EECR |= (1 << EERE);

        /* concat read value and increment cell pointer */
        primary_data = primary_data | EEDR;
        primary_memaddr++;
    }

    /* only overwrite cell if data is different */
    if(primary_data != data)
    {
        /* copy new data to shifting variable */
        primary_data = data;

        /* write all 8 bytes to addressed cell space */
        for(counter = 0; counter < 8; counter++)
        {
            /* decrement cell pointer */
            primary_memaddr--;

            /* wait for eeprom ready */
            while(EECR & (1 << EEPE));

            /* set cell address, data and trigger cell write */
            EEARL = (uint8_t)(primary_memaddr >> 0);
            EEARH = (uint8_t)(primary_memaddr >> 8);
            EEDR = (uint8_t)primary_data;
            EECR |= (1 << EEMPE);
            EECR |= (1 << EEPE);

            /* shift data register to the right */
            primary_data = primary_data >> 8;
        }
    }

    /* read all 8 bytes for addressed uint64 in secondary storage space */
    for(counter = 0; counter < 8; counter++)
    {
        /* shift received values to the left */
        secondary_data = secondary_data << 8;

        /* wait for eeprom ready */
        while(EECR & (1 << EEPE));

        /* set cell address and start read */
        EEARL = (uint8_t)(secondary_memaddr >> 0);
        EEARH = (uint8_t)(secondary_memaddr >> 8);
        EECR |= (1 << EERE);

        /* concat read value and increment cell pointer */
        secondary_data = secondary_data | EEDR;
        secondary_memaddr++;
    }

    /* only overwrite cell if data is different */
    if(secondary_data != data)
    {
        /* copy new data to shifting variable */
        secondary_data = data;

        /* write all 8 bytes to addressed cell space */
        for(counter = 0; counter < 8; counter++)
        {
            /* decrement cell pointer */
            secondary_memaddr--;

            /* wait for eeprom ready */
            while(EECR & (1 << EEPE));

            /* set cell address, data and trigger cell write */
            EEARL = (uint8_t)(secondary_memaddr >> 0);
            EEARH = (uint8_t)(secondary_memaddr >> 8);
            EEDR = (uint8_t)secondary_data;
            EECR |= (1 << EEMPE);
            EECR |= (1 << EEPE);

            /* shift data register to the right */
            secondary_data = secondary_data >> 8;
        }
    }

    /* reenable all interrupts and return */
    sei();
    return 0;
}
