#include "SENSE.h"
#include "./MAVG/MAVG.h"

#ifdef SENSE_USE_CHANNEL_1
    static MAVG_s filtered_sense1;
#endif
#ifdef SENSE_USE_CHANNEL_2
    static MAVG_s filtered_sense2;
#endif
#ifdef SENSE_USE_CHANNEL_3
    static MAVG_s filtered_sense3;
#endif
#ifdef SENSE_USE_CHANNEL_4
    static MAVG_s filtered_sense4;
#endif

void SENSE_INIT(void)
{
    /* setup sense input pins */
    DDRC  &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));
    PORTC &= ~((1 << PC7) | (1 << PC6) | (1 << PC5) | (1 << PC4));

    /* setup clock (idle low) and ~cs/shdn */
    DDRD  |=  ((1 << PD5) | (1 << PD7));
    PORTD &= ~((1 << PD5));
    PORTD |=  ((1 << PD7));

    /* initialize filter */
    #ifdef SENSE_USE_CHANNEL_1
        MAVG_INIT(&filtered_sense1);
    #endif
    #ifdef SENSE_USE_CHANNEL_2
        MAVG_INIT(&filtered_sense2);
    #endif
    #ifdef SENSE_USE_CHANNEL_3
        MAVG_INIT(&filtered_sense3);
    #endif
    #ifdef SENSE_USE_CHANNEL_4
        MAVG_INIT(&filtered_sense4);
    #endif
}

void SENSE_FETCH(uint16_t *sense1, uint16_t *sense2, uint16_t *sense3, uint16_t *sense4)
{
    uint8_t counter = 0;

    *sense1 = 0x0000;
    *sense2 = 0x0000;
    *sense3 = 0x0000;
    *sense4 = 0x0000;

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
        #ifdef SENSE_USE_CHANNEL_1
            *sense1 <<= 1;

            if(PINC & (1 << PC4))
            {
                *sense1 |= (1 << 0);
            }
        #endif
        #ifdef SENSE_USE_CHANNEL_2
            *sense2 <<= 1;

            if(PINC & (1 << PC5))
            {
                *sense2 |= (1 << 0);
            }
        #endif
        #ifdef SENSE_USE_CHANNEL_3
            *sense3 <<= 1;

            if(PINC & (1 << PC6))
            {
                *sense3 |= (1 << 0);
            }
        #endif
        #ifdef SENSE_USE_CHANNEL_4
            *sense4 <<= 1;

            if(PINC & (1 << PC7))
            {
                *sense4 |= (1 << 0);
            }
        #endif
    }

    /* shutdown sensor chips */
    PORTD |= ((1 << PD7));

    /* filter data */
    #ifdef SENSE_USE_CHANNEL_1
        *sense1 = MAVG_UPDATE(&filtered_sense1, (*sense1 & 0x0FFF));
    #endif
    #ifdef SENSE_USE_CHANNEL_2
        *sense2 = MAVG_UPDATE(&filtered_sense2, (*sense2 & 0x0FFF));
    #endif
    #ifdef SENSE_USE_CHANNEL_3
        *sense3 = MAVG_UPDATE(&filtered_sense3, (*sense3 & 0x0FFF));
    #endif
    #ifdef SENSE_USE_CHANNEL_4
        *sense4 = MAVG_UPDATE(&filtered_sense4, (*sense4 & 0x0FFF));
    #endif
}
