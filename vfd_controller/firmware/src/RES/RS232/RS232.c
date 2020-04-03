#include "RS232.h"

static volatile struct SENDBUFFER_s
{
    volatile char buffer[256];
    volatile uint8_t readIdx;
    volatile uint8_t writeIdx;
    volatile uint8_t size;
}
SENDBUFFER;

static volatile struct RECVBUFFER_s
{
    volatile char buffer[256];
    volatile uint8_t readIdx;
    volatile uint8_t writeIdx;
    volatile uint8_t size;
}
RECVBUFFER;

void RS232_INIT(void)
{
    /* initialize sendbuffer */
    SENDBUFFER.readIdx = 0;
    SENDBUFFER.writeIdx = 0;
    SENDBUFFER.size = 0;

    /* initialize recvbuffer */
    RECVBUFFER.readIdx = 0;
    RECVBUFFER.writeIdx = 0;
    RECVBUFFER.size = 0;

    /* setup baudrate - 20Mhz, U2X, 4800 bps */
    UBRR1H = (uint8_t)(520 >> 8);
    UBRR1L = (uint8_t)(520 >> 0);

    UCSR1A |= ((1 << U2X1));
    UCSR1B |= ((1 << RXCIE1) | (0 << UDRIE1) | (1 << RXEN1) | (1 << TXEN1));
    UCSR1C |= ((1 << UCSZ11) | (1 << UCSZ10));
}

void RS232_SEND(char *data, uint8_t size)
{
    uint8_t counter = 0;

    /* wait until buffer has enaugh free space */
    while((0xFF - SENDBUFFER.size) <= size);

    /* add data to buffer */
    for(counter = 0; counter < size; counter++)
    {
        SENDBUFFER.buffer[SENDBUFFER.writeIdx] = data[counter];
        SENDBUFFER.writeIdx++;
    }

    SENDBUFFER.size = SENDBUFFER.size + size;

    /* trigger new transmission start */
    if(!(UCSR1B & (1 << UDRIE1)))
    {
        /* start new transmission */

        /* clear flag and enable interrupt */
        UCSR1A |= ((1 << TXC1));
        UCSR1B |= ((1 << UDRIE1));
    }
}

uint8_t RS232_RXSIZE(void)
{
    return RECVBUFFER.size;
}

void RS232_FETCH(char *data, uint8_t size)
{
    uint8_t counter = 0;

    while(RECVBUFFER.size < size);

    for(counter = 0; counter < size; counter++)
    {
        data[counter] = RECVBUFFER.buffer[RECVBUFFER.readIdx];
        RECVBUFFER.readIdx++;
    }

    RECVBUFFER.size = RECVBUFFER.size - size;
}

void RS232_TX_CLEAR(void)
{
    SENDBUFFER.size = 0;
}

void RS232_RX_CLEAR(void)
{
    RECVBUFFER.size = 0;
}

ISR(USART1_RX_vect)
{
    if(RECVBUFFER.size != 0xFF)
    {
        RECVBUFFER.buffer[RECVBUFFER.writeIdx] = UDR1;
        RECVBUFFER.writeIdx++;
        RECVBUFFER.size++;
    }
}

ISR(USART1_UDRE_vect)
{
    if(SENDBUFFER.size != 0x00)
    {
        UDR1 = SENDBUFFER.buffer[SENDBUFFER.readIdx];
        SENDBUFFER.readIdx++;
        SENDBUFFER.size--;
        return;
    }

    UCSR1B &= ~((1 << UDRIE1));
}
