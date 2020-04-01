#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


typedef struct TIME_s
{
    uint64_t time;
}
TIME_s;


extern void TIME_INIT(void);
extern void TIME_SET(TIME_s *ptr, uint64_t time_us);
extern uint8_t TIME_CHECKEXP(TIME_s *ptr);
extern uint64_t TIME_GETREMAINS(TIME_s *ptr);
extern ISR(TIMER1_OVF_vect);

#endif
