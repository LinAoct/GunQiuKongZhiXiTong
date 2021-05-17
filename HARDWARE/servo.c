#include "servo.h"

/**************************************************************************
函数功能：舵机PWM初始化 TIM3 | CH1 PA.6 | CH2 PA.7
入口参数：无
返回  值：无
**************************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设时钟使能

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;					//PA.6 PA.7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    							//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 												//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 											//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 										//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 								//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 750; 														//设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//输出极性:TIM输出比较极性高

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH2预装载使能	 

	//TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM4
} 

/*
*===================================================================
*		说明：舵机测试函数
*		参数：无
*		返回：无
*===================================================================
*/
void SERVO_Test(void)
{
	u16 temp = 0;
	while(++temp<180)
	{
		TIM3->CCR1 = 45 + temp;
		TIM3->CCR2 = 45 + temp;
		printf("%d", 45 + temp);
		delay_ms(1000);
	}
	while(--temp>0)
	{
		TIM3->CCR1 = 45 + temp;
		TIM3->CCR2 = 45 + temp;
		printf("%d", 45 + temp);
		delay_ms(1000);
	}
}

void Set_PWM(int pwm_x, int pwm_y)
{
	TIM3->CCR1 = 135 + pwm_x;
	TIM3->CCR2 = 152 + pwm_y;
}
