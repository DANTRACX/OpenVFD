#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "./SYS/SCPI/SCPI.h"
#include "./SYS/MODBUS/MODBUS.h"
#include "./SYS/REGISTRY/REGISTRY.h"
#include "./SYS/CONTROLLER/CONTROLLER.h"


int main(void)
{
    wdt_disable();
    sei();

    REGISTRY_INIT();
    SCPI_INIT();
    MODBUS_INIT();
    CONTROLLER_INIT();

    while(1)
    {
        while(CONTROLLER_WAIT_CYCLE());

        SCPI_PROCESS();
        CONTROLLER_STEP_CYCLE();
    }

    return 0;
}
