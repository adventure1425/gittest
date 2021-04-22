/********************************************************
 *
 * @file    button.c
 * @brief   button����
 * @author  Kobe
 * @date  
 * @note    �˵����ƣ������ݰ������ �޵��ݰ������
 *
********************************************************/
#include "button.h"
#include "tim.h"
uint8_t sw1Count;
uint8_t pushFlag1;
extern int menu_flag;
extern int maxtemp;
extern int mintemp;
extern int maxhum;
extern int minhum; 
extern teESP_TaskStatus ESP_TaskStatus;
int swState1=0;
/****************************************************************************
  * ��������: ������⺯��/�˵�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������ʱ�������� ms�ⰴ��״̬  �����������
  ***************************************************************************/
void Key_Scan()
{
	
         /*����޵��ݰ����Ƿ��� */
	if(HAL_GPIO_ReadPin(KEY_Ctrol_GPIO_Port,KEY_Ctrol_Pin) == GPIO_PIN_SET )
	{
		 sw1Count++;                         //SW1�����£�����sw1Count��1
		 if(sw1Count>=32)                    //1MS�ж�һ�Σ�sw1Count���ڵ���32���������Ѱ���32ms
		 {
				if(pushFlag1==0)                //�ж���û���ذ�����1Ϊ�У�0Ϊû��
				{
					swState1=1;                       //���ð�����־
                    //printf("*************(****\r\n");		
                    menu_flag++;			 //�������Ʋ˵�			
					sw1Count=0;
					pushFlag1=1;                     //�����ذ�����־
				}
				else
					sw1Count=0;
		 }
		 else
				swState1=0;
	}
	else                                            //�ް�������
	{
		 sw1Count=0;                           //����sw1Count
		 swState1=0;                            //���������־
		 pushFlag1=0;                          //����ذ�����־
	}
	/*����е��ݰ����Ƿ��� */
	if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_SET)
	{
		char msg[50];         //�ַ�����Ϣ
		while(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_SET);
	    switch(menu_flag)
		{
			case 1:
				break;
			case 2:
              maxtemp++; 
		    if(ESP_TaskStatus==ESP_ACCESS)
		    {
				sprintf(msg,"AT+MQTTPUB=0,\"maxT\",\"%d\",1,1\r\n",maxtemp);
				HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
				//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
		     }
				break;
			case 3:
				if(mintemp+1<maxtemp)
				{
					mintemp++;
				}
			    if(ESP_TaskStatus==ESP_ACCESS)
				{
					sprintf(msg,"AT+MQTTPUB=0,\"minT\",\"%d\",1,1\r\n",mintemp);
					HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
					//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
				 }
				break;
			case 4:
				maxhum++;
				if(ESP_TaskStatus==ESP_ACCESS)
				{
					sprintf(msg,"AT+MQTTPUB=0,\"maxH\",\"%d\",1,1\r\n",maxhum);
					HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
					//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
				 }
				break;
			case 5:
				if(minhum+1<maxtemp)
				{
					minhum++;
				}
                if(ESP_TaskStatus==ESP_ACCESS)
				{
					sprintf(msg,"AT+MQTTPUB=0,\"minH\",\"%d\",1,1\r\n",minhum);
					HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
					//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
				 }
				break;
			default:
				break;
		
		}
	
	}
    /*����е��ݰ����Ƿ��� */
	if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_SET)
	{
			char msg[50];         //�ַ�����Ϣ
		while(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_SET);
	    switch(menu_flag)
		{
			case 1:
				break;
			case 2:
			if(maxtemp>mintemp+1)
				{
					maxtemp--;
				}
			if(ESP_TaskStatus==ESP_ACCESS)
		    {
				sprintf(msg,"AT+MQTTPUB=0,\"maxT\",\"%d\",1,1\r\n",maxtemp);
				HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
				//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
		     }
				break;
			case 3:
			if(mintemp>0)
			{
				mintemp--;
			}
			   if(ESP_TaskStatus==ESP_ACCESS)
				{
					sprintf(msg,"AT+MQTTPUB=0,\"minT\",\"%d\",1,1\r\n",mintemp);
					HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
					//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
				 }
				break;
			case 4:
				if(maxhum>minhum+1)
				{
					maxhum--;
				}
			if(ESP_TaskStatus==ESP_ACCESS)
				{
					sprintf(msg,"AT+MQTTPUB=0,\"maxH\",\"%d\",1,1\r\n",maxhum);
					HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
					//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
				 }
				break;
			case 5:
				if(minhum>0)
				{
					minhum--;
				}
			if(ESP_TaskStatus==ESP_ACCESS)
				{
					sprintf(msg,"AT+MQTTPUB=0,\"minH\",\"%d\",1,1\r\n",minhum);
					HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
					//HAL_UART_Transmit(&huart3,(uint8_t*)"AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n",strlen("AT+MQTTPUB=0,\"TempData\",\"21\",1,0\r\n"),0xff);
				 }
				break;
			default:
				break;
		
		}
	
	}
}

