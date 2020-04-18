#include "MODBUS.h"
#include "../REGISTRY/REGISTRY.h"
#include "../../RES/RS485/RS485.h"

static struct MODBUSSTATES_s
{
}
MODBUSSTATES;

void MODBUS_INIT(void)
{
    RS485_INIT(PARAMETERS.MODBUS_ADDRESS, PARAMETERS.MODBUS_BAUDRATE, PARAMETERS.MODBUS_PARITY);
}

void MODBUS_PROCESS(void)
{
    /* check for data in receiver buffer */
    if(!(RS485_RXSIZE()))
    {
        return;
    }

    // interpret frame and send response
}
