#include "wave.h"


u8 Limiti_Distance = 20;		//最小停车距离 单位 cm
u16 Distance = 0;						//障碍物距离
u8 OB_L =0,OB_M=0,OB_R=0;		//障碍物位置标记

/*
*===================================================================
*		说明：超声波端口初始化 PB9 - TRIG | PB8 - ECH0
*		参数：无
*		返回：无
*===================================================================
*/
void WAVE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	       
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = WAVE_TRIG_PIN;					   	//PB9 - TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     	//设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	                 	//初始化外设 GPIO 
	
	GPIO_InitStructure.GPIO_Pin = WAVE_ECHO_PIN;				    	//PB8 - ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//设为浮空输入模式
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM3_Configuration();					//定时器 TIM3 配置
	TIM3_NVIC_Configuration();		//定时器 TIM3 中断初始化
	
}

/*
*===================================================================
*		说明：超声波测距 发送一个 >10us 的脉冲，测量返回高电平时间间隔
*					从而推算出距离，利用 TIM3 记录时间
*		参数：无
*		返回：distance 距离，单位 cm
*===================================================================
*/
u16 WAVE_Get_Distance(void)
{
	u16 distance;		//定义距离大小变量
  Wave_ON();  		//送>10uS的高电平
  delay_us(20);		//延时20uS
  Wave_OFF();			//设为低电平
  
  while(!Wave_State());	    //等待高电平
  TIM_Cmd(TIM3, ENABLE);    //开启 TIM3 时钟 计时以便知道信号往返时间
  while(Wave_State());	    //等待低电平
  TIM_Cmd(TIM3, DISABLE);		//TIM3 失能
  //distance = (TIM_GetCounter(TIM3) * 34)/2;	//计算距离 厘米cm
	distance = TIM_GetCounter(TIM3)*5*34/2000;	//计算距离 厘米cm
  TIM_SetCounter(TIM3, 0);	//定时器2 TIM3 计数值清零
  return distance;					//返回距离
}


/*
*===================================================================
*		说明：小车避障
*		参数：无
*		返回：无
*===================================================================
*/
void Wave_Find_Way(void)
{
	Distance = WAVE_Get_Distance();		//获取距离
	if(Distance < 800)		//防止突然距离跳变
	{
		if((Distance < Limiti_Distance) && (Distance != 0))
		{
			Car_Stop();						//停车
			OB_M = 1;							//标记中间有障碍物
			Car_Turn_Left();			//左转
			delay_ms(250);										//粗略等待 250ms 转身到右边
			Car_Stop();						//停车
			Distance = WAVE_Get_Distance();		//测量右端是否有障碍物
			//检测左边距离
			if((Distance < Limiti_Distance) && (Distance != 0))
			{
				OB_L = 1;					//标记左侧有障碍物
				Car_Turn_Left();	//左转
				delay_ms(500);		//粗略等待 500ms 180度 转身
				Car_Stop();						//停车
				Distance = WAVE_Get_Distance();		//测量右端是否有障碍物
				//检测右边距离
				if((Distance < Limiti_Distance) && (Distance != 0))
				{
					OB_R = 1;					//标记左侧有障碍物
					Car_Turn_Right();	//左转
					delay_ms(250);		//粗略等待 250ms 90度 右转身
					Car_Go();				//开车
				}
				else	//右侧有空间走
				{
					Car_Go();
					return;
				}
			}
			else	//左侧有空间走
			{
				Car_Go();
				return;
			}
		}
	}
}
