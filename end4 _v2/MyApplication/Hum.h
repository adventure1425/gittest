#ifndef _HUM_H_
#define _HUM_H_
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f1xx.h"
#include "core_delay.h"
#include "stdio.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include "gpio.h"
#include "esp.h"
void Hum_Task(void);
void Get_Hum_Voltage(void);

#endif

