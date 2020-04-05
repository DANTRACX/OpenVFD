#include "REGISTRY.h"

uint8_t REGISTRY_REGREAD(uint16_t registerAddress, int64_t *registerValue)
{
    switch(registerAddress)
    {
        case 40100:
        {
            *registerValue = (int64_t)SETPOINTS.ENABLE;
            return 0;
        }
        case 40101:
        {
            *registerValue = (int64_t)SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE;
            return 0;
        }
        case 40102:
        {
            *registerValue = (int64_t)SETPOINTS.ENABLE_CURRENT_OVERDRIVE;
            return 0;
        }
        case 40103:
        {
            *registerValue = (int64_t)SETPOINTS.ENABLE_PROGMODE;
            return 0;
        }
        case 40104:
        {
            *registerValue = (int64_t)SETPOINTS.TARGET_FREQUENCY;
            return 0;
        }
        case 40105:
        {
            *registerValue = (int64_t)SETPOINTS.TARGET_TORQUE;
            return 0;
        }
        case 40106:
        {
            *registerValue = (int64_t)SETPOINTS.REVERSAL;
            return 0;
        }
        case 30200:
        {
            *registerValue = (int64_t)PROCESSVALUES.DCBUS_VOLTAGE;
            return 0;
        }
        case 30201:
        {
            *registerValue = (int64_t)PROCESSVALUES.DCBUS_CURRENT;
            return 0;
        }
        case 30202:
        {
            *registerValue = (int64_t)PROCESSVALUES.MOTOR_VOLTAGE;
            return 0;
        }
        case 30203:
        {
            *registerValue = (int64_t)PROCESSVALUES.MOTOR_CURRENT;
            return 0;
        }
        case 30204:
        {
            *registerValue = (int64_t)PROCESSVALUES.MOTOR_FREQUENCY;
            return 0;
        }
        case 30205:
        {
            *registerValue = (int64_t)PROCESSVALUES.MOTOR_POWER;
            return 0;
        }
        case 30250:
        {
            *registerValue = (int64_t)PROCESSVALUES.SYSTEM_ERROR;
            return 0;
        }
        case 40300:
        {
            *registerValue = (int64_t)PARAMETERS.DCBUS_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40301:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40302:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_MINIMAL_VOLTAGE;
            return 0;
        }
        case 40303:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40304:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY;
            return 0;
        }
        case 40305:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT;
            return 0;
        }
        case 40306:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_REV_CURRENT;
            return 0;
        }
        case 40307:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT;
            return 0;
        }
        case 40308:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT;
            return 0;
        }
        case 40310:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40311:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY;
            return 0;
        }
        case 40312:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR;
            return 0;
        }
        case 40313:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_INTEGRAL_FACTOR;
            return 0;
        }
        case 40314:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_SAMPLETIME;
            return 0;
        }
        case 40322:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_ENABLE_TIMEOUT;
            return 0;
        }
        case 40330:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT;
            return 0;
        }
        case 40338:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN;
            return 0;
        }
        case 40350:
        {
            *registerValue = (int64_t)PARAMETERS.PWM_FREQUENCY;
            return 0;
        }
        case 40351:
        {
            *registerValue = (int64_t)PARAMETERS.PWM_DEADTIME;
            return 0;
        }
        case 40352:
        {
            *registerValue = (int64_t)PARAMETERS.PWM_DEADTIME_PRESCALING;
            return 0;
        }
        case 40360:
        {
            *registerValue = (int64_t)PARAMETERS.MODBUS_ADDRESS;
            return 0;
        }
        default:
        {
            return 1;
        }
    }

    return 1;
}
