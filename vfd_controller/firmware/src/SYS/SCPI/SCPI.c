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
    uint16_t  registerAddress = 0;
    uint64_t registerValue = 0;
    uint64_t convert = 1;

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
                //case 30210: registerValue = SYSTEM_STATUS; break;
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
                default:    registerValue = 0; break;
            }

            // NOT YET TESTED FROM HERE
            SCPISTATES.framePtr = 31;

            while(registerValue > 10)
            {
                SCPISTATES.frame[SCPISTATES.framePtr] = (registerValue % 10) + 0x30;
                registerValue = registerValue / 10;
                SCPISTATES.framePtr--;
            }

            SCPISTATES.frame[SCPISTATES.framePtr] = (registerValue % 10) + 0x30;
        }

        else
        {
            while(SCPISTATES.frame[SCPISTATES.framePtr] != 0x20)
            {
                registerValue = registerValue + ((SCPISTATES.frame[SCPISTATES.framePtr] - 0x30) * convert);
                convert = convert * 10;
                SCPISTATES.framePtr--;
            }
        }

        SCPISTATES.framePtr = 0;


    }


    /*REG:40100 100\n */
    /*REG:40100?\n    */
}
