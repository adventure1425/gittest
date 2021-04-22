/********************************************************
 *
 * @file    button.c
 * @brief   button驱动
 * @author  Kobe
 * @date  
 * @note    菜单控制，带电容按键检测 无电容按键检测
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
  * 函数功能: 按键检测函数/菜单控制
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：定时器中运行 ms测按键状态  进行消抖检测
  ***************************************************************************/
void Key_Scan()
{
	
         /*检测无电容按键是否按下 */
	if(HAL_GPIO_ReadPin(KEY_Ctrol_GPIO_Port,KEY_Ctrol_Pin) == GPIO_PIN_SET )
	{
		 sw1Count++;                         //SW1键按下，计数sw1Count加1
		 if(sw1Count>=32)                    //1MS中断一次，sw1Count大于等于32，即按键已按下32ms
		 {
				if(pushFlag1==0)                //判断有没有重按键，1为有，0为没有
				{
					swState1=1;                       //设置按键标志
                    //printf("*************(****\r\n");		
                    menu_flag++;			 //按键控制菜单			
					sw1Count=0;
					pushFlag1=1;                     //设置重按键标志
				}
				else
					sw1Count=0;
		 }
		 else
				swState1=0;
	}
	else                                            //无按键按下
	{
		 sw1Count=0;                           //清零sw1Count
		 swState1=0;                            //清除按键标志
		 pushFlag1=0;                          //清除重按键标志
	}
	/*检测有电容按键是否按下 */
	if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_SET)
	{
		char msg[50];         //字符串消息
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
    /*检测有电容按键是否按下 */
	if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_SET)
	{
			char msg[50];         //字符串消息
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

