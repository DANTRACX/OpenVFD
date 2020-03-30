#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <stdint.h>

extern void CONTROLLER_INIT(void);
extern void CONTROLLER_STEP_CYCLE(void);
extern uint8_t CONTROLLER_WAIT_CYCLE(void);

#endif
