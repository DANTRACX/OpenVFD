#include "TIME.h"

static volatile struct TIMEBASE_s
{
    volatile uint64_t time;
}
TIMEBASE;

void TIME_INIT(void)
{
    cli();

    TIMEBASE.time = 0;
    TCNT1H = 0x00;
    TCNT1L = 0x00;

    TIMSK1 |= (1 << TOIE1);
    TCCR1A  = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
    TCCR1B  = (0 <<  ICNC1) | (0 <<  ICES1) | (0 <<  WGM13) | (0 <<  WGM12) | (0 <<  CS12) | (0 <<  CS11) | (1 <<  CS10);

    sei();
}

void TIME_SET(TIME_s *ptr, uint64_t time_us)
{
    uint64_t tempTime = (((uint64_t)TCNT1H) << 8) | (((uint64_t)TCNT1L) << 0);

    ptr->time = (((TIMEBASE.time) & 0xFFFFFFFFFFFF0000) | (tempTime & 0x000000000000FFFF)) + (time_us * 20);
}

uint8_t TIME_CHECKEXP(TIME_s *ptr)
{
    uint64_t tempTime = (((uint64_t)TCNT1H) << 8) | (((uint64_t)TCNT1L) << 0);
    uint64_t timeRemain = ptr->time - (((TIMEBASE.time) & 0xFFFFFFFFFFFF0000) | (tempTime & 0x000000000000FFFF));

    if(timeRemain >= ptr->time)
    {
        return 1;
    }

    return 0;
}

uint64_t TIME_GETREMAINS(TIME_s *ptr)
{
    uint64_t tempTime = (((uint64_t)TCNT1H) << 8) | (((uint64_t)TCNT1L) << 0);
    uint64_t timeRemain = ptr->time - (((TIMEBASE.time) & 0xFFFFFFFFFFFF0000) | (tempTime & 0x000000000000FFFF));

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
