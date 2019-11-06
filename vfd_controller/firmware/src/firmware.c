#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

ISR(SPI_STC_vect)
{
    PORTB |= ( 1 << PB4);
}

int main(void)
{
    wdt_disable();
    sei();

    DDRB |= (1 << DDB4) | (1 << DDB5) | (1 << DDB7);
    DDRB &= ~((1 << DDB6));
    PORTB &= ~((1 << PB5) | (1 << PB7));
    PORTB |= ( 1 << PB4);

    SPCR = 0b11010011;
    SPSR = 0b00000000;

    while(1)
    {
        __builtin_avr_delay_cycles(20000000);
        PORTB &= ~( 1 << PB4);
        __builtin_avr_delay_cycles(10000);
        SPDR = 0x05;

        __builtin_avr_delay_cycles(20000000);
        PORTB &= ~( 1 << PB4);
        __builtin_avr_delay_cycles(10000);
        SPDR = 0x04;
    }

    return 0;
}
