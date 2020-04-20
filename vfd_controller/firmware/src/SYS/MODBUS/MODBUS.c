#include "MODBUS.h"
#include "../REGISTRY/REGISTRY.h"
#include "../../RES/RS485/RS485.h"
#include "../../RES/MEM/MEM.h"

static union convert_u
{
    char        cbyte[2];
    uint16_t    iword;
}
convert;

__attribute__((always_inline))
static inline uint16_t _bytes2word(char *highByte, char *lowByte)
{
    convert.cbyte[1] = *highByte;
    convert.cbyte[0] = *lowByte;
    return convert.iword;
}

static inline void _word2bytes(uint16_t word, char *highByte, char *lowByte)
{
    convert.iword = word;
    *highByte = convert.cbyte[1];
    *lowByte = convert.cbyte[0];
}

__attribute__((always_inline))
static inline void _modbus_exception(uint8_t id, uint8_t fc, uint8_t ex, char *buffer)
{
    buffer[0] = id;
    buffer[1] = (fc | 0x80);
    buffer[2] = ex;
    MEM_CRC16(&buffer[0], 3, &buffer[3], &buffer[4]);
    RS485_SEND(buffer, 5);
}


void MODBUS_INIT(void)
{
    RS485_INIT(PARAMETERS.MODBUS_ADDRESS, PARAMETERS.MODBUS_BAUDRATE, PARAMETERS.MODBUS_PARITY);
}

void MODBUS_PROCESS(void)
{
    /* check for data in receiver buffer */
    if(!(RS485_RXSIZE()))
    {
        return;
    }

    char     temp[256];
    uint16_t counter = 0;
    uint16_t registerCount = 0;
    uint16_t registerAddress = 0;
    uint16_t registerValue = 0;
    uint8_t  bufferPtr = 0;

    RS485_FETCH(temp, 2);

    switch(temp[1])
    {
        case 0x03:
        {
            if(RS485_RXSIZE() < 6)
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            RS485_FETCH(&temp[2], 6);
            MEM_CRC16(&temp[0], 6, &temp[8], &temp[9]);

            if((temp[6] != temp[8]) || (temp[7] != temp[9]))
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            registerAddress = 40000 + _bytes2word(&temp[2], &temp[3]) + PARAMETERS.MODBUS_OFFSET;
            registerCount = _bytes2word(&temp[4], &temp[5]);
            bufferPtr = 2;
            temp[bufferPtr++] = (uint8_t)registerCount*2;

            for(counter = 0; counter < registerCount; counter++)
            {
                if(REGISTRY_REGREAD(registerAddress + counter, &(convert.iword)) == 1)
                {
                    _modbus_exception(PARAMETERS.MODBUS_ADDRESS, 0x03, 0x02, temp);
                    return;
                }

                temp[bufferPtr++] = convert.cbyte[1];
                temp[bufferPtr++] = convert.cbyte[0];
            }

            MEM_CRC16(&temp[0], (3 + 2*registerCount), &temp[bufferPtr + 0], &temp[bufferPtr + 1]);
            RS485_SEND(temp, (5 + 2*registerCount));
            return;
        }
        case 0x04:
        {
            if(RS485_RXSIZE() < 6)
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            RS485_FETCH(&temp[2], 6);
            MEM_CRC16(&temp[0], 6, &temp[8], &temp[9]);

            if((temp[6] != temp[8]) || (temp[7] != temp[9]))
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            registerAddress = 30000 + _bytes2word(&temp[2], &temp[3]) + PARAMETERS.MODBUS_OFFSET;
            registerCount = _bytes2word(&temp[4], &temp[5]);
            bufferPtr = 2;
            temp[bufferPtr++] = (uint8_t)registerCount*2;

            for(counter = 0; counter < registerCount; counter++)
            {
                if(REGISTRY_REGREAD(registerAddress + counter, &(convert.iword)) == 1)
                {
                    _modbus_exception(PARAMETERS.MODBUS_ADDRESS, 0x04, 0x02, temp);
                    return;
                }

                temp[bufferPtr++] = convert.cbyte[1];
                temp[bufferPtr++] = convert.cbyte[0];
            }

            MEM_CRC16(&temp[0], (3 + 2*registerCount), &temp[bufferPtr + 0], &temp[bufferPtr + 1]);
            RS485_SEND(temp, (5 + 2*registerCount));
            return;
        }
        case 0x06:
        {
            if(RS485_RXSIZE() < 6)
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            RS485_FETCH(&temp[2], 6);
            MEM_CRC16(&temp[0], 6, &temp[8], &temp[9]);

            if((temp[6] != temp[8]) || (temp[7] != temp[9]))
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            registerAddress = 40000 + _bytes2word(&temp[2], &temp[3]) + PARAMETERS.MODBUS_OFFSET;
            registerValue = _bytes2word(&temp[4], &temp[5]);

            if(REGISTRY_REGWRITE(registerAddress, registerValue) == 1)
            {
                _modbus_exception(PARAMETERS.MODBUS_ADDRESS, 0x06, 0x02, temp);
                return;
            }

            RS485_SEND(temp, 8);
            return;
        }
        case 0x10:
        {
            if(RS485_RXSIZE() < 5)
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            RS485_FETCH(&temp[2], 5);

            if(RS485_RXSIZE() < (temp[6] + 2))
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            RS485_FETCH(&temp[7], (temp[6] + 2));
            MEM_CRC16(&temp[0], 7 + temp[6], &temp[254], &temp[255]);

            if((temp[7 + temp[6] + 0] != temp[254]) || (temp[7 + temp[6] + 1] != temp[255]))
            {
                RS485_FETCH(temp, RS485_RXSIZE());
                return;
            }

            registerAddress = 40000 + _bytes2word(&temp[2], &temp[3]) + PARAMETERS.MODBUS_OFFSET;
            registerCount = _bytes2word(&temp[4], &temp[5]);
            bufferPtr = 7;

            for(counter = 0; counter < registerCount; counter++)
            {
                if(REGISTRY_REGWRITE(registerAddress + counter, _bytes2word(&temp[bufferPtr + 0], &temp[bufferPtr + 1])) == 1)
                {
                    _modbus_exception(PARAMETERS.MODBUS_ADDRESS, 0x10, 0x02, temp);
                    return;
                }

                bufferPtr = bufferPtr + 2;
            }

            MEM_CRC16(&temp[0], 6, &temp[6], &temp[7]);
            RS485_SEND(temp, 8);
            return;
        }
        default:
        {
            _modbus_exception(PARAMETERS.MODBUS_ADDRESS, temp[1], 0x01, temp);
            break;
        }
    }
}
