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
            *registerValue = (int64_t)SETPOINTS.TARGET_TORQUE;
            return 0;
        }
        case 40102:
        {
            *registerValue = (int64_t)SETPOINTS.TARGET_FREQUENCY;
            return 0;
        }
        case 40103:
        {
            *registerValue = (int64_t)SETPOINTS.REVERSAL;
            return 0;
        }
        case 40104:
        {
            *registerValue = (int64_t)SETPOINTS.ENABLE_CURRENT_OVERDRIVE;
            return 0;
        }
        case 40105:
        {
            *registerValue = (int64_t)SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE;
            return 0;
        }
        case 40110:
        {
            *registerValue = (int64_t)SETPOINTS.ENABLE_PROGMODE;
            return 0;
        }
        case 40111:
        {
            *registerValue = (int64_t)SETPOINTS.TRIGGER_PARAMETER_SAVE;
            return 0;
        }
        case 40120:
        {
            *registerValue = (int64_t)SETPOINTS.TRIGGER_UF_CALCULATION;
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
            *registerValue = (int64_t)PARAMETERS.MODBUS_ADDRESS;
            return 0;
        }
        case 40400:
        {
            *registerValue = (int64_t)PARAMETERS.PWM_FREQUENCY;
            return 0;
        }
        case 40401:
        {
            *registerValue = (int64_t)PARAMETERS.PWM_DEADTIME;
            return 0;
        }
        case 40402:
        {
            *registerValue = (int64_t)PARAMETERS.PWM_DEADTIME_PRESCALING;
            return 0;
        }
        case 40500:
        {
            *registerValue = (int64_t)PARAMETERS.DCBUS_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40501:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40502:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_MINIMAL_VOLTAGE;
            return 0;
        }
        case 40503:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40504:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY;
            return 0;
        }
        case 40505:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT;
            return 0;
        }
        case 40506:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_NOMINAL_REV_CURRENT;
            return 0;
        }
        case 40507:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT;
            return 0;
        }
        case 40508:
        {
            *registerValue = (int64_t)PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT;
            return 0;
        }

        case 40600:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40601:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY;
            return 0;
        }
        case 40602:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR;
            return 0;
        }
        case 40603:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_INTEGRAL_FACTOR;
            return 0;
        }
        case 40604:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_SAMPLETIME;
            return 0;
        }
        case 40612:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_ENABLE_TIMEOUT;
            return 0;
        }
        case 40620:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT;
            return 0;
        }
        case 40628:
        {
            *registerValue = (int64_t)PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN;
            return 0;
        }
        default:
        {
            if((registerAddress >= 41000) && (registerAddress <= 43047))
            {
                *registerValue = (int64_t)PARAMETERS.CONTROLLER_UF_VALUE[registerAddress - 41000];
                return 0;
            }

            return 1;
        }
    }

    return 1;
}
