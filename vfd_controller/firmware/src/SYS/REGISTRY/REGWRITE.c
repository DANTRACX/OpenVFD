#include "REGISTRY.h"

void REGISTRY_REGWRITE(uint16_t registerAddress, int64_t registerValue)
{
    switch(registerAddress)
    {
        case 40100:
        {
            if(registerValue == 1)
            {
                SETPOINTS.ENABLE = 1;
            }

            else
            {
                SETPOINTS.ENABLE = -1;
            }

            return;
        }
        case 40101:
        {
            if(registerValue == 1)
            {
                SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE = 1;
            }

            else
            {
                SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE = 0;
            }

            return;
        }
        case 40102:
        {
            if(registerValue == 1)
            {
                SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 1;
            }

            else
            {
                SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 0;
            }

            return;
        }
        case 40103:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 0) && (registerValue == 1))
            {
                SETPOINTS.ENABLE_PROGMODE = 1;
            }

            else if((SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue != 1))
            {
                REGISTRY_PUSHEEMEM();
            }

            return;
        }
        case 40104:
        {
            SETPOINTS.TARGET_FREQUENCY = (uint16_t)registerValue;

            if(SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE == 1)
            {
                if(SETPOINTS.TARGET_FREQUENCY > PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY)
                {
                    SETPOINTS.TARGET_FREQUENCY = PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY;
                }
            }

            else
            {
                if(SETPOINTS.TARGET_FREQUENCY > PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY)
                {
                    SETPOINTS.TARGET_FREQUENCY = PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY;
                }
            }

            return;
        }
        case 40105:
        {
            if((int16_t)registerValue > 100)
            {
                SETPOINTS.TARGET_TORQUE = 100;
            }

            else if((int16_t)registerValue < -100)
            {
                SETPOINTS.TARGET_TORQUE = -100;
            }

            else
            {
                SETPOINTS.TARGET_TORQUE = (int16_t)registerValue;
            }

            return;
        }
        case 40106:
        {
            SETPOINTS.REVERSAL = (uint16_t)registerValue;
            return;
        }
        case 40300:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.DCBUS_NOMINAL_VOLTAGE = (uint16_t)registerValue;
            }

            return;
        }
        case 40301:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_VOLTAGE = (uint16_t)registerValue;
            }

            return;
        }
        case 40302:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_MINIMAL_VOLTAGE = (uint16_t)registerValue;
            }

            return;
        }
        case 40303:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40304:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40305:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT = (int16_t)registerValue;;
            }

            return;
        }
        case 40306:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_REV_CURRENT = (int16_t)registerValue;
            }

            return;
        }
        case 40307:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT = (int16_t)registerValue;
            }

            return;
        }
        case 40308:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT = (int16_t)registerValue;
            }

            return;
        }
        case 40310:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40311:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40312:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR = (int16_t)registerValue;
            }

            return;
        }
        case 40313:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_INTEGRAL_FACTOR = (int16_t)registerValue;
            }

            return;
        }
        case 40314:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_SAMPLETIME = (uint64_t)registerValue;
            }

            return;
        }
        case 40322:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = (uint64_t)registerValue;
            }

            return;
        }
        case 40330:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = (uint64_t)registerValue;
            }

            return;
        }
        case 40338:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = (uint64_t)registerValue;
            }

            return;
        }
        case 40350:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.PWM_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40351:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.PWM_DEADTIME = (uint16_t)registerValue;
            }

            return;
        }
        case 40352:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.PWM_DEADTIME_PRESCALING = (uint16_t)registerValue;
            }

            return;
        }
        case 40360:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MODBUS_ADDRESS = (uint16_t)registerValue;
            }

            return;
        }
        default:
        {
            return;
        }
    }
}
