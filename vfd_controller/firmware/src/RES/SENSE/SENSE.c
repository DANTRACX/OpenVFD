#include "SENSE.h"

void SENSE_INIT(void)
{
    /* setup sense input pins */
    DDRC  &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));
    PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));

    /* setup clock (idle low) and ~cs/shdn */
    DDRD  |=  ((1 << PD5) | (1 << PD7));
    PORTD &= ~((1 << PD5));
    PORTD |=  ((1 << PD7));
}

void SENSE_FETCH(uint16_t *sense1, uint16_t *sense2, uint16_t *sense3, uint16_t *sense4)
{
    uint8_t counter = 0;
    uint16_t temp1 = 0x0000;
    uint16_t temp2 = 0x0000;
    uint16_t temp3 = 0x0000;
    uint16_t temp4 = 0x0000;

    /* enable sensor chips and start conversion */
    PORTD &= ~((1 << PD5) | (1 << PD7));
    __builtin_avr_delay_cycles(10);
    PORTD |=  ((1 << PD5));
    __builtin_avr_delay_cycles(10);
    PORTD &= ~((1 << PD5));
    __builtin_avr_delay_cycles(10);
    PORTD |=  ((1 << PD5));
    __builtin_avr_delay_cycles(10);

    /* read bit per bit into sensor data buffer */
    for(counter = 0; counter < 13; counter++)
    {
        /* shift a new bit to input */
        PORTD &= ~((1 << PD5));
        __builtin_avr_delay_cycles(10);
        PORTD |=  ((1 << PD5));

        /* shift data buffer by 1 position to the left */
        temp1 <<= 1;
        temp2 <<= 1;
        temp3 <<= 1;
        temp4 <<= 1;

        /* read current pin status and set bits accordingly */
        if(PINC & (1 << PC4))
        {
            temp1 |= (1 << 0);
        }

        if(PINC & (1 << PC5))
        {
            temp2 |= (1 << 0);
        }

        if(PINC & (1 << PC6))
        {
            temp3 |= (1 << 0);
        }

        if(PINC & (1 << PC7))
        {
            temp4 |= (1 << 0);
        }
    }

    /* shutdown sensor chips */
    PORTD |= ((1 << PD7));

    /* remove unkown/error data */
    *sense1 = temp1 & 0x0FFF;
    *sense2 = temp2 & 0x0FFF;
    *sense3 = temp3 & 0x0FFF;
    *sense4 = temp4 & 0x0FFF;
}
