#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

int main(void)
{
    wdt_disable();
    sei();
    DDRD  |= (1 << PD6);
    DDRB |= (1 << DDB4) | (1 << DDB5) | (1 << DDB7);
    DDRB &= ~((1 << DDB6));
    PORTB &= ~((1 << PB5) | (1 << PB7));
    PORTB |= ( 1 << PB4);

    SPCR = 0b01010011;
    SPSR = 0b00000000;

    __builtin_avr_delay_cycles(60000000);
    PORTD |= (1 << PD6);
    __builtin_avr_delay_cycles(60000000);
    PORTB &= ~( 1 << PB4);
    __builtin_avr_delay_cycles(100000);
    SPDR = 0x02;
    while(!(SPSR & (1<<SPIF)));
    SPDR = 0x50;

    while(!(SPSR & (1<<SPIF)));
    SPDR = 0x03;
    while(!(SPSR & (1<<SPIF)));
    SPDR = 0x00;
    while(!(SPSR & (1<<SPIF)));
    SPDR = 0x00;

    while(!(SPSR & (1<<SPIF)));
    SPDR = 0x05;
    while(!(SPSR & (1<<SPIF)));
    SPDR = 0x03;

    while(!(SPSR & (1<<SPIF)));
    PORTB |= ( 1 << PB4);

    __builtin_avr_delay_cycles(100000);

    uint16_t counter = 1;
    int8_t temp = 1;

    while(1)
    {
        PORTB &= ~( 1 << PB4);
        __builtin_avr_delay_cycles(10000);

        while(!(SPSR & (1<<SPIF)));
        SPDR = 0x05;
        while(!(SPSR & (1<<SPIF)));
        SPDR = 0x02;

        while(!(SPSR & (1<<SPIF)));
        PORTB |= ( 1 << PB4);

        __builtin_avr_delay_cycles(10000);

        temp = 1;
        do
        {
            PORTB &= ~( 1 << PB4);
            __builtin_avr_delay_cycles(10000);

            while(!(SPSR & (1<<SPIF)));
            SPDR = 0x03;
            while(!(SPSR & (1<<SPIF)));
            SPDR = ((uint8_t)counter);
            while(!(SPSR & (1<<SPIF)));
            SPDR = ((uint8_t)((counter >> 8) & 0xFF));

            while(!(SPSR & (1<<SPIF)));
            PORTB |= ( 1 << PB4);

            __builtin_avr_delay_cycles(10000);
            counter = counter + temp;
            if(counter == 65535)
            {
                temp = -1;
            }
        }
        while(counter != 0);
        temp = 1;

        PORTB &= ~( 1 << PB4);
        __builtin_avr_delay_cycles(10000);

        while(!(SPSR & (1<<SPIF)));
        SPDR = 0x05;
        while(!(SPSR & (1<<SPIF)));
        SPDR = 0x05;

        while(!(SPSR & (1<<SPIF)));
        PORTB |= ( 1 << PB4);

        __builtin_avr_delay_cycles(10000);

        do
        {
            PORTB &= ~( 1 << PB4);
            __builtin_avr_delay_cycles(10000);

            while(!(SPSR & (1<<SPIF)));
            SPDR = 0x03;
            while(!(SPSR & (1<<SPIF)));
            SPDR = ((uint8_t)counter);
            while(!(SPSR & (1<<SPIF)));
            SPDR = ((uint8_t)((counter >> 8) & 0xFF));

            while(!(SPSR & (1<<SPIF)));
            PORTB |= ( 1 << PB4);

            __builtin_avr_delay_cycles(10000);
            counter = counter + 1;
            if(counter == 65535)
            {
                temp = -1;
            }
        }
        while(counter != 0);
    }

    return 0;
}
