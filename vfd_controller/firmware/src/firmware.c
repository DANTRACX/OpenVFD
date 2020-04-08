#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "./SYS/SCPI/SCPI.h"
#include "./SYS/MODBUS/MODBUS.h"
#include "./SYS/REGISTRY/REGISTRY.h"
#include "./SYS/CONTROLLER/CONTROLLER.h"

int main(void)
{
    wdt_enable(WDTO_1S);
    wdt_reset();
    sei();

    REGISTRY_INIT();
    wdt_reset();
    SCPI_INIT();
    wdt_reset();
    MODBUS_INIT();
    wdt_reset();
    CONTROLLER_INIT();
    wdt_reset();

    while(1)
    {
        while(CONTROLLER_WAIT_CYCLE())
        {
                wdt_reset();
        }

        SCPI_PROCESS();
        CONTROLLER_STEP_CYCLE();
    }

    return 0;
}
