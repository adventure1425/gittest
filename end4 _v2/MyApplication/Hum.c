/********************************************************
 *
 * @file    Hum.c
 * @brief  ʪ������
 * @author  Kobe
 * @date    
 * @note    ADC�ɼ�
 *
********************************************************/
#include "Hum.h"
int ADC_Value;
int Hum_Flag=0;
extern teESP_TaskStatus ESP_TaskStatus;
/****************************************************************************
  * ��������: ����ADC�ɼ�  ��ȡ����ʪ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  ***************************************************************************/
void Get_Hum_Voltage() 
{	
	HAL_ADC_Start(&hadc1);	
	if(HAL_ADC_PollForConversion(&hadc1,10) == HAL_OK)   //��ѯ���
	{
	     ADC_Value    = HAL_ADC_GetValue(&hadc1);
		 ADC_Value=100-(int)((ADC_Value-1400)/52);
		//printf("ADת��ԭʼֵ = %d\r\n",ADC_Value);
	}
}
/****************************************************************************
  * ��������: �ɼ�������
  * �������: ��
  * �� �� ֵ: ��
* ˵    ����������ѭ��������   ����־λ  ��ʱ��������
  ***************************************************************************/
void Hum_Task()
{
	char msg[50];         //�ַ�����Ϣ
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

