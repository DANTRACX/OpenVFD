#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "./RES/RS232/RS232.h"
#include "./RES/RS485/RS485.h"

#include "./SYS/REGISTRY/REGISTRY.h"
#include "./SYS/CONTROLLER/CONTROLLER.h"

int main(void)
{
    wdt_disable();
    sei();
    //RS232_INIT();
    //RS485_INIT();

    REGISTRY_INIT();
    // SERIAL_INIT();
    // MODBUS_INIT();
    CONTROLLER_INIT();

    while(1)
    {
        while(CONTROLLER_WAIT_CYCLE())
        {
            // do some modbus/serial fetching stuff
            // or calculate some register values
        }

        CONTROLLER_STEP_CYCLE();
    }

    return 0;
}
