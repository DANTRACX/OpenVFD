#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "./RES/SVPWM/SVPWM.h"

int main(void)
{
    wdt_disable();
    sei();
    SVPWM_INIT();
    SVPWM_START();

    __builtin_avr_delay_cycles(60000000);

    SVPWM_QUEUE_SET_PWM_FREQUENCY(KHZ_8);
    SVPWM_QUEUE_SET_MAGNITUDE(128);
    SVPWM_QUEUE_SET_FREQUENCY(0);
    SVPWM_QUEUE_SEND();

    uint16_t counter = 0;

    while(1)
    {
        __builtin_avr_delay_cycles(200);
        SVPWM_QUEUE_SET_PHASE(counter);
        SVPWM_QUEUE_SEND();

        counter++;
        if(counter > 15624)
        {
            counter = 0;
        }
    }


    return 0;
}
