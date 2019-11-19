#ifndef __RS485_H__
#define __RS485_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


extern void RS485_INIT(void);
extern void RS485_SEND(char *data, uint8_t size);
extern void RS485_FETCH(char *data, uint8_t size);
extern void RS485_TX_CLEAR(void);
extern void RS485_RX_CLEAR(void);
extern ISR(USART0_RX_vect);
extern ISR(USART0_UDRE_vect);


#endif
