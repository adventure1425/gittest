#include "Relay.h"
#include "tim.h"
extern uint16_t Relay_work;
static void sendData(int flag);
extern teESP_TaskStatus ESP_TaskStatus;
int Relay_Flag=0;
int R_OPEN_SET=0;
void Relay_Task()
{
	if(Relay_Flag ==1&&R_OPEN_SET==0)
	{
		Relay_Start();
	}
	if(Relay_Flag==0&&R_OPEN_SET==1)
	{
		Relay_Stop();
	}
	if(Relay_Flag==1&&R_OPEN_SET==1)
	{
		if(Relay_work==5)
		{
		 HAL_TIM_Base_Stop(&htim4);
			Relay_work=0;
			Relay_Stop();
		}
	}	
	
}
void Relay_Start()
{
	printf("浇水启动**************************\r\n");
	HAL_GPIO_WritePin(Electric_Relay_GPIO_Port,Electric_Relay_Pin,GPIO_PIN_SET);
	R_OPEN_SET=1;
    HAL_TIM_Base_Start(&htim4);
	sendData(R_OPEN_SET);
	
}
void Relay_Stop()
{
	HAL_GPIO_WritePin(Electric_Relay_GPIO_Port,Electric_Relay_Pin,GPIO_PIN_RESET);
	R_OPEN_SET=0;
	sendData(R_OPEN_SET);
}
void sendData(int flag){
		char msg[50];         //字符串消息
		if(ESP_TaskStatus==ESP_ACCESS)
		{
		sprintf(msg,"AT+MQTTPUB=0,\"step\",\"%d\",1,1\r\n",flag);
		HAL_UART_Transmit(&huart3,(uint8_t*)msg,sizeof(msg),0xff);
		}
}