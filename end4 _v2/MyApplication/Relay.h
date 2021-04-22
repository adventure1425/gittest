#ifndef __RELAY_H__
#define __RELAY_H__

#include "stm32f1xx.h"
#include "core_delay.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include "gpio.h"
#include "esp.h"
void Relay_Start(void);
void Relay_Task(void);
void Relay_Stop(void);
#endif


