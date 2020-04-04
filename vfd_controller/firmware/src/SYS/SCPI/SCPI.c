#include "SCPI.h"
#include "../../RES/RS232/RS232.h"
#include "../REGISTRY/REGISTRY.h"

static struct SCPISTATES_s
{
    uint8_t framePtr;
    char    frame[32];
}
SCPISTATES;

void SCPI_INIT(void)
{
    RS232_INIT();

    SCPISTATES.framePtr = 0;
}

void SCPI_PROCESS(void)
{
    char     temp;
    uint8_t  frameComplete = 0;
    uint16_t registerAddress = 0;
    int64_t registerValue = 0;
    uint64_t convert = 0;

    if(!(RS232_RXSIZE()))
    {
        return;
    }

    while(RS232_RXSIZE())
    {
        RS232_FETCH(&temp, 1);

        if(temp == 'R')
        {
            SCPISTATES.framePtr = 0;
            SCPISTATES.frame[SCPISTATES.framePtr] = 'R';
            SCPISTATES.framePtr++;
        }

        else if(temp == '\n')
        {
            SCPISTATES.frame[SCPISTATES.framePtr] = '\n';
            frameComplete = 1;
            break;
        }

        else
        {
            SCPISTATES.frame[SCPISTATES.framePtr] = temp;
            SCPISTATES.framePtr++;
        }
    }

    if(frameComplete == 1)
    {
        if((SCPISTATES.framePtr < 10) || (SCPISTATES.frame[0] != 'R') || (SCPISTATES.frame[1] != 'E') || (SCPISTATES.frame[2] != 'G') || (SCPISTATES.frame[3] != ':'))
        {
            SCPISTATES.framePtr = 0;
            return;
        }

        registerAddress = registerAddress + ((SCPISTATES.frame[4] - 0x30) * 10000);
        registerAddress = registerAddress + ((SCPISTATES.frame[5] - 0x30) *  1000);
        registerAddress = registerAddress + ((SCPISTATES.frame[6] - 0x30) *   100);
        registerAddress = registerAddress + ((SCPISTATES.frame[7] - 0x30) *    10);
        registerAddress = registerAddress + ((SCPISTATES.frame[8] - 0x30) *     1);

        SCPISTATES.framePtr--;

        if(SCPISTATES.frame[SCPISTATES.framePtr] == '?')
        {
            if(registerAddress == 30210)
            {
                RS232_SEND(PROCESSVALUES.SYSTEM_STATUS + 1, PROCESSVALUES.SYSTEM_STATUS[0]);
                SCPISTATES.framePtr = 0;
                return;
            }

            if(registerAddress == 30206)
            {
                RS232_SEND((char *)&(PROCESSVALUES.CONTROLLER_UF_VALUE[0]), 250);
                RS232_SEND((char *)&(PROCESSVALUES.CONTROLLER_UF_VALUE[250]), 250);
                RS232_SEND((char *)&(PROCESSVALUES.CONTROLLER_UF_VALUE[500]), 250);
                RS232_SEND((char *)&(PROCESSVALUES.CONTROLLER_UF_VALUE[750]), 250);
                RS232_SEND((char *)&(PROCESSVALUES.CONTROLLER_UF_VALUE[100]), 24);
                SCPISTATES.framePtr = 0;
                return;
            }

            switch(registerAddress)
            {
                case 40100: registerValue = SETPOINTS.ENABLE; break;
                case 40101: registerValue = SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE; break;
                case 40102: registerValue = SETPOINTS.ENABLE_CURRENT_OVERDRIVE; break;
                case 40103: registerValue = SETPOINTS.ENABLE_PROGMODE; break;
                case 40104: registerValue = SETPOINTS.TARGET_FREQUENCY; break;
                case 40105: registerValue = SETPOINTS.TARGET_TORQUE; break;
                case 40106: registerValue = SETPOINTS.REVERSAL; break;
                case 30200: registerValue = PROCESSVALUES.DCBUS_VOLTAGE; break;
                case 30201: registerValue = PROCESSVALUES.DCBUS_CURRENT; break;
                case 30202: registerValue = PROCESSVALUES.MOTOR_VOLTAGE; break;
                case 30203: registerValue = PROCESSVALUES.MOTOR_CURRENT; break;
                case 30204: registerValue = PROCESSVALUES.MOTOR_FREQUENCY; break;
                case 30205: registerValue = PROCESSVALUES.MOTOR_POWER; break;
                case 40300: registerValue = PARAMETERS.DCBUS_NOMINAL_VOLTAGE; break;
                case 40301: registerValue = PARAMETERS.MOTOR_NOMINAL_VOLTAGE; break;
                case 40302: registerValue = PARAMETERS.MOTOR_MINIMAL_VOLTAGE; break;
                case 40303: registerValue = PARAMETERS.MOTOR_NOMINAL_FREQUENCY; break;
                case 40304: registerValue = PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY; break;
                case 40305: registerValue = PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT; break;
                case 40306: registerValue = PARAMETERS.MOTOR_NOMINAL_REV_CURRENT; break;
                case 40307: registerValue = PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT; break;
                case 40308: registerValue = PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT; break;
                case 40310: registerValue = PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY; break;
                case 40311: registerValue = PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY; break;
                case 40312: registerValue = PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR; break;
                case 40313: registerValue = PARAMETERS.CONTROLLER_INTEGRAL_FACTOR; break;
                case 40314: registerValue = PARAMETERS.CONTROLLER_SAMPLETIME; break;
                case 40322: registerValue = PARAMETERS.CONTROLLER_ENABLE_TIMEOUT; break;
                case 40330: registerValue = PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT; break;
                case 40338: registerValue = PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN; break;
                case 40350: registerValue = PARAMETERS.PWM_FREQUENCY; break;
                case 40351: registerValue = PARAMETERS.PWM_DEADTIME; break;
                case 40352: registerValue = PARAMETERS.PWM_DEADTIME_PRESCALING; break;
                case 40360: registerValue = PARAMETERS.MODBUS_ADDRESS; break;
                default   : SCPISTATES.framePtr = 0; return;
            }

            SCPISTATES.frame[31] = '\n';
            SCPISTATES.framePtr = 30;
            convert = ' ';

            if(registerValue < 0)
            {
                convert = '-';
                registerValue = registerValue * (-1);
            }

            while(registerValue >= 10)
            {
                SCPISTATES.frame[SCPISTATES.framePtr--] = (registerValue % 10) + 0x30;
                registerValue = registerValue / 10;
            }

            SCPISTATES.frame[SCPISTATES.framePtr--] = registerValue + 0x30;
            SCPISTATES.frame[SCPISTATES.framePtr] = convert;
            RS232_SEND(&(SCPISTATES.frame[SCPISTATES.framePtr]), 32 - SCPISTATES.framePtr);
            SCPISTATES.framePtr = 0;
            return;
        }

        else
        {
            convert = 1;

            /* convert string number to integer for register value */
            while(SCPISTATES.frame[SCPISTATES.framePtr] != 0x20)
            {
                if(SCPISTATES.frame[SCPISTATES.framePtr] == '-')
                {
                    registerValue = registerValue * (-1);
                    break;
                }

                registerValue = registerValue + ((SCPISTATES.frame[SCPISTATES.framePtr] - 0x30) * convert);
                convert = convert * 10;
                SCPISTATES.framePtr--;
            }

            /* set register values */
            if((registerAddress == 40103) && (SETPOINTS.ENABLE_PROGMODE == 0) && (registerValue == 1))
            {
                SETPOINTS.ENABLE_PROGMODE = 1;
                SCPISTATES.framePtr = 0;
                return;
            }

            else if((registerAddress == 40103) && (SETPOINTS.ENABLE_PROGMODE == 1) && (registerValue == 0))
            {
                SETPOINTS.ENABLE_PROGMODE = 1;
                REGISTRY_UPDATE();
                SCPISTATES.framePtr = 0;
                return;
            }

            switch(registerAddress)
            {
                case 40100: SETPOINTS.ENABLE = registerValue; break;
                case 40101: SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE = registerValue; break;
                case 40102: SETPOINTS.ENABLE_CURRENT_OVERDRIVE = registerValue; break;
                case 40104: SETPOINTS.TARGET_FREQUENCY = registerValue; break;
                case 40105: SETPOINTS.TARGET_TORQUE = registerValue; break;
                case 40106: SETPOINTS.REVERSAL = registerValue; break;
                default   : break;
            }

            /* setpoint limiting */
            if(SETPOINTS.ENABLE_CURRENT_OVERDRIVE > 1)
            {
                SETPOINTS.ENABLE_CURRENT_OVERDRIVE = 0;
            }

            if(SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE == 1)
            {
                if(SETPOINTS.TARGET_FREQUENCY > PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY)
                {
                    SETPOINTS.TARGET_FREQUENCY = PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY;
                }
            }

            else
            {
                SETPOINTS.ENABLE_FREQUENCY_OVERDRIVE = 0;

                if(SETPOINTS.TARGET_FREQUENCY > PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY)
                {
                    SETPOINTS.TARGET_FREQUENCY = PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY;
                }
            }

            if(SETPOINTS.TARGET_TORQUE > 100)
            {
                SETPOINTS.TARGET_TORQUE = 100;
            }

            else if (SETPOINTS.TARGET_TORQUE < -100)
            {
                SETPOINTS.TARGET_TORQUE = -100;
            }

            if(SETPOINTS.ENABLE_PROGMODE == 1)
            {
                switch(registerAddress)
                {
                    case 40300: PARAMETERS.DCBUS_NOMINAL_VOLTAGE = registerValue; break;
                    case 40301: PARAMETERS.MOTOR_NOMINAL_VOLTAGE = registerValue; break;
                    case 40302: PARAMETERS.MOTOR_MINIMAL_VOLTAGE = registerValue; break;
                    case 40303: PARAMETERS.MOTOR_NOMINAL_FREQUENCY = registerValue; break;
                    case 40304: PARAMETERS.MOTOR_VOLTBOOST_FREQUENCY = registerValue; break;
                    case 40305: PARAMETERS.MOTOR_NOMINAL_FWD_CURRENT = registerValue; break;
                    case 40306: PARAMETERS.MOTOR_NOMINAL_REV_CURRENT = registerValue; break;
                    case 40307: PARAMETERS.MOTOR_OVERDRIVE_FWD_CURRENT = registerValue; break;
                    case 40308: PARAMETERS.MOTOR_OVERDRIVE_REV_CURRENT = registerValue; break;
                    case 40310: PARAMETERS.CONTROLLER_NOMINAL_FREQUENCY = registerValue; break;
                    case 40311: PARAMETERS.CONTROLLER_OVERDRIVE_FREQUENCY = registerValue; break;
                    case 40312: PARAMETERS.CONTROLLER_PROPORTIONAL_FACTOR = registerValue; break;
                    case 40313: PARAMETERS.CONTROLLER_INTEGRAL_FACTOR = registerValue; break;
                    case 40314: PARAMETERS.CONTROLLER_SAMPLETIME = registerValue; break;
                    case 40322: PARAMETERS.CONTROLLER_ENABLE_TIMEOUT = registerValue; break;
                    case 40330: PARAMETERS.CONTROLLER_OVERDRIVE_TIMEOUT = registerValue; break;
                    case 40338: PARAMETERS.CONTROLLER_OVERDRIVE_COOLDOWN = registerValue; break;
                    case 40350: PARAMETERS.PWM_FREQUENCY = registerValue; break;
                    case 40351: PARAMETERS.PWM_DEADTIME = registerValue; break;
                    case 40352: PARAMETERS.PWM_DEADTIME_PRESCALING = registerValue; break;
                    case 40360: PARAMETERS.MODBUS_ADDRESS = registerValue; break;
                    default   : SCPISTATES.framePtr = 0; return;
                }
            }

            SCPISTATES.framePtr = 0;
            return;
        }

        SCPISTATES.framePtr = 0;
        return;
    }


    /*REG:40100 100\n */
    /*REG:40100?\n    */
}
