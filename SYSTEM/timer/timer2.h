#ifndef __TIMER2_H
#define __TIMER2_H
#include "stm32f10x.h"


void MOTOR_PWM_Init(u16 Period, u16 Prescaler);
void MOTO_PWM_Out(u16 moto_l, u16 moto_r);

#endif
