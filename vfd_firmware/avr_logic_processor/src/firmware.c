#include <avr/io.h>

int main(void)
{
    DDRB = 0x01;
    PORTB = 0x01;
    
    while(1)
    {
    }

    return 0;
}
