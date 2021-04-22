#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "gpio.h"
#include "stdlib.h"	
#include "stm32f1xx_hal.h"
//-----------------OLED�˿ڶ���---------------- 


#define OLED_RES_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_RESET)//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_SET)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define uint8_t unsigned char
#define uint32_t unsigned int
#define WHICH_SPI &hspi2//����ʹ�õ�SPI����

void OLED_ClearPoint(uint8_t x,uint8_t y);    //���ĳһ����
void OLED_ColorTurn(uint8_t i);                //
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[]);
void OLED_Printf(uint8_t str[]);
void OLED_Init(void);
void Sys_begin(void);
//void Sys_menu(uint8_t flag,uint8_t temp);
void OLED_ShowChar1(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1);
void OLED_Print(uint8_t x, uint8_t y, char *s);
#endif
