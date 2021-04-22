/********************************************************
 *
 * @file    step.c
 * @brief   步进电机驱动
 * @author  Kobe
 * @date    
 * @note    步进电机控制
 *
********************************************************/
#include "step.h"
#include "ds18b20.h"
static void sendData(int flag);
int Step_Flag=0;
int OPEN_SET=0;
extern teESP_TaskStatus ESP_TaskStatus;
/****************************************************************************
  * 函数功能: 步进电机任务函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：函数在循环中运行 
  ***************************************************************************/
void Step_Task()
{
	if(Step_Flag ==1&&OPEN_SET==0)
	{
		Step_open();
	}
	if(Step_Flag==0&&OPEN_SET==1)
	{
		Step_Close();
	
	}
}
/****************************************************************************
  * 函数功能: 步进电机开启函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：步进电机运行 开关标志置一 
  ***************************************************************************/
void Step_open()
{
	 uint8_t a=200;
	  while(a--)
	  {
    /* USER CODE END WHILE */
	   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
	  	  DS18B20_Delay_us(6000);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
	  	  DS18B20_Delay_us(6000);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
	  	  DS18B20_Delay_us(6000);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_SET);
	  	  DS18B20_Delay_us(6000);
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_RESET);
	  }
	  OPEN_SET=1;
	  sendData(OPEN_SET);
	
}
/****************************************************************************
  * 函数功能: 步进电机关闭函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：步进电机运行 开关标志置零 
  ***************************************************************************/
void Step_Close()
{
	 uint8_t a=200;
	  while(a--)
	  {
    /* USER CODE END WHILE */
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_SET);
	  	  DS18B20_Delay_us(6000);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
		  DS18B20_Delay_us(6000);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
	  	  DS18B20_Delay_us(6000);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
	  	  DS18B20_Delay_us(6000);
		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
	  }
	  OPEN_SET=0;
	  sendData(OPEN_SET);
	
}
void sendData(int flag){
		char msg[50];         //字符串消息
		if(ESP_TaskStatus==ESP_ACCESS)
		{
		sprintf(msg,"AT+MQTTPUB=0,\"step\",\"%d\",1,1\r\n",flag);
		HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
		}
}

