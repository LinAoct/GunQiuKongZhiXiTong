#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "delay.h"
#include "led.h"

/*PA -- 4 5 6 7*/

#define KEY0 PAin(4)
#define KEY1 PAin(5)
#define KEY2 PAin(6)
#define KEY3 PAin(7)



void KEY_Init(void);  //按键初始化
u8 KEY_Scan(u8 i); 		//按键扫描
//void EXTI0_IRQHandler(void);
//void EXTI1_IRQHandler(void);
//void EXTI2_IRQHandler(void);
//void EXTI3_IRQHandler(void);
#endif
