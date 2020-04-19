#include "RS485.h"

#define RS232_F_CPU 20000000UL

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
    volatile char buffer[512];
    volatile uint16_t readIdx;
    volatile uint16_t writeIdx;
    volatile uint16_t size;
    volatile uint16_t pendingSize;
}
RECVBUFFER;

static volatile struct RS485STATES_s
{
    volatile enum
    {
        WAIT_ROUNDTRIP   = 0x01,
        WAIT_FRAMESTART  = 0x02,
        WAIT_RXFRAMEDATA = 0x04,
        WAIT_TXFRAMEDATA = 0x08
    }
    state;

    uint8_t address;
}
RS485STATES;

__attribute__((always_inline))
static inline void _rs485_timer_start(void)
{
    TCCR3B |= (1 << CS31);
}

__attribute__((always_inline))
static inline void _rs485_timer_stop(void)
{
    TCCR3B &=  ~(1 << CS31);
}

__attribute__((always_inline))
static inline void _rs485_timer_reset(void)
{
    TCNT3 = 0x0000;
}

__attribute__((always_inline))
static inline void _rs485_reset_wrong_byte_distance(void)
{
    TIFR3 |= (1 << OCF3A);
}


void RS485_INIT(uint8_t addr, uint32_t baud, uint8_t parity)
{
    /* initialize sendbuffer */
    SENDBUFFER.readIdx = 0;
    SENDBUFFER.writeIdx = 0;
    SENDBUFFER.size = 0;

    /* initialize recvbuffer */
    RECVBUFFER.readIdx = 0;
    RECVBUFFER.writeIdx = 0;
    RECVBUFFER.size = 0;

    /* initialize states */
    RS485STATES.address = addr;
    RS485STATES.state = WAIT_ROUNDTRIP;

    /* setup baudrate - F_CPU, U2X, */
    uint32_t baudrate  = (((RS232_F_CPU           ) / 8) / baud);
    uint64_t bytedist  = (((RS232_F_CPU * 11 * 2.5) / 8) / baud);
    uint64_t framedist = (((RS232_F_CPU * 11 * 4.5) / 8) / baud);

    /* initialize RS485 rx/tx switch */
    DDRB  |= (1 <<   DDB0);
    PORTB |= (1 << PORTB0);

    /* initialize uart */
    UBRR0H = (uint8_t)(baudrate >> 8);
    UBRR0L = (uint8_t)(baudrate >> 0);

    switch(parity)
    {
        case 0:
        {
            UCSR0C = ((0 << UMSEL01) | (0 << UMSEL00) | (0 << UPM01) | (0 << UPM00) | (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0));
            break;
        }
        case 1:
        {
            UCSR0C = ((0 << UMSEL01) | (0 << UMSEL00) | (1 << UPM01) | (1 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0));
            break;
        }
        case 2:
        {
            UCSR0C = ((0 << UMSEL01) | (0 << UMSEL00) | (1 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0));
            break;
        }
        default:
        {
            UCSR0C = ((0 << UMSEL01) | (0 << UMSEL00) | (1 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0));
            break;
        }
    }

    UCSR0A  = ((0 <<   RXC0) | (0 <<   TXC0) | (0 <<  UDRE0) | (0 <<    FE0) | (0 <<   DOR0) | (0 <<   UPE0) | (1 <<  U2X0) | (0 << MPCM0));
    UCSR0B  = ((1 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (1 <<  RXEN0) | (1 <<  TXEN0) | (0 << UCSZ02) | (0 << RXB80) | (0 << TXB80));

    /* initialize timer */
    OCR3A   = (uint16_t)(bytedist);
    OCR3B   = (uint16_t)(framedist);
    TCCR3A  = ((0 << COM3A1) | (0 << COM3A0) | (0 << COM3B1) | (0 << COM3B0) | (0 <<  WGM31) | (0 <<  WGM30));
    TCCR3B  = ((0 <<  ICNC3) | (0 <<  ICES3) | (0 <<  WGM33) | (0 <<  WGM32) | (0 <<   CS32) | (0 <<   CS31) | (0 <<  CS30));
    TIMSK3 |= ((1 << OCIE3B));
    _rs485_timer_start();
}

void RS485_SEND(char *data, uint8_t size)
{
    uint8_t counter = 0;

    /* wait until buffer has enaugh free space */
    wdt_reset();
    while((0xFF - SENDBUFFER.size) <= size);

    /* add data to buffer */
    for(counter = 0; counter < size; counter++)
    {
        SENDBUFFER.buffer[SENDBUFFER.writeIdx] = data[counter];
        SENDBUFFER.writeIdx++;
    }

    SENDBUFFER.size = SENDBUFFER.size + size;

    /* trigger new transmission start */
    if(!(UCSR0B & (1 << UDRIE0)))
    {
        while(RS485STATES.state != WAIT_FRAMESTART);

        RS485STATES.state = WAIT_TXFRAMEDATA;

        /* clear flag and enable interrupt */
        PORTB  &= ~(1 << PORTB0);
        UCSR0A |= ((1 <<   TXC0));
        UCSR0B |= ((1 << UDRIE0));
    }
}

uint8_t RS485_RXSIZE(void)
{
    return RECVBUFFER.size;
}

void RS485_FETCH(char *data, uint8_t size)
{
    uint8_t counter = 0;

    wdt_reset();
    while(RECVBUFFER.size < size);

    for(counter = 0; counter < size; counter++)
    {
        data[counter] = RECVBUFFER.buffer[RECVBUFFER.readIdx];
        RECVBUFFER.readIdx++;
    }

    RECVBUFFER.size = RECVBUFFER.size - size;
}

void RS485_TX_CLEAR(void)
{
    SENDBUFFER.size = 0;
    SENDBUFFER.readIdx = 0;
    SENDBUFFER.writeIdx = 0;
}

void RS485_RX_CLEAR(void)
{
    RECVBUFFER.size = 0;
    RECVBUFFER.pendingSize = 0;
    RECVBUFFER.readIdx = 0;
    RECVBUFFER.writeIdx = 0;
}


/*
 * COMPA : < 1.5 byte durations passed
 * COMPB : > 3.5 byte durations passed
 */
ISR(TIMER3_COMPB_vect)
{
    /* interrupts fires on long time bus idle - indicates end of last frame, start of new one */
    RECVBUFFER.size = RECVBUFFER.pendingSize;
    RECVBUFFER.pendingSize = 0;
    RS485STATES.state = WAIT_FRAMESTART;
    _rs485_timer_stop();
    _rs485_reset_wrong_byte_distance();
}

ISR(USART0_RX_vect)
{
    _rs485_timer_reset();
    _rs485_timer_start();

    /* modbus waits for beginning roundtrip delay */
    if((RS485STATES.state == WAIT_ROUNDTRIP) || (RS485STATES.state == WAIT_TXFRAMEDATA))
    {
        RECVBUFFER.buffer[RECVBUFFER.writeIdx] = UDR0;
        return;
    }

    /* >1.5 between bytes, buffer full, parity error or frame error - delete pending frame */
    if((TIFR3 & (1 << OCF3A)) || (UCSR0A & (1 << FE0)) || (UCSR0A & (1 << UPE0)) || ((RECVBUFFER.size + RECVBUFFER.pendingSize) > 512))
    {
        RECVBUFFER.buffer[RECVBUFFER.writeIdx] = UDR0;
        RECVBUFFER.writeIdx = (RECVBUFFER.writeIdx - RECVBUFFER.pendingSize) & 0x01FF;
        RECVBUFFER.pendingSize = 0;
        RS485STATES.state = WAIT_ROUNDTRIP;
        _rs485_reset_wrong_byte_distance();
        return;
    }

    RECVBUFFER.buffer[RECVBUFFER.writeIdx] = UDR0;

    /* modbus slave waits for beeing adressed by master */
    if(RS485STATES.state == WAIT_FRAMESTART)
    {
        /* check if slave is beeing adressed - broadcast msg 0 omitted at this point */
        if((RECVBUFFER.buffer[RECVBUFFER.writeIdx] == RS485STATES.address))
        {
            RECVBUFFER.writeIdx = (RECVBUFFER.writeIdx + 1) & 0x01FF;
            RECVBUFFER.pendingSize++;
            RS485STATES.state = WAIT_RXFRAMEDATA;
            return;
        }

        /* slave was not adressed - restart with roundtrip delay */
        else
        {
            RS485STATES.state = WAIT_ROUNDTRIP;
            return;
        }
    }

    /* slaves receives all parts of a frame after successfull adressing */
    else
    {
        RECVBUFFER.writeIdx = (RECVBUFFER.writeIdx + 1) & 0x01FF;
        RECVBUFFER.pendingSize++;
    }
}

ISR(USART0_UDRE_vect)
{
    /* until empty buffer send byte */
    if(SENDBUFFER.size != 0x0000)
    {
        UDR0 = SENDBUFFER.buffer[SENDBUFFER.readIdx];
        SENDBUFFER.readIdx = (SENDBUFFER.readIdx + 1) & 0x01FF;
        SENDBUFFER.size--;
        return;
    }

    /*on empty buffer - disable isr, switch to rx */
    UCSR0B &= ~((1 << UDRIE0));
    PORTB  |=   (1 << PORTB0);

    /* restart timer for roundtrip */
    _rs485_reset_wrong_byte_distance();
    _rs485_timer_reset();
    _rs485_timer_start();

    RS485STATES.state = WAIT_ROUNDTRIP;
}
