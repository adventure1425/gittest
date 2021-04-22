/********************************************************
 *
 * @file    Hum.c
 * @brief  湿度驱动
 * @author  Kobe
 * @date    
 * @note    ADC采集
 *
********************************************************/
#include "Hum.h"
int ADC_Value;
int Hum_Flag=0;
extern teESP_TaskStatus ESP_TaskStatus;
/****************************************************************************
  * 函数功能: 开启ADC采集  获取土壤湿度
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  ***************************************************************************/
void Get_Hum_Voltage() 
{	
	HAL_ADC_Start(&hadc1);	
	if(HAL_ADC_PollForConversion(&hadc1,10) == HAL_OK)   //轮询检测
	{
	     ADC_Value    = HAL_ADC_GetValue(&hadc1);
		 ADC_Value=100-(int)((ADC_Value-1400)/52);
		//printf("AD转换原始值 = %d\r\n",ADC_Value);
	}
}
/****************************************************************************
  * 函数功能: 采集任务函数
  * 输入参数: 无
  * 返 回 值: 无
* 说    明：函数在循环中运行   检查标志位  定时发送数据
  ***************************************************************************/
void Hum_Task()
{
	char msg[50];         //字符串消息
	if(Hum_Flag)
	{
		
		Get_Hum_Voltage();
		if(ESP_TaskStatus==ESP_ACCESS)
		{
		sprintf(msg,"AT+MQTTPUB=0,\"HumpData\",\"%d\",1,1\r\n",ADC_Value);
		//printf("%s",msg);
		HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
		} 
	   Hum_Flag=0;



}
	}

