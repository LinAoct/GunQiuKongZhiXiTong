#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "delay.h"

#define LED0 PBout(3)
#define LED1 PBout(4)
#define LED2 PBout(5)
#define LED3 PBout(6)
#define LED4 PBout(7)
#define LED5 PBout(8)
#define LED6 PBout(9)
#define LED7 PBout(10)

//#define LEDTEST PCout(13)

#define LEDTEST(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_13);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_13)				


void LED_Init(void);  				//LED灯初始化
void LED_Init_PWM_Mode(void);	//LED端口复用 以便显示呼吸灯
void LED_Shine(void);					//LED灯闪烁
void LED_Slide(void);					//LED流水灯
void LED_Turn(void);					//LED状态翻转
void LED_Live(void);					//LED呼吸灯

#endif
