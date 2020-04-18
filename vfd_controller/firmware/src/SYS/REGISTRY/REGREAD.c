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
        case 30205:
        {
            *registerValue = (uint16_t)PROCESSVALUES.MOTOR_FREQUENCY;
            return 0;
        }
        case 30206:
        {
            *registerValue = (uint16_t)PROCESSVALUES.MOTOR_POWER;
            return 0;
        }
        case 30207:
        {
            *registerValue = (uint16_t)PROCESSVALUES.MOTOR_VOLTAGE;
            return 0;
        }
        case 30208:
        {
            *registerValue = (uint16_t)(PROCESSVALUES.MOTOR_CURRENT >> 16);
            return 0;
        }
        case 30209:
        {
            *registerValue = (uint16_t)(PROCESSVALUES.MOTOR_CURRENT >>  0);
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
        case 40400:
        {
            *registerValue = (uint16_t)PARAMETERS.PWM_FREQUENCY;
            return 0;
        }
        case 40401:
        {
            *registerValue = (uint16_t)PARAMETERS.PWM_DEADTIME;
            return 0;
        }
        case 40402:
        {
            *registerValue = (uint16_t)PARAMETERS.PWM_DEADTIME_PRESCALING;
            return 0;
        }
        case 40500:
        {
            *registerValue = (uint16_t)PARAMETERS.DCBUS_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40501:
        {
            *registerValue = (uint16_t)PARAMETERS.DCBUS_MINIMAL_VOLTAGE;
            return 0;
        }
        case 40502:
        {
            *registerValue = (uint16_t)PARAMETERS.DCBUS_MAXIMAL_VOLTAGE;
            return 0;
        }
        case 40503:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_VOLTAGE;
            return 0;
        }
        case 40504:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_MINIMAL_VOLTAGE;
            return 0;
        }
        case 40505:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40506:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY;
            return 0;
        }
        case 40507:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT;
            return 0;
        }
        case 40508:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_NOMINAL_REV_CURRENT;
            return 0;
        }
        case 40509:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT;
            return 0;
        }
        case 40510:
        {
            *registerValue = (uint16_t)PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT;
            return 0;
        }

        case 40600:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY;
            return 0;
        }
        case 40601:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY;
            return 0;
        }
        case 40602:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR;
            return 0;
        }
        case 40603:
        {
            *registerValue = (uint16_t)PARAMETERS.CONTROLLER_INTEGRAL_FACTOR;
            return 0;
        }
        case 40604:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >> 48);
            return 0;
        }
        case 40605:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >> 32);
            return 0;
        }
        case 40606:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >> 16);
            return 0;
        }
        case 40607:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_SAMPLETIME >>  0);
            return 0;
        }
        case 40608:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >> 48);
            return 0;
        }
        case 40609:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >> 32);
            return 0;
        }
        case 40610:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >> 16);
            return 0;
        }
        case 40611:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_ENABLE_TIMEOUT >>  0);
            return 0;
        }
        case 40612:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >> 48);
            return 0;
        }
        case 40613:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >> 32);
            return 0;
        }
        case 40614:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >> 16);
            return 0;
        }
        case 40615:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT >>  0);
            return 0;
        }
        case 40616:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >> 48);
            return 0;
        }
        case 40617:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >> 32);
            return 0;
        }
        case 40618:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >> 16);
            return 0;
        }
        case 40619:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN >>  0);
            return 0;
        }
        case 40620:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >> 48);
            return 0;
        }
        case 40621:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >> 32);
            return 0;
        }
        case 40622:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >> 16);
            return 0;
        }
        case 40623:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_NOMINAL_UMZ_TIMEOUT >>  0);
            return 0;
        }
        case 40624:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >> 48);
            return 0;
        }
        case 40625:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >> 32);
            return 0;
        }
        case 40626:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >> 16);
            return 0;
        }
        case 40627:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_OVERDRIVE_UMZ_TIMEOUT >>  0);
            return 0;
        }
        case 40628:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >> 48);
            return 0;
        }
        case 40629:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >> 32);
            return 0;
        }
        case 40630:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >> 16);
            return 0;
        }
        case 40631:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_UMZ_TIMEOUT >>  0);
            return 0;
        }
        case 40632:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_VOLTAGE_OFFSET >>  16);
            return 0;
        }
        case 40633:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_VOLTAGE_OFFSET >>  0);
            return 0;
        }
        case 40634:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_CURRENT_OFFSET >>  16);
            return 0;
        }
        case 40635:
        {
            *registerValue = (uint16_t)(PARAMETERS.CONTROLLER_DCBUS_CURRENT_OFFSET >>  0);
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
