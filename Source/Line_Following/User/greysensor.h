#ifndef __GREYSENSOR__
#define __GREYSENSOR__

#include "gpio.h"

extern uint8_t L2, L1, Center, R1, R2;
extern uint8_t error;

void get_error(void);
#endif