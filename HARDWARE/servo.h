#ifndef __SERVO_H
#define __SERVO_H
#include <sys.h>	 

void TIM3_PWM_Init(u16 arr,u16 psc);
void SERVO_Test(void);
void Set_PWM(int pwm_x, int pwm_y);

#endif
