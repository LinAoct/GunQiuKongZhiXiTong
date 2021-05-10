#ifndef __WAVE_H
#define __WAVE_H
#include "sys.h"
#include "timer3.h"

#define WAVE_TRIG_PIN GPIO_Pin_9		//宏定义 超声波 TRIG引脚
#define WAVE_ECHO_PIN GPIO_Pin_8    //宏定义 超声波 ECHO引脚

#define Wave_ON()  GPIO_SetBits(GPIOB, WAVE_TRIG_PIN)								//超声波 TRIG 引脚输出高电平
#define Wave_OFF() GPIO_ResetBits(GPIOB, WAVE_TRIG_PIN)							//超声波 TRIG 引脚输出低电平
#define Wave_State()  GPIO_ReadInputDataBit(GPIOB, WAVE_ECHO_PIN)		//读取超声波 ECHO 引脚电平

void WAVE_Init(void);					//超声波端口初始化
u16 WAVE_Get_Distance(void);	//超声波测量距离	
void Wave_Find_Way(void);

#endif

