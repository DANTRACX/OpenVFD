#include "SVPWM.h"

static struct SVPWM_CMDFRAME_s
{
    uint8_t  cmdMask;
    uint8_t  rotationDirection;
    uint8_t  magnitude;
    uint16_t frequency;
    uint16_t phase;
    uint8_t  pwmFrequency;
    uint8_t  deadtimeTimings;
    uint8_t  deadtimePrescaler;
}
SVPWM_CMDFRAME_QUEUE;

static uint8_t SVPWM_CMDFRAME_SEND[20];
static uint8_t SVPWM_CMDFRAME_SIZE;


__attribute__((always_inline))
static inline void _svpwm_sendframe(void)
{
    if(SVPWM_CMDFRAME_SIZE == 0)
    {
        __builtin_avr_delay_cycles(100);
        PORTB |= (1 << PB4);
        return;
    }

    SPDR = SVPWM_CMDFRAME_SEND[SVPWM_CMDFRAME_SIZE];
    SVPWM_CMDFRAME_SIZE--;
}

void SVPWM_INIT(void)
{
    /* setup fault protection pin */
    DDRD  |=  ((1 << PD6));
    PORTD &= ~((1 << PD6));

    /* setup spi pins - miso,mosi,sck,ss */
    DDRB  |=  ((1 << DDB4) | (1 << DDB5) | (1 << DDB7));
    DDRB  &= ~((1 << DDB6));
    PORTB |=  (1 << PB4);
    PORTB &= ~((1 << PB5) | (1 << PB7));

    /* setup dataspace */
    SVPWM_CMDFRAME_SIZE = 0;
    SVPWM_CMDFRAME_QUEUE.cmdMask = 0x00;

    /* setup spi interface */
    SPCR = 0b11010011;
    SPSR = 0b00000000;
}


void SVPWM_START(void)
{
    /* disengange fault protection */
    PORTD |=  (1 << PD6);
}


void SVPWM_STOP(void)
{
    /* engange fault protection */
    PORTD &= ~(1 << PD6);
}


void SVPWM_QUEUE_SEND(void)
{
    uint8_t *ptr = &(SVPWM_CMDFRAME_SEND[1]);

    /* wait until data buffer has been sent */
    while(SVPWM_CMDFRAME_SIZE != 0);

    /* copy queue to sendbuffer */
    *(ptr - 1) = 0x00;

    if(SVPWM_CMDFRAME_QUEUE.cmdMask & (1 << 1))
    {
        /* copy rotation direction */
        *(ptr + 0) = SVPWM_CMDFRAME_QUEUE.rotationDirection;
        *(ptr + 1) = 0x01;
        ptr = ptr + 2;
        SVPWM_CMDFRAME_SIZE = SVPWM_CMDFRAME_SIZE + 2;
    }

    if(SVPWM_CMDFRAME_QUEUE.cmdMask & (1 << 2))
    {
        /* copy magnitude */
        *(ptr + 0) = SVPWM_CMDFRAME_QUEUE.magnitude;
        *(ptr + 1) = 0x02;
        ptr = ptr + 2;
        SVPWM_CMDFRAME_SIZE = SVPWM_CMDFRAME_SIZE + 2;
    }

    if(SVPWM_CMDFRAME_QUEUE.cmdMask & (1 << 3))
    {
        /* copy frequency */
        *(ptr + 0) = (uint8_t)(((SVPWM_CMDFRAME_QUEUE.frequency) >> 8) & 0x00FF);
        *(ptr + 1) = (uint8_t)(((SVPWM_CMDFRAME_QUEUE.frequency) >> 0) & 0x00FF);
        *(ptr + 2) = 0x03;
        ptr = ptr + 3;
        SVPWM_CMDFRAME_SIZE = SVPWM_CMDFRAME_SIZE + 3;
    }

    if(SVPWM_CMDFRAME_QUEUE.cmdMask & (1 << 4))
    {
        /* copy phase */
        *(ptr + 0) = (uint8_t)(((SVPWM_CMDFRAME_QUEUE.phase) >> 8) & 0x00FF);
        *(ptr + 1) = (uint8_t)(((SVPWM_CMDFRAME_QUEUE.phase) >> 0) & 0x00FF);
        *(ptr + 2) = 0x04;
        ptr = ptr + 3;
        SVPWM_CMDFRAME_SIZE = SVPWM_CMDFRAME_SIZE + 3;
    }

    if(SVPWM_CMDFRAME_QUEUE.cmdMask & (1 << 5))
    {
        /* copy pwm carrier frequency */
        *(ptr + 0) = SVPWM_CMDFRAME_QUEUE.pwmFrequency;
        *(ptr + 1) = 0x05;
        ptr = ptr + 2;
        SVPWM_CMDFRAME_SIZE = SVPWM_CMDFRAME_SIZE + 2;
    }

    if(SVPWM_CMDFRAME_QUEUE.cmdMask & (1 << 6))
    {
        /* copy deadtime timings */
        *(ptr + 0) = SVPWM_CMDFRAME_QUEUE.deadtimeTimings;
        *(ptr + 1) = 0x06;
        ptr = ptr + 2;
        SVPWM_CMDFRAME_SIZE = SVPWM_CMDFRAME_SIZE + 2;
    }

    if(SVPWM_CMDFRAME_QUEUE.cmdMask & (1 << 7))
    {
        /* copy deadtime prescaler */
        *(ptr + 0) = SVPWM_CMDFRAME_QUEUE.deadtimePrescaler;
        *(ptr + 1) = 0x07;
        ptr = ptr + 2;
        SVPWM_CMDFRAME_SIZE = SVPWM_CMDFRAME_SIZE + 2;
    }

    /* reset cmdMask */
    SVPWM_CMDFRAME_QUEUE.cmdMask = 0x00;

    /* trigger send loop */
    PORTB &= ~(1 << PB4);
    __builtin_avr_delay_cycles(100);
    _svpwm_sendframe();
}


void SVPWM_QUEUE_SET_ROTATION_DIRECTION(SVPWM_ROTATION_DIRECTION_e direction)
{
    /* clockwise rotation */
    if (direction <= 1)
    {
        SVPWM_CMDFRAME_QUEUE.rotationDirection = 0x00;
    }

    /* counterclockwise rotation */
    else
    {
        SVPWM_CMDFRAME_QUEUE.rotationDirection = 0x01;
    }

    SVPWM_CMDFRAME_QUEUE.cmdMask |= (1 << 1);
}


void SVPWM_QUEUE_SET_MAGNITUDE(uint8_t magnitude)
{
    SVPWM_CMDFRAME_QUEUE.magnitude = magnitude;
    SVPWM_CMDFRAME_QUEUE.cmdMask |= (1 << 2);
}


void SVPWM_QUEUE_SET_FREQUENCY(uint16_t frequency)
{
    SVPWM_CMDFRAME_QUEUE.frequency = frequency;
    SVPWM_CMDFRAME_QUEUE.cmdMask |= (1 << 3);
}


void SVPWM_QUEUE_SET_PHASE(uint16_t phase)
{
    SVPWM_CMDFRAME_QUEUE.phase = phase;
    SVPWM_CMDFRAME_QUEUE.cmdMask |= (1 << 4);
}


void SVPWM_QUEUE_SET_PWM_FREQUENCY(SVPWM_PWM_FREQUENCY_e fpwm)
{
    SVPWM_CMDFRAME_QUEUE.pwmFrequency = fpwm + 1;
    SVPWM_CMDFRAME_QUEUE.cmdMask |= (1 << 5);
}


void SVPWM_QUEUE_SET_DEADTIME_TIMINGS(uint8_t timingH, uint8_t timingL)
{
    if((timingH != 0) && (timingL != 0) && (timingH < 16) && (timingL < 16))
    {
        SVPWM_CMDFRAME_QUEUE.deadtimeTimings = ((timingH << 4) | (timingL & 0x0F));
        SVPWM_CMDFRAME_QUEUE.cmdMask |= (1 << 6);
    }
}


void SVPWM_QUEUE_SET_DEADTIME_PRESCALER(SVPWM_DEADTIME_PRESCALER_e prescaler)
{
    SVPWM_CMDFRAME_QUEUE.deadtimePrescaler = prescaler;
    SVPWM_CMDFRAME_QUEUE.cmdMask |= (1 << 7);
}


ISR(SPI_STC_vect)
{
    _svpwm_sendframe();
}
