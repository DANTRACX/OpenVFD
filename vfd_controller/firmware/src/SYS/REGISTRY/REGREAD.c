#include "REGISTRY.h"

uint8_t REGISTRY_REGREAD(uint16_t registerAddress, uint16_t *registerValue)
{
    switch(registerAddress)
    {
        case 40100:
        {
            *registerValue = (uint16_t)SETPOINTS.ENABLE;
            return 0;
        }
        case 40101:
        {
            *registerValue = (uint16_t)SETPOINTS.TARGET_TORQUE;
            return 0;
        }
        case 40102:
        {
            *registerValue = (uint16_t)SETPOINTS.TARGET_FREQUENCY;
            return 0;
        }
        case 40103:
        {
            *registerValue = (uint16_t)SETPOINTS.REVERSAL;
            return 0;
        }
        case 40104:
        {
            *registerValue = (uint16_t)SETPOINTS.ENABLE_CURRENT_OVERDRIVE;
            return 0;
        }
        case 40105:
        {
            *registerValue = (uint16_t)SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE;
            return 0;
        }
        case 40110:
        {
            *registerValue = (uint16_t)SETPOINTS.ENABLE_PROGMODE;
            return 0;
        }
        case 40111:
        {
            *registerValue = (uint16_t)SETPOINTS.TRIGGER_PARAMETER_SAVE;
            return 0;
        }
        case 40120:
        {
            *registerValue = (uint16_t)SETPOINTS.TRIGGER_UF_CALCULATION;
            return 0;
        }
        case 40150:
        {
            *registerValue = (uint16_t)SETPOINTS.TRIGGER_RESET;
            return 0;
        }
        case 30200:
        {
            *registerValue = (uint16_t)PROCESSVALUES.DCBUS_VOLTAGE;
            return 0;
        }
        case 30201:
        {
            *registerValue = (uint16_t)(PROCESSVALUES.DCBUS_CURRENT >> 16);
            return 0;
        }
        case 30202:
        {
            *registerValue = (uint16_t)(PROCESSVALUES.DCBUS_CURRENT >>  0);
            return 0;
        }
        case 30203:
        {
            *registerValue = (uint16_t)PROCESSVALUES.DCBUS_TEMPERATURE;
            return 0;
        }
        case 30204:
        {
            *registerValue = (uint16_t)PROCESSVALUES.MOTOR_FREQUENCY;
            return 0;
        }
        case 30205:
        {
            *registerValue = (uint16_t)PROCESSVALUES.MOTOR_POWER;
            return 0;
        }
        case 30206:
        {
            *registerValue = (uint16_t)PROCESSVALUES.MOTOR_VOLTAGE;
            return 0;
        }
        case 30207:
        {
            *registerValue = (uint16_t)(PROCESSVALUES.MOTOR_CURRENT >> 16);
            return 0;
        }
        case 30208:
        {
            *registerValue = (uint16_t)(PROCESSVALUES.MOTOR_CURRENT >>  0);
            return 0;
        }
        case 30209:
        {
            *registerValue = (uint16_t)PROCESSVALUES.MOTOR_TEMPERATURE;
            return 0;
        }
        case 30250:
        {
            *registerValue = (uint16_t)PROCESSVALUES.SYSTEM_STATUS;
            return 0;
        }
        case 40300:
        {
            *registerValue = (uint16_t)PARAMETERS.MODBUS_ADDRESS;
            return 0;
        }
        case 40301:
        {
            *registerValue = (uint16_t)(PARAMETERS.MODBUS_BAUDRATE >> 16);
            return 0;
        }
        case 40302:
        {
            *registerValue = (uint16_t)(PARAMETERS.MODBUS_BAUDRATE >>  0);
            return 0;
        }
        case 40303:
        {
            *registerValue = (uint16_t)PARAMETERS.MODBUS_PARITY;
            return 0;
        }
        case 40304:
        {
            *registerValue = (uint16_t)PARAMETERS.MODBUS_OFFSET;
            return 0;
        }
        case 40400:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40401:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_MINIMAL_VOLTAGE;
            return 0;
        }
        case 40402:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40403:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY;
            return 0;
        }
        case 40404:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT;
            return 0;
        }
        case 40405:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_REV_CURRENT;
            return 0;
        }
        case 40406:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT;
            return 0;
        }
        case 40407:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT;
            return 0;
        }
        case 40500:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40501:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY;
            return 0;
        }
        case 40502:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR;
            return 0;
        }
        case 40503:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_INTEGRAL_FACTOR;
            return 0;
        }
        case 40504:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >> 48);
            return 0;
        }
        case 40505:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >> 32);
            return 0;
        }
        case 40506:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >> 16);
            return 0;
        }
        case 40507:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >>  0);
            return 0;
        }
        case 40508:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >> 48);
            return 0;
        }
        case 40509:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >> 32);
            return 0;
        }
        case 40510:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >> 16);
            return 0;
        }
        case 40511:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >>  0);
            return 0;
        }
        case 40512:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >> 48);
            return 0;
        }
        case 40513:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >> 32);
            return 0;
        }
        case 40514:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >> 16);
            return 0;
        }
        case 40515:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >>  0);
            return 0;
        }
        case 40516:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >> 48);
            return 0;
        }
        case 40517:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >> 32);
            return 0;
        }
        case 40518:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >> 16);
            return 0;
        }
        case 40519:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >>  0);
            return 0;
        }
        case 40520:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >> 48);
            return 0;
        }
        case 40521:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >> 32);
            return 0;
        }
        case 40522:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >> 16);
            return 0;
        }
        case 40523:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >>  0);
            return 0;
        }
        case 40524:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >> 48);
            return 0;
        }
        case 40525:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >> 32);
            return 0;
        }
        case 40526:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >> 16);
            return 0;
        }
        case 40527:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >>  0);
            return 0;
        }
        case 40528:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >> 48);
            return 0;
        }
        case 40529:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >> 32);
            return 0;
        }
        case 40530:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >> 16);
            return 0;
        }
        case 40531:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >>  0);
            return 0;
        }
        case 40532:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_DCBUS_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40533:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_DCBUS_MINIMAL_VOLTAGE;
            return 0;
        }
        case 40534:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_DCBUS_MAXIMAL_VOLTAGE;
            return 0;
        }
        case 40535:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_DCBUS_VOLTAGE_OFFSET;
            return 0;
        }
        case 40536:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_DCBUS_CURRENT_OFFSET;
            return 0;
        }
        case 40537:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_DCBUS_FAN_THRESHOLD;
            return 0;
        }
        case 40538:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_DCBUS_MAXIMAL_TEMP;
            return 0;
        }
        case 40539:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_MOTOR_FAN_THRESHOLD;
            return 0;
        }
        case 40540:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_MOTOR_MAXIMAL_TEMP;
            return 0;
        }
        case 40541:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_PWM_FREQUENCY;
            return 0;
        }
        case 40542:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_PWM_DEADTIME;
            return 0;
        }
        case 40543:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_PWM_DEADTIME_PRESCALING;
            return 0;
        }
        default:
        {
            if((registerAddress >= 41000) && (registerAddress <= 43047))
            {
                *registerValue = (uint16_t)PARAMETERS.CONTROLLER_UF_VALUE[registerAddress - 41000];
                return 0;
            }

            return 1;
        }
    }

    return 1;
}
