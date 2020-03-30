#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "./RES/RS232/RS232.h"
#include "./RES/RS485/RS485.h"

#include "./SYS/REGISTRY/REGISTRY.h"
#include "./SYS/CONTROLLER/CONTROLLER.h"

#include "./RES/SENSE/SENSE.h"

int main(void)
{
    int16_t temp0, temp1;
    int16_t test;
    char output[2];

    wdt_disable();
    sei();
    RS232_INIT();
    SENSE_INIT();
    //RS485_INIT();

    // REGISTRY_INIT();
    // SERIAL_INIT();
    // MODBUS_INIT();
    //CONTROLLER_INIT();
    /*
    while(1)
    {
        while(CONTROLLER_WAIT_CYCLE())
        {
            // do some modbus/serial fetching stuff
            // or calculate some register values
        }

        CONTROLLER_STEP_CYCLE();
    }*/

    __builtin_avr_delay_cycles(16000000);
    //RS232_SEND("START\n\r", 7);

    while(1)
    {
        SENSE_FETCH((uint16_t *)&temp0, (uint16_t *)&temp1, (uint16_t *)&temp1, (uint16_t *)&temp1);
        test = (temp1) - (temp0);
        output[0] = (char)(test >> 8);
        output[1] = (char)(test >> 0);

        RS232_SEND(output, 2);
    }

    return 0;
}
