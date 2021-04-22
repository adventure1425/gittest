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
typedef enum   //AT指令状态机
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
	AT_CWMODE,         //设置为STA
	//AT_RST,  //  重启 我们不使用这个直接使用硬件复位
	AT_RST,
	//AT_CWJAP,      //是否已经连接网络
	//AT_CWJAP1,    //连接网络
	AT_MQTTUSERCFG, 	  //设置MQTT连接属性
	AT_MQTTCONNCFG,
	AT_MQTTCONN,
	AT_SUB1,	//连接MQTT
	AT_SUB2,	//连接MQTT
	AT_SUB3,	//连接MQTT
	//AT_SUB4,	//连接MQTT
	AT_PUB,	//连接MQTT
	AT_SUB,	    //连接MQTT
	AT_CWSTARTSMART,
	AT_CWAUTOCONN,
    //测试是否已经连接mqtt
	//AT_PUB, 	//设置自动联网
	
}teATCmdNum;
/*定时器相关*/
typedef struct
{
	uint32_t TimeStart;
	uint32_t TimeInter;
}tsTimeType;
void ATRec(void);   //指令返回
void ESP_Init(void);
void Rec_WaitAT(void);
void ESP_Task(void);
void ESP_RESET(void);
void AT_SUBM(void);
void SetTime(tsTimeType *TimeType,uint32_t TimeInter);
int Find_string(char *pcBuf,char *left,char *right, char *pcRes);
uint8_t  CompareTime(tsTimeType *TimeType);
#endif


