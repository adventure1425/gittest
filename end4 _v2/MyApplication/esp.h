#ifndef __ESP_H
#define __ESP_H
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include "gpio.h"
//#include "tim.h"
//#define SENDDATATIME 10*1000
typedef enum
{
	SUCCESS_REC=0,
	TIME_OUT,
	NO_REC,
	
}teATstatus;
typedef struct
{
	char *ATSendStr;
	char *ATRecStr;
	uint16_t TimeOut;
	teATstatus ATStatus;
	uint8_t RtyNum;
}tsATCmds;
typedef enum   //ATָ��״̬��
{
	ESP_IDIE = 0,
	ESP_SEND,
	ESP_WAIT,
	ESP_ACCESS,
	NO_WIFI,
	SUB_TOP,
}teESP_TaskStatus;
typedef enum
{
	AT = 0,
	ATE0,
	AT_CWMODE,         //����ΪSTA
	//AT_RST,  //  ���� ���ǲ�ʹ�����ֱ��ʹ��Ӳ����λ
	AT_RST,
	//AT_CWJAP,      //�Ƿ��Ѿ���������
	//AT_CWJAP1,    //��������
	AT_MQTTUSERCFG, 	  //����MQTT��������
	AT_MQTTCONNCFG,
	AT_MQTTCONN,
	AT_SUB1,	//����MQTT
	AT_SUB2,	//����MQTT
	AT_SUB3,	//����MQTT
	//AT_SUB4,	//����MQTT
	AT_PUB,	//����MQTT
	AT_SUB,	    //����MQTT
	AT_CWSTARTSMART,
	AT_CWAUTOCONN,
    //�����Ƿ��Ѿ�����mqtt
	//AT_PUB, 	//�����Զ�����
	
}teATCmdNum;
/*��ʱ�����*/
typedef struct
{
	uint32_t TimeStart;
	uint32_t TimeInter;
}tsTimeType;
void ATRec(void);   //ָ���
void ESP_Init(void);
void Rec_WaitAT(void);
void ESP_Task(void);
void ESP_RESET(void);
void AT_SUBM(void);
void SetTime(tsTimeType *TimeType,uint32_t TimeInter);
int Find_string(char *pcBuf,char *left,char *right, char *pcRes);
uint8_t  CompareTime(tsTimeType *TimeType);
#endif


