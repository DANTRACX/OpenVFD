#ifndef __SCPI_H__
#define __SCPI_H__

#include <stdint.h>
#include <avr/pgmspace.h>

extern  const char  SCPI_INFO[] PROGMEM;

extern  void        SCPI_INIT(void);
extern  void        SCPI_PROCESS(void);

#endif
