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
    uint64_t counter = 0;
    uint8_t  frameComplete = 0;
    uint16_t registerAddress = 0;
    int64_t registerValue = 0;
    uint16_t registerTemp = 0;
    uint64_t convert = 0;


    /* check for data in receiver buffer */
    if(!(RS232_RXSIZE()))
    {
        return;
    }

    /* fetch byte per byte and store as frame */
    while(RS232_RXSIZE())
    {
        /* fetch new byte from buffer */
        RS232_FETCH(&temp, 1);

        /* start a new frame upon arrival of 'R' */
        if(temp == 'R')
        {
            SCPISTATES.frame[0] = 'R';
            SCPISTATES.framePtr = 1;
        }

        /* end of scpi command detected - mark frame as completed */
        else if(temp == '\n')
        {
            SCPISTATES.frame[SCPISTATES.framePtr] = '\n';
            frameComplete = 1;
            break;
        }

        /* store current byte in frame */
        else
        {
            SCPISTATES.frame[SCPISTATES.framePtr] = temp;
            SCPISTATES.framePtr++;
        }
    }


    /* if frame is complete - ready to interprete message */
    if(frameComplete == 1)
    {
        /* check if frame start is valid - otherwise drop frame */
        if((SCPISTATES.framePtr < 10) || (SCPISTATES.frame[0] != 'R') || (SCPISTATES.frame[1] != 'E') || (SCPISTATES.frame[2] != 'G') || (SCPISTATES.frame[3] != ':'))
        {
            /* send a hint for help command? */
            RS232_SEND("Send REG:00000? for help\n", 25);
            SCPISTATES.framePtr = 0;
            return;
        }

        /* convert message scpi address from asci to binary */
        registerAddress = registerAddress + ((SCPISTATES.frame[4] - 0x30) * 10000);
        registerAddress = registerAddress + ((SCPISTATES.frame[5] - 0x30) *  1000);
        registerAddress = registerAddress + ((SCPISTATES.frame[6] - 0x30) *   100);
        registerAddress = registerAddress + ((SCPISTATES.frame[7] - 0x30) *    10);
        registerAddress = registerAddress + ((SCPISTATES.frame[8] - 0x30) *     1);

        /* check if last symbol before frame end was ? - msg is a query */
        if(SCPISTATES.frame[--SCPISTATES.framePtr] == '?')
        {
            registerValue = 0;

            /* read value from registry */
            if(REGISTRY_REGREAD(registerAddress, &registerTemp))
            {
                /* registry address does not exist - check if user wants help text */
                if(registerAddress == 00000)
                {
                    counter = 0;

                    while((temp = pgm_read_byte(SCPI_INFO + counter)) != '\0')
                    {
                        RS232_SEND(&temp, 1);
                        counter++;
                    }
                }

                /* or the plot of the U/F curve */
                else if(registerAddress == 43048)
                {
                    for(counter = 0; counter < 2048; counter++)
                    {
                        temp = (char)PARAMETERS.CONTROLLER_UF_VALUE[counter];
                        RS232_SEND(&temp, 1);
                    }
                }

                /* frame does not exist or is protocol specific - drop it */
                SCPISTATES.framePtr = 0;
                return;
            }

            /* read multi byte register */
            if((registerAddress == 30201) || (registerAddress == 30208) || (registerAddress == 40301) || (registerAddress == 40632) || (registerAddress == 40634))
            {
                registerValue = (((int64_t)((int16_t)registerTemp)) << 16);
                REGISTRY_REGREAD(registerAddress + 1, &registerTemp);
                registerValue |= (((int64_t)((int16_t)registerTemp)) <<  0);
            }

            else if((registerAddress == 40604) || (registerAddress == 40608) || (registerAddress == 40612) || (registerAddress == 40616) || (registerAddress == 40620) || (registerAddress == 40624) || (registerAddress == 40628))
            {
                registerValue = (((int64_t)registerTemp) << 48);
                REGISTRY_REGREAD(registerAddress + 1, &registerTemp);
                registerValue |= (((int64_t)registerTemp) << 32);
                REGISTRY_REGREAD(registerAddress + 2, &registerTemp);
                registerValue |= (((int64_t)registerTemp) << 16);
                REGISTRY_REGREAD(registerAddress + 3, &registerTemp);
                registerValue |= (((int64_t)registerTemp) <<  0);
            }

            else
            {
                registerValue = (((int64_t)registerTemp) << 0);
            }

            /* convert value to ascii and send it */
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
        }

        /* otherwise message is an input */
        else
        {
            /* set initial muliplication factor */
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

            if((registerAddress == 40301) || (registerAddress == 40632) || (registerAddress == 40634))
            {
                REGISTRY_REGWRITE(registerAddress + 0, (uint16_t)(registerValue >> 16));
                REGISTRY_REGWRITE(registerAddress + 1, (uint16_t)(registerValue >>  0));
            }

            else if((registerAddress == 40604) || (registerAddress == 40608) || (registerAddress == 40612) || (registerAddress == 40616) || (registerAddress == 40620) || (registerAddress == 40624) || (registerAddress == 40628))
            {
                REGISTRY_REGWRITE(registerAddress + 0, (uint16_t)(registerValue >> 48));
                REGISTRY_REGWRITE(registerAddress + 1, (uint16_t)(registerValue >> 32));
                REGISTRY_REGWRITE(registerAddress + 2, (uint16_t)(registerValue >> 16));
                REGISTRY_REGWRITE(registerAddress + 3, (uint16_t)(registerValue >>  0));
            }

            else
            {
                REGISTRY_REGWRITE(registerAddress + 0, (uint16_t)(registerValue >>  0));
            }
        }

        SCPISTATES.framePtr = 0;
        return;
    }


    /*REG:40100 100\n */
    /*REG:40100?\n    */
}
