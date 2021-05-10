#include "led.h"

u16 LED_GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
									 GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;	//LED灯Pin


/*
*===================================================================
*		说明：LED灯端口初始化 PB -- 3 4 5 6 7 8 9 10
*		参数：无
*		返回：无
*===================================================================
*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;									//GPIO初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	//使能GPIO口时钟，打开复用再修改复用功能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);											//因为 PB3 PB4 的原因需要重映射  关闭JTAG  使能SWD
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
																GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;	//GPIO 端口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //速度50MHZ
  GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化
	GPIO_WriteBit(GPIOB,LED_GPIO_Pin,(BitAction)1);				//灯全亮
}

/*
*===================================================================
*		说明：LED灯端口设置为复用输出模式，以输出PWM PB -- 3 4 5 6 7 8 9 10
*		参数：无
*		返回：无
*===================================================================
*/
void LED_Init_PWM_Mode(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 |RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	//使能 GPIO 口时钟，打开复用AFIO再修改复用功能

	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2 | GPIO_PartialRemap2_TIM2, ENABLE);	//重定义 GPIO 口定时器功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;	//PB 3 10	TIM2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//PB 4 5	TIM3
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 	//PB 6 7 8 9 TIM4
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
*===================================================================
*		说明：LED全闪烁一次
*		参数：无
*		返回：无
*===================================================================
*/
void LED_Shine(void)
{
	GPIO_WriteBit(GPIOB,LED_GPIO_Pin,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, LED_GPIO_Pin))));
	delay_ms(200);
	GPIO_WriteBit(GPIOB,LED_GPIO_Pin,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, LED_GPIO_Pin))));
	delay_ms(200);
}

/*
*===================================================================
*		说明：LED流水灯
*		参数：无
*		返回：无
*===================================================================
*/
void LED_Slide(void)
{
		GPIOB -> BSRR = 0x0000FFFF;			//LED灯全灭
		GPIO_WriteBit(GPIOB,GPIO_Pin_3,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_3,(BitAction)1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_4,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_4,(BitAction)1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_7,(BitAction)1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)0); delay_ms(100); GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)1);
}

/*
*===================================================================
*		说明：LED状态翻转
*		参数：无
*		返回：无
*===================================================================
*/
void LED_Turn(void)
{
	GPIO_WriteBit(GPIOB,LED_GPIO_Pin,(BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, LED_GPIO_Pin))));
}

/*
*===================================================================
*		说明：LED呼吸灯效果
*		参数：无
*		返回：无
*===================================================================
*/
void LED_Live(void)
{
	u8 flag=1, up_flag=1;
	u16 led_val=0;
	while(flag)
	{
//	  TIM4->CCR1=i;
//		TIM4->CCR2=i;
//		TIM4->CCR3=i;
//		TIM4->CCR4=i;
		if(led_val < 499 && up_flag) led_val += 2;
		else
		{
			led_val--;
			up_flag=0;
			if(led_val==0) flag=0;
		}

		TIM_SetCompare2(TIM2,led_val);	//灯0设值
		TIM_SetCompare1(TIM3,led_val);	//灯1设值
		TIM_SetCompare2(TIM3,led_val);	//灯2设值
		TIM_SetCompare1(TIM4,led_val);	//灯3设值
		TIM_SetCompare2(TIM4,led_val);	//灯4设值
		TIM_SetCompare3(TIM4,led_val);	//灯5设值
		TIM_SetCompare4(TIM4,led_val);	//灯6设值
		TIM_SetCompare3(TIM2,led_val);	//灯7设值
		delay_ms(5);
	}

}
