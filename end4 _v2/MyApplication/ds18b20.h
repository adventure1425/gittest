/********************************************************
 *
 * @file    ds18b20.h
 * @brief   ds18b20驱动
 * @author  Mculover666
 * @date    2020/08/29
 * @note    此驱动中us级延时使用DWT
 *
********************************************************/

#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "stm32f1xx.h"
#include "core_delay.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include "gpio.h"
#include "esp.h"

/* DS18B20控制引脚 */
#define DS18B20_GPIO_PORT     GPIOE
#define DS18B20_GPIO_PIN      GPIO_PIN_6   

/* DS18B20控制IO模式配置 */
#define DS18B20_OutPut_Mode() {DS18B20_GPIO_PORT->CRL &= 0x0FFFFFFF;DS18B20_GPIO_PORT->CRL |= 0x30000000;}
#define DS18B20_InPut_Mode()  {DS18B20_GPIO_PORT->CRL &= 0x0FFFFFFF;DS18B20_GPIO_PORT->CRL |= 0x80000000;}

/* DS18B20控制IO操作函数 */
#define DS18B20_Out(n)        (n?HAL_GPIO_WritePin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN,GPIO_PIN_SET):HAL_GPIO_WritePin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN,GPIO_PIN_RESET))
#define DS18B20_In()           HAL_GPIO_ReadPin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN)

/* DS18B20 us 级延时函数 */
#define DS18B20_Delay_us(n)   CPU_TS_Tmr_Delay_US(n)

void DS18B20_Init(void);
int  DS18B20_Read_Temperature(void);
/*任务函数*/
void DS18B20_Task(void);

#endif /* _DS18B20_H_ */
