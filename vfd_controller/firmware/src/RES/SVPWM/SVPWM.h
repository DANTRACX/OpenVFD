#ifndef __SVPWM_H__
#define __SVPWM_H__


#include <stdint.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>


typedef enum SVPWM_ROTATION_DIRECTION_e
{
        CW,
        CLOCKWISE,
        CCW,
        COUNTERCLOCKWISE
}
SVPWM_ROTATION_DIRECTION_e;

typedef enum SVPWM_PWM_FREQUENCY_e
{
        KHZ_1,
        KHZ_2,
        KHZ_4,
        KHZ_8,
        KHZ_16
}
SVPWM_PWM_FREQUENCY_e;

typedef enum SVPWM_DEADTIME_PRESCALER_e
{
        X1,
        X2,
        X3,
        X4
}
SVPWM_DEADTIME_PRESCALER_e;


extern  void    SVPWM_INIT(void);
extern  void    SVPWM_START(void);
extern  void    SVPWM_STOP(void);
extern  void    SVPWM_QUEUE_SEND(void);
extern  void    SVPWM_QUEUE_SET_ROTATION_DIRECTION(SVPWM_ROTATION_DIRECTION_e direction);
extern  void    SVPWM_QUEUE_SET_MAGNITUDE(uint8_t magnitude);
extern  void    SVPWM_QUEUE_SET_FREQUENCY(uint16_t frequency);
extern  void    SVPWM_QUEUE_SET_PHASE(uint16_t phase);
extern  void    SVPWM_QUEUE_SET_PWM_FREQUENCY(SVPWM_PWM_FREQUENCY_e fpwm);
extern  void    SVPWM_QUEUE_SET_DEADTIME_TIMINGS(uint8_t timingH, uint8_t timingL);
extern  void    SVPWM_QUEUE_SET_DEADTIME_PRESCALER(SVPWM_DEADTIME_PRESCALER_e prescaler);
extern  ISR(SPI_STC_vect);

#endif
