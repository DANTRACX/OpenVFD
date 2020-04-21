#include "TIME.h"

static volatile union TIMEBASE_u
{
    volatile uint64_t time;
    volatile uint16_t clock[4];
    #define  clockreg clock[0]
}
TIMEBASE;

void TIME_INIT(void)
{
    cli();

    TIMEBASE.time = 0;
    TCNT1 = 0x0000;

    TIMSK1 |= (1 << TOIE1);
    TCCR1A  = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
    TCCR1B  = (0 <<  ICNC1) | (0 <<  ICES1) | (0 <<  WGM13) | (0 <<  WGM12) | (0 <<  CS12) | (0 <<  CS11) | (1 <<  CS10);

    sei();
}

void TIME_SET(TIME_s *ptr, uint64_t time_us)
{
    TIMEBASE.clockreg = TCNT1;
    ptr->time = TIMEBASE.time + (time_us * 20);
}

uint8_t TIME_CHECKEXP(TIME_s *ptr)
{
    TIMEBASE.clockreg = TCNT1;

    if((ptr->time - TIMEBASE.time) >= ptr->time)
    {
        return 1;
    }

    return 0;
}

uint64_t TIME_GETREMAINS(TIME_s *ptr)
{
    TIMEBASE.clockreg = TCNT1;
    uint64_t timeRemain = ptr->time - TIMEBASE.time;

    if(timeRemain >= ptr->time)
    {
        return 0;
    }

    return (timeRemain / 20);
}

ISR(TIMER1_OVF_vect)
{
    TIMEBASE.time = TIMEBASE.time + 0x10000;
}
