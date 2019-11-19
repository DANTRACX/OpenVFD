#ifndef __SENSE_H__
#define __SENSE_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/wdt.h>

extern void SENSE_INIT(void);
extern void SENSE_FETCH(uint16_t *sense1, uint16_t *sense2, uint16_t *sense3, uint16_t *sense4);

#endif
