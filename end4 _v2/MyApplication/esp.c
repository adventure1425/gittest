 /*********************************************************
*   接线：                                             
*   GND ---> GND                                       
*   VCC ---> 3.3V                                      
*   TXD  ---> USART3—RX->PB11                                
*   RXD ---> USART3—TX->PB10                                
*   REST ---> ESP_RST_Pin->PA3                                  
*   IO—0--->3.3v                                  
*                                     
*                                                      
*/                                                   
/********************************************************/
#include "esp.h"
static uint8_t CurrentRty;    //当前次数
static tsTimeType TimeESP;
static teATCmdNum ATRecCmdNum;   //
static int SmartWificount=0;
int  WIFI_Flag=0;
teESP_TaskStatus ESP_TaskStatus;
teATCmdNum ATNextCmdNum;      //下一个指令                                              //下条指令
teATCmdNum ATCurrentCmdNum;   //当前指令
tsATCmds ATCmds[] =                                                        //AT指令列表
{
	{"AT\r\n","OK",100,NO_REC,100},                                        //回显测试
	{"ATE1\r\n","OK",100,NO_REC,100},                                       //关闭回显测试
    {"AT+CWMODE=1\r\n","OK",100,NO_REC,100},                               //设置为STA模式
	{"AT+RST\r\n","WIFI GOT IP",8000,NO_REC,100},  
	{"AT+MQTTUSERCFG=0,1,\"ESP8266\",\"admin\",\"wenyue123\",0,0,\"\"\r\n","OK",1000,NO_REC,100},     //设置mqtt属性	
	{"AT+MQTTCONNCFG=0,60,0,\"SOS\",\"1\",0,1\r\n","OK",1000,NO_REC,100},
	{"AT+MQTTCONN=0,\"8.131.82.156\",1883,0\r\n","MQTTCONNECTED",1000,NO_REC,100},
	{"AT+MQTTSUB=0,\"step\",1\r\n","OK",8000,NO_REC,100},
	{"AT+MQTTSUB=0,\"light\",1\r\n","OK",8000,NO_REC,100},
	{"AT+MQTTSUB=0,\"water\",1\r\n","OK",8000,NO_REC,100},
	{"AT+MQTTSUB=0,\"Start\",1\r\n","OK",8000,NO_REC,100},
	//{"AT+MQTTSUB=0,\"Cauto\",1\r\n","OK",2000,NO_REC,100},
	{"AT+MQTTPUB=0,\"SOS\",\"0\",1,1\r\n","OK",2000,NO_REC,100},
	{"AT+CWSTARTSMART=1\r\n","WIFI GOT IP",60000,NO_REC,100},
	{"AT+CWAUTOCONN=1\r\n","OK",200,NO_REC,100},	
}; 	
/****************************************************************************
  * 函数功能: 通过串口发送AT指令
  * 输入参数: ATCmdNum发送的第几个指令
  * 返 回 值: 无
  * 说    明：正常情况下无需控制  随着状态机的运行  指令会递加

  ***************************************************************************/
void ATSend(teATCmdNum ATCmdNum)
{
    ATCmds[ATCmdNum].ATStatus = NO_REC;   //设置为 未接收
    ATRecCmdNum = ATCmdNum;
	uint8_t len=strlen(ATCmds[ATCmdNum].ATSendStr);
	HAL_UART_Transmit(&huart3,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,len,0xff);
    printf("@@Send:%s\r\n",ATCmds[ATCmdNum].ATSendStr);
    SetTime(&TimeESP,ATCmds[ATCmdNum].TimeOut);
}
/****************************************************************************
  * 函数功能:WIFI状态机  实现WIFI的自动连接 以及wifi各种状态的自动切换（还不稳定）
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  ***************************************************************************/
void ESP_Task(void)
{
	while(1)
    {
        switch(ESP_TaskStatus)
        {
			/***发送状态 传递ATSEND当前指令   设置状态机为等待状态***/
        case ESP_SEND:                                 //发送状态
			WIFI_Flag=1;
            ATSend(ATCurrentCmdNum);                  //发送数据
            ESP_TaskStatus = ESP_WAIT;
            return;
			/***等待状态***/
        case ESP_WAIT:                                 //等待数据状态
             if(Usart3type.UsartRecFlag)  //串口收到数据
				  {
					  printf("@@Rev:%s\r\n",Usart3type.Usart3RecBuffer);
					  if(strstr((const char*)Usart3type.Usart3RecBuffer,ATCmds[ATRecCmdNum].ATRecStr) != NULL) //如果包含对应校验符
						  {
							  ATCmds[ATRecCmdNum].ATStatus = SUCCESS_REC;  //设置为成功接收
						  }
			     
				/***经过调试发现  串口原本没有清空  会出现错误判断  加上这个之后增加系统稳定性***/
					  memset(Usart3type.Usart3RecBuffer,0x00,sizeof(Usart3type.Usart3RecBuffer));
					  Usart3type.UsartRecFlag = 0;  //清空接收标志位
					  Usart3type.UsartRecLen = 0;   //清空接收长度
				}                                 //接收数据
			  /***数据发送成功之后的处理***/
             if(ATCmds[ATCurrentCmdNum].ATStatus == SUCCESS_REC) //成功数据包
				 {
					switch(ATCurrentCmdNum)
						{
							/***SUB指令是我定义的最后一个发送的指令  该指令成功则配网成功***/
							case AT_SUB:
								ESP_TaskStatus = ESP_ACCESS;
								break;
							/***手机配网之后 设置重启自动联网 然后重启***/
							case AT_CWAUTOCONN:
								ATCurrentCmdNum = AT_RST;
								ESP_TaskStatus = ESP_SEND;
								break;
							/***默认状态保持状态机运行***/
							default:
								ATCurrentCmdNum = (teATCmdNum)(ATCurrentCmdNum+1);
								ATNextCmdNum = (teATCmdNum)(ATCurrentCmdNum+1);
								ESP_TaskStatus = ESP_SEND;
								break;
						}
				
				 }else if(CompareTime(&TimeESP))           //还没收到预期数据  并且超时
					{	
						/***这里的判断是对于特殊指令手机配网的判断 该命令下不存在超时 直接死等***/
						if(ATCurrentCmdNum!=AT_CWSTARTSMART)  //如果是配网就不会有超时的概念只会一直等待
							{						
								printf("@@REV--TimeOut:%s\r\n",Usart3type.Usart3RecBuffer);
								ATCmds[ATCurrentCmdNum].ATStatus = TIME_OUT;
								if(CurrentRty > 0)  //重发
									{
										CurrentRty--;
										//printf("@@ now:%d,trytime:%d\r\n",ATCurrentCmdNum,CurrentRty);
										ATNextCmdNum = ATCurrentCmdNum;  //下一条 还是当前命令  实现重发效果
										ESP_TaskStatus = ESP_SEND;
									}else if(ATCurrentCmdNum==AT_RST)//重发次数用完
									    {		
											//ATCurrentCmdNum = AT;
											ESP_TaskStatus = NO_WIFI;
											return;
									
										}else{
											ESP_RESET();
											HAL_Delay(1000);
											ATCurrentCmdNum = AT;
											ESP_TaskStatus = ESP_SEND;
											return;
										}
							}else
							{
								if(SmartWificount<=10)
								{
									SmartWificount++;
									SetTime(&TimeESP,10000);
									ATCmds[ATCurrentCmdNum].ATStatus = NO_REC;
									printf("手动配网");
								//	ATNextCmdNum = ATCurrentCmdNum;  //下一条 还是当前命令  实现重发效果
									ESP_TaskStatus = ESP_WAIT;
								}else
								{
									SmartWificount=0;
									ESP_RESET();
									HAL_Delay(1000);
									ATCurrentCmdNum = AT;
									ESP_TaskStatus = ESP_SEND;
								}
							 }
					}
            return;
		case NO_WIFI:
			ATCurrentCmdNum = AT_CWSTARTSMART;
			printf("111111111111111111111111111111111111\r\n");
			ESP_TaskStatus = ESP_SEND;
			return;
        case ESP_ACCESS:
			return;
		default:
            return;
			}
		}
}
/****************************************************************************
  * 函数功能: ESP模块初始化函数    设置状态机指令
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  ***************************************************************************/
void ESP_Init(void)
{
    printf("@@ Will Enable ESP...\r\n");
    ESP_RESET();
    //ESP_POWER_ON();                     //开机
    ESP_TaskStatus = ESP_SEND;           //进入发送模式
    ATCurrentCmdNum = AT;              //当前指令
    ATNextCmdNum = (teATCmdNum)(ATCurrentCmdNum+1);        //下一条指令
}
/****************************************************************************
  * 函数功能: 硬件复位  ESP8266 多次超时之后 使用
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  ***************************************************************************/
void ESP_RESET(void)
{
    HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(2500);
    HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_SET);
}
/****************************************************************************
  * 函数功能: 获取系统时间  来进行超时的处理
  * 输入参数:  *TimeType 自定义时间结构体，TimeInter 时间间隔
  * 返 回 值: 无
  * 说    明：TimeInter在ATCmds[]中
  ***************************************************************************/
void SetTime(tsTimeType *TimeType,uint32_t TimeInter)
{
    TimeType->TimeStart = HAL_GetTick();    //获取系统运行时间
    TimeType->TimeInter = TimeInter;      //间隔时间
}
/****************************************************************************
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 时间结构体
  * 返 回 值: 返回一代表超时
  * 说    明：无
  ****************************************************************************/
uint8_t  CompareTime(tsTimeType *TimeType)  //比较时间
{
    return ((HAL_GetTick()-TimeType->TimeStart) >= TimeType->TimeInter);  
}

/***********************************************************
  函数名称：Find_string(char *pcBuf,char*left,char*right, char *pcRes)
  函数功能：寻找特定字符串
  入口参数：
           char *pcBuf 为传入的字符串
           char*left   为搜索字符的左边标识符  例如："["
           char*right  为搜索字符的右边标识符  例如："]"
					 char *pcRes 为输出转存的字符串数组
  返回值：用来校验是否成功，无所谓的。
  备注： left字符需要唯一，right字符从left后面开始唯一即可
 服务器下发命令举例：+MQTTPUBLISH: 0,0,0,0,/device/NB/zx99999999999999_back,6,[reastrobot]
***********************************************************/
int Find_string(char *pcBuf,char *left,char *right, char *pcRes)
{
    char *pcBegin = NULL;
    char *pcEnd = NULL;
    pcBegin = strstr(pcBuf, left);//取出左边数据
    pcEnd = strstr(pcBegin+strlen(left), right);//扫描右边标识
    if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
    {
        printf("string name not found!\n");
        return 0;
    }
    else
    {
        pcBegin += strlen(left);
        memcpy(pcRes, pcBegin, pcEnd-pcBegin);
        return 1;
    }
}




