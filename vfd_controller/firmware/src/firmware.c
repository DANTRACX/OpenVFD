#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "./RES/RS232/RS232.h"
#include "./RES/RS485/RS485.h"
#include "./RES/SVPWM/SVPWM.h"
#include "./RES/SENSE/SENSE.h"

int main(void)
{
    wdt_disable();
    sei();
    RS232_INIT();
    SENSE_INIT();
    //RS485_INIT();
    //SVPWM_INIT();
    //SVPWM_START();

    //__builtin_avr_delay_cycles(60000000);

    //SVPWM_QUEUE_SET_PWM_FREQUENCY(KHZ_8);
    //SVPWM_QUEUE_SET_MAGNITUDE(0);
    //SVPWM_QUEUE_SET_FREQUENCY(1250);
    //SVPWM_QUEUE_SEND();

    //uint8_t counter = 0;
    uint16_t data0, data1, data2, data3;
    char temp[2];

    while(1)
    {
        __builtin_avr_delay_cycles(20000000);
        SENSE_FETCH(&data0, &data1, &data2, &data3);
        temp[0] = (char)(data0 >> 8);
        temp[1] = (char)(data0 >> 0);
        RS232_SEND(temp, 2);
        //SVPWM_QUEUE_SET_MAGNITUDE(counter);
        //SVPWM_QUEUE_SEND();
        //counter++;
    }

    return 0;
}
