#ifndef __RS232_H__
#define __RS232_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


extern void RS232_INIT(void);
extern void RS232_SEND(char *data, uint8_t size);
extern void RS232_FETCH(char *data, uint8_t size);
extern void RS232_TX_CLEAR(void);
extern void RS232_RX_CLEAR(void);
extern ISR(USART0_RX_vect);
extern ISR(USART0_UDRE_vect);


#endif
