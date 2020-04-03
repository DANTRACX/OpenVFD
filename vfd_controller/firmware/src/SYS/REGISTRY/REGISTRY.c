#include "REGISTRY.h"
#include <string.h>
#include "../../RES/MEM/MEM.h"

SETPOINTS_s SETPOINTS;
PROCESSVALUES_s PROCESSVALUES;
PARAMETERS_s PARAMETERS;

void REGISTRY_INIT(void)
{
    SETPOINTS.ENABLE = 0;
    SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE = 0;
    SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 0;
    SETPOINTS.ENABLE_PROGMODE = 0;
    SETPOINTS.TARGET_FREQUENCY = 0;
    SETPOINTS.TARGET_TORQUE = 0;
    SETPOINTS.REVERSAL = 0;

    PROCESSVALUES.DCBUS_VOLTAGE = 0;
    PROCESSVALUES.DCBUS_CURRENT = 0;
    PROCESSVALUES.MOTOR_VOLTAGE = 0;
    PROCESSVALUES.MOTOR_CURRENT = 0;
    PROCESSVALUES.MOTOR_FREQUENCY = 0;
    PROCESSVALUES.MOTOR_POWER = 0;

    PARAMETERS.DCBUS_NOMINAL_VOLTAGE = 0;
    PARAMETERS.MOTOR_NOMINAL_VOLTAGE = 0;
    PARAMETERS.MOTOR_MINIMAL_VOLTAGE = 0;
    PARAMETERS.MOTOR_NOMINAL_FREQUENCY = 0;
    PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY = 0;
    PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT = 0;
    PARAMETERS.MOTOR_NOMINAL_REV_CURRENT = 0;
    PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT = 0;
    PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT = 0;
    PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY = 0;
    PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY = 0;
    PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = 0;
    PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = 0;
    PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = 0;
    PARAMETERS.CONTROLLER_SAMPLETIME = 0;
    PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR = 0;
    PARAMETERS.CONTROLLER_INTEGRAL_FACTOR = 0;
    PARAMETERS.PWM_FREQUENCY = 0;
    PARAMETERS.PWM_DEADTIME = 0;
    PARAMETERS.PWM_DEADTIME_PRESCALING = 0;
    PARAMETERS.MODBUS_ADDRESS = 0;

    strcpy(PROCESSVALUES.SYSTEM_STATUS, "INITIALIZING");
    REGISTRY_READ();
    strcpy(PROCESSVALUES.SYSTEM_STATUS, "SYSTEM OK");
}


void REGISTRY_UPDATE(void)
{
    MEM_WRITE64( 0, (uint64_t)PARAMETERS.DCBUS_NOMINAL_VOLTAGE);
    MEM_WRITE64( 1, (uint64_t)PARAMETERS.MOTOR_NOMINAL_VOLTAGE);
    MEM_WRITE64( 2, (uint64_t)PARAMETERS.MOTOR_MINIMAL_VOLTAGE);
    MEM_WRITE64( 3, (uint64_t)PARAMETERS.MOTOR_NOMINAL_FREQUENCY);
    MEM_WRITE64( 4, (uint64_t)PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY);
    MEM_WRITE64( 5, (uint64_t)PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT);
    MEM_WRITE64( 6, (uint64_t)PARAMETERS.MOTOR_NOMINAL_REV_CURRENT);
    MEM_WRITE64( 7, (uint64_t)PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT);
    MEM_WRITE64( 8, (uint64_t)PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT);

    MEM_WRITE64( 9, (uint64_t)PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY);
    MEM_WRITE64(10, (uint64_t)PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY);
    MEM_WRITE64(11, (uint64_t)PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR);
    MEM_WRITE64(12, (uint64_t)PARAMETERS.CONTROLLER_INTEGRAL_FACTOR);
    MEM_WRITE64(13, (uint64_t)PARAMETERS.CONTROLLER_SAMPLETIME);
    MEM_WRITE64(14, (uint64_t)PARAMETERS.CONTROLLER_ENABLE_TIMEOUT);
    MEM_WRITE64(15, (uint64_t)PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT);
    MEM_WRITE64(16, (uint64_t)PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN);

    MEM_WRITE64(17, (uint64_t)PARAMETERS.PWM_FREQUENCY);
    MEM_WRITE64(18, (uint64_t)PARAMETERS.PWM_DEADTIME);
    MEM_WRITE64(19, (uint64_t)PARAMETERS.PWM_DEADTIME_PRESCALING);

    MEM_WRITE64(20, (uint64_t)PARAMETERS.MODBUS_ADDRESS);
}

void REGISTRY_READ(void)
{
    uint64_t temp[10];
    uint8_t counter = 0;

    for(counter = 0; counter < 9; counter++)
    {
        if(MEM_READ64(counter, &temp[counter]) < 0)
        {
            strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P4030X");
            PROCESSVALUES.SYSTEM_STATUS[11] = counter + 0x30;
            return;
        }
    }

    PARAMETERS.DCBUS_NOMINAL_VOLTAGE = temp[0];
    PARAMETERS.MOTOR_NOMINAL_VOLTAGE = temp[1];
    PARAMETERS.MOTOR_MINIMAL_VOLTAGE = temp[2];
    PARAMETERS.MOTOR_NOMINAL_FREQUENCY = temp[3];
    PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY = temp[4];
    PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT = temp[5];
    PARAMETERS.MOTOR_NOMINAL_REV_CURRENT = temp[6];
    PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT = temp[7];
    PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT = temp[8];

    for(counter = 0; counter < 8; counter++)
    {
        if(MEM_READ64(9 + counter, &temp[counter]) < 0)
        {
            if(counter == 4)
            {
                strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P40314");
            }

            else if(counter == 5)
            {
                strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P40322");
            }

            else if(counter == 6)
            {
                strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P40330");
            }

            else if (counter == 7)
            {
                strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P40338");
            }

            else
            {
                strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P4031X");
                PROCESSVALUES.SYSTEM_STATUS[11] = counter + 0x30;
            }

            return;
        }
    }

    PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY = temp[0];
    PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY = temp[1];
    PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR = temp[2];
    PARAMETERS.CONTROLLER_INTEGRAL_FACTOR = temp[3];
    PARAMETERS.CONTROLLER_SAMPLETIME = temp[4];
    PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = temp[5];
    PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = temp[6];
    PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = temp[7];

    for(counter = 0; counter < 3; counter++)
    {
        if(MEM_READ64(8 + 9 + counter, &temp[counter]) < 0)
        {
            strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P4035X");
            PROCESSVALUES.SYSTEM_STATUS[11] = counter + 0x30;
            return;
        }
    }

    PARAMETERS.PWM_FREQUENCY = temp[0];
    PARAMETERS.PWM_DEADTIME = temp[1];
    PARAMETERS.PWM_DEADTIME_PRESCALING = temp[2];

    if(MEM_READ64(8 + 9 + 3, &temp[0]) < 0)
    {
        strcpy(PROCESSVALUES.SYSTEM_STATUS, "ERROR P40360");
        return;
    }

    PARAMETERS.MODBUS_ADDRESS = temp[0];
}
