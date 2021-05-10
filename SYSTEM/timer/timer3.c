#include "timer3.h"


/*
*===================================================================
*		说明：TIM3中断优先级配置
*		参数：无
*		返回：无
*===================================================================
*/
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
*===================================================================
*		说明：TIM3配置 产生1ms为周期的中断
*		参数：无
*		返回：无
*===================================================================
*/
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period = 65535;									//自动重装载寄存器周期的值(计数值)	1000
    TIM_TimeBaseStructure.TIM_Prescaler= 359;									//时钟预分频数 72M/360   	71
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//采样分频设置
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		//清除溢出中断标志 
}

void TIM3_IRQHandler(void)  
{
   if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  
   {
       TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
   }
}

