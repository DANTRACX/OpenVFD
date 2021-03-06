#include "MEM.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

__attribute__((always_inline))
static inline uint8_t _eemem_read(uint16_t eememAddr)
{
    /* disable all interrupts during eeprom read */
    cli();
    wdt_reset();

    /* wait for eeprom ready */
    while(EECR & (1 << EEPE));

    /* set cell address and start read */
    EEARL = (uint8_t)(eememAddr >> 0);
    EEARH = (uint8_t)(eememAddr >> 8);
    EECR |= (1 << EERE);

    /* reenable all interrupts and return data */
    sei();
    return EEDR;
}

__attribute__((always_inline))
static inline void _eemem_write(uint16_t eememAddr, uint8_t eememData)
{
    /* disable all interrupts during eeprom write */
    cli();
    wdt_reset();

    /* check old cell data - only overwrite if not the same */
    if(_eemem_read(eememAddr) != eememData)
    {
        /* wait for eeprom ready */
        while(EECR & (1 << EEPE));

        /* set cell address, data and trigger cell write */
        EEARL = (uint8_t)(eememAddr >> 0);
        EEARH = (uint8_t)(eememAddr >> 8);
        EEDR = (uint8_t)eememData;
        EECR |= (1 << EEMPE);
        EECR |= (1 << EEPE);
    }

    /* reenable all interrupts and return */
    sei();
}

int8_t MEM_READ(uint16_t eememOffset, void *eememData, uint16_t size)
{
    uint8_t *ptr = (uint8_t *)eememData;
    uint16_t counter = 0;
    char crcH_old = 0;
    char crcL_old = 0;
    char crcH_new = 0;
    char crcL_new = 0;

    for(counter = 0; counter < size; counter++)
    {
        *ptr = (uint8_t)_eemem_read(eememOffset + counter);
        ptr++;
    }

    crcH_old = (char)_eemem_read(eememOffset + counter + 0);
    crcL_old = (char)_eemem_read(eememOffset + counter + 1);
    MEM_CRC16((char *)eememData, size, &crcL_new, &crcH_new);

    if((crcL_old != crcL_new) || (crcH_old != crcH_new))
    {
        return -1;
    }

    return 0;
}

void MEM_WRITE(uint16_t eememOffset, void *eememData, uint16_t size)
{
    uint16_t counter = 0;
    uint8_t *ptr = (uint8_t *)eememData;
    char crcH = 0;
    char crcL = 0;

    for(counter = 0; counter < size; counter++)
    {
        _eemem_write(eememOffset + counter, (uint8_t)*ptr);
        ptr++;
    }

    MEM_CRC16((char *)eememData, size, &crcL, &crcH);
    _eemem_write(eememOffset + counter + 0, (uint8_t)crcH);
    _eemem_write(eememOffset + counter + 1, (uint8_t)crcL);
}
