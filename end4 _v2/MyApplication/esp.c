 /*********************************************************
*   ���ߣ�                                             
*   GND ---> GND                                       
*   VCC ---> 3.3V                                      
*   TXD  ---> USART3��RX->PB11                                
*   RXD ---> USART3��TX->PB10                                
*   REST ---> ESP_RST_Pin->PA3                                  
*   IO��0--->3.3v                                  
*                                     
*                                                      
*/                                                   
/********************************************************/
#include "esp.h"
static uint8_t CurrentRty;    //��ǰ����
static tsTimeType TimeESP;
static teATCmdNum ATRecCmdNum;   //
static int SmartWificount=0;
int  WIFI_Flag=0;
teESP_TaskStatus ESP_TaskStatus;
teATCmdNum ATNextCmdNum;      //��һ��ָ��                                              //����ָ��
teATCmdNum ATCurrentCmdNum;   //��ǰָ��
tsATCmds ATCmds[] =                                                        //ATָ���б�
{
	{"AT\r\n","OK",100,NO_REC,100},                                        //���Բ���
	{"ATE1\r\n","OK",100,NO_REC,100},                                       //�رջ��Բ���
    {"AT+CWMODE=1\r\n","OK",100,NO_REC,100},                               //����ΪSTAģʽ
	{"AT+RST\r\n","WIFI GOT IP",8000,NO_REC,100},  
	{"AT+MQTTUSERCFG=0,1,\"ESP8266\",\"admin\",\"wenyue123\",0,0,\"\"\r\n","OK",1000,NO_REC,100},     //����mqtt����	
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
  * ��������: ͨ�����ڷ���ATָ��
  * �������: ATCmdNum���͵ĵڼ���ָ��
  * �� �� ֵ: ��
  * ˵    ��������������������  ����״̬��������  ָ���ݼ�

  ***************************************************************************/
void ATSend(teATCmdNum ATCmdNum)
{
    ATCmds[ATCmdNum].ATStatus = NO_REC;   //����Ϊ δ����
    ATRecCmdNum = ATCmdNum;
	uint8_t len=strlen(ATCmds[ATCmdNum].ATSendStr);
	HAL_UART_Transmit(&huart3,(uint8_t*)ATCmds[ATCmdNum].ATSendStr,len,0xff);
    printf("@@Send:%s\r\n",ATCmds[ATCmdNum].ATSendStr);
    SetTime(&TimeESP,ATCmds[ATCmdNum].TimeOut);
}
/****************************************************************************
  * ��������:WIFI״̬��  ʵ��WIFI���Զ����� �Լ�wifi����״̬���Զ��л��������ȶ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  ***************************************************************************/
void ESP_Task(void)
{
	while(1)
    {
        switch(ESP_TaskStatus)
        {
			/***����״̬ ����ATSEND��ǰָ��   ����״̬��Ϊ�ȴ�״̬***/
        case ESP_SEND:                                 //����״̬
			WIFI_Flag=1;
            ATSend(ATCurrentCmdNum);                  //��������
            ESP_TaskStatus = ESP_WAIT;
            return;
			/***�ȴ�״̬***/
        case ESP_WAIT:                                 //�ȴ�����״̬
             if(Usart3type.UsartRecFlag)  //�����յ�����
				  {
					  printf("@@Rev:%s\r\n",Usart3type.Usart3RecBuffer);
					  if(strstr((const char*)Usart3type.Usart3RecBuffer,ATCmds[ATRecCmdNum].ATRecStr) != NULL) //���������ӦУ���
						  {
							  ATCmds[ATRecCmdNum].ATStatus = SUCCESS_REC;  //����Ϊ�ɹ�����
						  }
			     
				/***�������Է���  ����ԭ��û�����  ����ִ����ж�  �������֮������ϵͳ�ȶ���***/
					  memset(Usart3type.Usart3RecBuffer,0x00,sizeof(Usart3type.Usart3RecBuffer));
					  Usart3type.UsartRecFlag = 0;  //��ս��ձ�־λ
					  Usart3type.UsartRecLen = 0;   //��ս��ճ���
				}                                 //��������
			  /***���ݷ��ͳɹ�֮��Ĵ���***/
             if(ATCmds[ATCurrentCmdNum].ATStatus == SUCCESS_REC) //�ɹ����ݰ�
				 {
					switch(ATCurrentCmdNum)
						{
							/***SUBָ�����Ҷ�������һ�����͵�ָ��  ��ָ��ɹ��������ɹ�***/
							case AT_SUB:
								ESP_TaskStatus = ESP_ACCESS;
								break;
							/***�ֻ�����֮�� ���������Զ����� Ȼ������***/
							case AT_CWAUTOCONN:
								ATCurrentCmdNum = AT_RST;
								ESP_TaskStatus = ESP_SEND;
								break;
							/***Ĭ��״̬����״̬������***/
							default:
								ATCurrentCmdNum = (teATCmdNum)(ATCurrentCmdNum+1);
								ATNextCmdNum = (teATCmdNum)(ATCurrentCmdNum+1);
								ESP_TaskStatus = ESP_SEND;
								break;
						}
				
				 }else if(CompareTime(&TimeESP))           //��û�յ�Ԥ������  ���ҳ�ʱ
					{	
						/***������ж��Ƕ�������ָ���ֻ��������ж� �������²����ڳ�ʱ ֱ������***/
						if(ATCurrentCmdNum!=AT_CWSTARTSMART)  //����������Ͳ����г�ʱ�ĸ���ֻ��һֱ�ȴ�
							{						
								printf("@@REV--TimeOut:%s\r\n",Usart3type.Usart3RecBuffer);
								ATCmds[ATCurrentCmdNum].ATStatus = TIME_OUT;
								if(CurrentRty > 0)  //�ط�
									{
										CurrentRty--;
										//printf("@@ now:%d,trytime:%d\r\n",ATCurrentCmdNum,CurrentRty);
										ATNextCmdNum = ATCurrentCmdNum;  //��һ�� ���ǵ�ǰ����  ʵ���ط�Ч��
										ESP_TaskStatus = ESP_SEND;
									}else if(ATCurrentCmdNum==AT_RST)//�ط���������
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
									printf("�ֶ�����");
								//	ATNextCmdNum = ATCurrentCmdNum;  //��һ�� ���ǵ�ǰ����  ʵ���ط�Ч��
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
  * ��������: ESPģ���ʼ������    ����״̬��ָ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  ***************************************************************************/
void ESP_Init(void)
{
    printf("@@ Will Enable ESP...\r\n");
    ESP_RESET();
    //ESP_POWER_ON();                     //����
    ESP_TaskStatus = ESP_SEND;           //���뷢��ģʽ
    ATCurrentCmdNum = AT;              //��ǰָ��
    ATNextCmdNum = (teATCmdNum)(ATCurrentCmdNum+1);        //��һ��ָ��
}
/****************************************************************************
  * ��������: Ӳ����λ  ESP8266 ��γ�ʱ֮�� ʹ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  ***************************************************************************/
void ESP_RESET(void)
{
    HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(2500);
    HAL_GPIO_WritePin(ESP_RST_GPIO_Port, ESP_RST_Pin, GPIO_PIN_SET);
}
/****************************************************************************
  * ��������: ��ȡϵͳʱ��  �����г�ʱ�Ĵ���
  * �������:  *TimeType �Զ���ʱ��ṹ�壬TimeInter ʱ����
  * �� �� ֵ: ��
  * ˵    ����TimeInter��ATCmds[]��
  ***************************************************************************/
void SetTime(tsTimeType *TimeType,uint32_t TimeInter)
{
    TimeType->TimeStart = HAL_GetTick();    //��ȡϵͳ����ʱ��
    TimeType->TimeInter = TimeInter;      //���ʱ��
}
/****************************************************************************
  * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
  * �������: ʱ��ṹ��
  * �� �� ֵ: ����һ����ʱ
  * ˵    ������
  ****************************************************************************/
uint8_t  CompareTime(tsTimeType *TimeType)  //�Ƚ�ʱ��
{
    return ((HAL_GetTick()-TimeType->TimeStart) >= TimeType->TimeInter);  
}

/***********************************************************
  �������ƣ�Find_string(char *pcBuf,char*left,char*right, char *pcRes)
  �������ܣ�Ѱ���ض��ַ���
  ��ڲ�����
           char *pcBuf Ϊ������ַ���
           char*left   Ϊ�����ַ�����߱�ʶ��  ���磺"["
           char*right  Ϊ�����ַ����ұ߱�ʶ��  ���磺"]"
					 char *pcRes Ϊ���ת����ַ�������
  ����ֵ������У���Ƿ�ɹ�������ν�ġ�
  ��ע�� left�ַ���ҪΨһ��right�ַ���left���濪ʼΨһ����
 �������·����������+MQTTPUBLISH: 0,0,0,0,/device/NB/zx99999999999999_back,6,[reastrobot]
***********************************************************/
int Find_string(char *pcBuf,char *left,char *right, char *pcRes)
{
    char *pcBegin = NULL;
    char *pcEnd = NULL;
    pcBegin = strstr(pcBuf, left);//ȡ���������
    pcEnd = strstr(pcBegin+strlen(left), right);//ɨ���ұ߱�ʶ
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




