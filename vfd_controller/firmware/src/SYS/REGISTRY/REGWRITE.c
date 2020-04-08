#include "REGISTRY.h"

void REGISTRY_REGWRITE(uint16_t registerAddress, int64_t registerValue)
{
    switch(registerAddress)
    {
        case 40100:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 0) && (registerValue == 1))
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
        case 40102:
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
        case 40103:
        {
            if(registerValue == 1)
            {
                SETPOINTS.REVERSAL = 1;
            }

            else
            {
                SETPOINTS.REVERSAL = 0;
            }

            return;
        }
        case 40104:
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
        case 40105:
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
        case 40110:
        {
            if((SETPOINTS.ENABLE_PROGMODE == 0) && (registerValue == 1))
            {
                SETPOINTS.ENABLE = -1;
                SETPOINTS.ENABLE_PROGMODE = 1;
            }

            else if((SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue != 1) && (SETPOINTS.TRIGGER_PARAMETER_SAVE == 1))
            {
                REGISTRY_PUSHEEMEM();
            }

            else
            {
                SETPOINTS.ENABLE_PROGMODE = 0;
            }

            return;
        }
        case 40111:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                if(registerValue == 1)
                {
                    SETPOINTS.TRIGGER_PARAMETER_SAVE = 1;
                }

                else
                {
                    SETPOINTS.TRIGGER_PARAMETER_SAVE = 0;
                }
            }

            return;
        }
        case 40120:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                if(registerValue == 1)
                {
                    uint16_t counter = 0;
                    uint64_t temp = 0;
                    uint64_t scaled_motorvoltage = 0;
                    uint64_t scaled_dcbusvoltage = 0;
                    uint64_t uboost_startvoltage = 0;
                    uint64_t uboost_finalvoltage = 0;

                    /* calculate u/f lookup-table base values */
                    scaled_dcbusvoltage = ((PARAMETERS.DCBUS_NOMINAL_VOLTAGE * (1.10 * 1774)) / 2048);
                    scaled_motorvoltage = ((PARAMETERS.MOTOR_NOMINAL_VOLTAGE * (1.00 * 1448)) / 1024);
                    scaled_motorvoltage = ((scaled_motorvoltage * 255) / scaled_dcbusvoltage);

                    /* calculate u/f lookup-table general curve */
                    for(counter = 0; counter < 2048; counter++)
                    {
                        temp = ((scaled_motorvoltage * counter) / (PARAMETERS.MOTOR_NOMINAL_FREQUENCY / 32));

                        if(temp > 0xFF)
                        {
                            temp = 0xFF;
                        }

                        PARAMETERS.CONTROLLER_UF_VALUE[counter] = (uint8_t)temp;
                    }

                    /* calculate u/f lookup-table voltage boost enhancement */
                    uboost_startvoltage = ((PARAMETERS.MOTOR_MINIMAL_VOLTAGE * (1.00 * 1774)) / 1024);
                    uboost_startvoltage = ((uboost_startvoltage * 255) / scaled_dcbusvoltage);
                    uboost_finalvoltage = PARAMETERS.CONTROLLER_UF_VALUE[(PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY / 32)];
                    scaled_motorvoltage = uboost_finalvoltage - uboost_startvoltage;

                    for(counter = 0; counter <= (PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY / 32); counter++)
                    {
                        temp = uboost_startvoltage + ((scaled_motorvoltage * counter) / (PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY / 32));

                        if(temp > 0xFF)
                        {
                            temp = 0xFF;
                        }

                        PARAMETERS.CONTROLLER_UF_VALUE[counter] = (uint8_t)temp;
                    }
                }
            }

            return;
        }
        case 40300:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MODBUS_ADDRESS = (uint16_t)registerValue;
            }

            return;
        }
        case 40400:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.PWM_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40401:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.PWM_DEADTIME = (uint16_t)registerValue;
            }

            return;
        }
        case 40402:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.PWM_DEADTIME_PRESCALING = (uint16_t)registerValue;
            }

            return;
        }
        case 40500:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.DCBUS_NOMINAL_VOLTAGE = (uint16_t)registerValue;
            }

            return;
        }
        case 40501:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_VOLTAGE = (uint16_t)registerValue;
            }

            return;
        }
        case 40502:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_MINIMAL_VOLTAGE = (uint16_t)registerValue;
            }

            return;
        }
        case 40503:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40504:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40505:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT = (int16_t)registerValue;;
            }

            return;
        }
        case 40506:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_NOMINAL_REV_CURRENT = (int16_t)registerValue;
            }

            return;
        }
        case 40507:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT = (int16_t)registerValue;
            }

            return;
        }
        case 40508:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT = (int16_t)registerValue;
            }

            return;
        }
        case 40600:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40601:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY = (uint16_t)registerValue;
            }

            return;
        }
        case 40602:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR = (int16_t)registerValue;
            }

            return;
        }
        case 40603:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_INTEGRAL_FACTOR = (int16_t)registerValue;
            }

            return;
        }
        case 40604:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_SAMPLETIME = (uint64_t)registerValue;
            }

            return;
        }
        case 40612:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = (uint64_t)registerValue;
            }

            return;
        }
        case 40620:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = (uint64_t)registerValue;
            }

            return;
        }
        case 40628:
        {
            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = (uint64_t)registerValue;
            }

            return;
        }
        default:
        {
            if((registerAddress >= 41000) && (registerAddress <= 43047))
            {
                PARAMETERS.CONTROLLER_UF_VALUE[registerAddress - 41000] = (uint16_t)registerValue;
            }

            return;
        }
    }
}
