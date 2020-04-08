#include "MEM.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/crc16.h>

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
    uint16_t crc_new = 0;
    uint16_t crc_old = 0;

    for(counter = 0; counter < size; counter++)
    {
        *ptr = (uint8_t)_eemem_read(eememOffset + counter);
        crc_new = _crc16_update(crc_new, (uint8_t)*ptr);
        ptr++;
    }

    crc_old = (uint8_t)_eemem_read(eememOffset + counter + 0);
    crc_old = crc_old << 8;
    crc_old = crc_old | ((uint8_t)_eemem_read(eememOffset + counter + 1));

    if(crc_old != crc_new)
    {
        return -1;
    }

    return 0;
}

void MEM_WRITE(uint16_t eememOffset, void *eememData, uint16_t size)
{
    uint16_t counter = 0;
    uint8_t *ptr = (uint8_t *)eememData;
    uint16_t crc = 0;

    for(counter = 0; counter < size; counter++)
    {
        _eemem_write(eememOffset + counter, (uint8_t)*ptr);
        crc = _crc16_update(crc, (uint8_t)*ptr);
        ptr++;
    }

    _eemem_write(eememOffset + counter + 0, (uint8_t)(crc >> 8));
    _eemem_write(eememOffset + counter + 1, (uint8_t)(crc >> 0));
}
