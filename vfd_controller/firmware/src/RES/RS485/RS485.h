#ifndef __RS485_H__
#define __RS485_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>


extern  void        RS485_INIT(uint8_t addr, uint32_t baud, uint8_t parity);
extern  void        RS485_SEND(char *data, uint8_t size);
extern  uint8_t     RS485_RXSIZE(void);
extern  void        RS485_FETCH(char *data, uint8_t size);
extern  void        RS485_TX_CLEAR(void);
extern  void        RS485_RX_CLEAR(void);
extern  ISR(TIMER3_COMPB_vect);
extern  ISR(USART0_RX_vect);
extern  ISR(USART0_UDRE_vect);


#endif
