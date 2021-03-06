#ifndef __SENSE_H__
#define __SENSE_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/wdt.h>

#define SENSE_USE_CHANNEL_1
#define SENSE_USE_CHANNEL_2
#define SENSE_USE_CHANNEL_3
#define SENSE_USE_CHANNEL_4

#define SENSE_TCLK  15

extern  void        SENSE_INIT(void);
extern  void        SENSE_FETCH(uint16_t *sense1, uint16_t *sense2, uint16_t *sense3, uint16_t *sense4);

#endif
