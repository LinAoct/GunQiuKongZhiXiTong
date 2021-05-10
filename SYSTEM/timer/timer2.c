#include "timer2.h"

/*
*===================================================================
*		说明：TIM2初始化配置，以便输出PWM波驱动电机
*					TIM2.3 -> 左电机  TIM2.4 -> 右电机
*		参数：Period: 定时器自动重装值   72000000/799 = 80KHZ
*					Prescaler: 时钟频率除数的预分频值
*		返回：无
*===================================================================
*/
void MOTOR_PWM_Init(u16 Period, u16 Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);												//使能 TIM2 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	//使能 GPIO 口时钟 打开复用再修改复用功能

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;				//PA 2 3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    						//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = Period; 										//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler; 							//设置用来作为TIMx时钟频率除数的预分频值 0代表不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//设置时钟分割 TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 							//根据 TIM_TimeBaseInitStruct 中指定的参数初始化 TIMx 的时间基数单位
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 													//设置待装入捕获比较寄存器的脉冲值 占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 			//输出极性:TIM输出比较极性

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  		//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  		//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1预装载使能	
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH2预装载使能	

	TIM_Cmd(TIM2, ENABLE);  											//使能TIM2
}

/*
*===================================================================
*		说明：TIM2 占空比调节 占空比 = TIMx_CCRx / TIMx_ARR
*		参数：moto_l：左轮电机  数值 0-100
*					moto_r：右轮电机	数值 0-100
*		返回：无
*===================================================================
*/
void MOTO_PWM_Out(u16 moto_l, u16 moto_r)
{
	TIM2->CCR4 = moto_l;
	TIM2->CCR3 = moto_r;

	//TIM_OCInitTypeDef TIM_OCInitSructure;
	
	//TIM_OCInitSructure.TIM_OCMode = TIM_OCMode_PWM1;
	//TIM_OCInitSructure.TIM_OutputState= TIM_OutputState_Enable;

	//CH3 左电机
	//TIM_OCInitSructure.TIM_Pulse = moto_l;//占空比= ccr/100
	//TIM_OC3Init(TIM2, &TIM_OCInitSructure);//参数初始化
	//TIM_OC3PolarityConfig(TIM2, TIM_OCPreload_Enable);//开始输出pwm
	
	//CH4 右电机
	//TIM_OCInitSructure.TIM_Pulse = moto_r;//占空比= ccr /100
	//TIM_OC4Init(TIM2, &TIM_OCInitSructure);//参数初始化
	//TIM_OC4PolarityConfig(TIM2, TIM_OCPreload_Enable);	//开始输出pwm
	
	//TIM_ARRPreloadConfig(TIM2, ENABLE);			//启动自动重装
}
