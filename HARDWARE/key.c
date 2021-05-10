#include "key.h"

/*
*===================================================================
*		说明：初始化按钮，设为上拉输入 PA -- 4 5 6 7
*		参数：无
*		返回：无
*===================================================================
*/
void KEY_Init(void)
{
//	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
//	RCC->APB2ENR|=1<<3;     //使能PORTB时钟

//	GPIOA->CRH&=0X0FF00FF0;
//	GPIOA->CRH|=0X80088008;
//	GPIOA->ODR|=153<<8;

//	AFIO -> EXTICR[1] = 0x1111;				//PB4 设置为外部中断
//	EXTI -> FTSR |= 240;								//设置 INT0~INT4 触发信号为下降沿
//	EXTI -> IMR |= 240;								//允许 INT0~INT4 中断信号
//	NVIC -> ISER[0] |= (240<<6);				//使能 INT0~INT4

	//NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE); 	//开启按键端口PB的时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				//端口配置为上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化端口
	
//	/* 中断Line4配置 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发模式
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);														//根据指定的参数初始化 EXTI 寄存器
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化 NVIC 寄存器

//	/* 中断Line5配置 */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发模式
//	EXTI_Init(&EXTI_InitStructure);														//根据指定的参数初始化 EXTI 寄存器
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//子优先级0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化 NVIC 寄存器
//	
//	/* 中断Line6配置 */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发模式
//	EXTI_Init(&EXTI_InitStructure);														//根据指定的参数初始化 EXTI 寄存器
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化 NVIC 寄存器

//	/* 中断Line7配置 */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发模式
//	EXTI_Init(&EXTI_InitStructure);														//根据指定的参数初始化 EXTI 寄存器
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//子优先级0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化 NVIC 寄存器
}

/*
*===================================================================
*		说明：按键0中断处理函数
*		参数：无
*		返回：无
*===================================================================
*/
//void EXTI4_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
//	{
//		LED_Init_PWM_Mode();	//设置为复用模式
//		LED_Live();		//显示呼吸灯
//		LED_MODE = 1;	//设置LED灯显示模式 以便循环
//		EXTI_ClearITPendingBit(EXTI_Line4);		//清除中断标志位
//	}
//}

///*
//*===================================================================
//*		说明：按键 1-3 中断处理函数
//*		参数：无
//*		返回：无
//*===================================================================
//*/
//void EXTI9_5_IRQHandler(void)
//{
//	u32 Temp = 0x00; 
//  Temp = EXTI -> PR;	//获取中断源
//  switch(Temp)
//  {
//		case 0x20:		//按键1中断 Line5
//			LED_Init();
//			LED_Slide();
//			LED_Slide();
//			LED_MODE = 2;
//			printf("中断Line5");
//      EXTI_ClearITPendingBit(EXTI_Line5);		//清除中断标志位
//      break;
//    case 0x40:		//按键2中断 Line6
//			LED_Init();
//			LED_Shine();
//			LED_Shine();
//			LED_Shine();
//			LED_MODE = 3;
//			printf("中断Line6");
//      EXTI_ClearITPendingBit(EXTI_Line6);		//清除中断标志位
//      break;
//    case 0x80:		//按键3中断 Line7
//			LED_Init();
//			LED_Slide();
//			LED_Slide();
//			LED_MODE = 4;
//			printf("中断Line7");
//      EXTI_ClearITPendingBit(EXTI_Line7);		//清除中断标志位
//      break;
//    default: EXTI_ClearITPendingBit(Temp); break;
//	}
//}

/*
*===================================================================
*		说明：按键扫描函数，若有按键按下，返回对应按键的编号
*		参数：i：暂无
*		返回：按键编号
*===================================================================
*/
u8 KEY_Scan(u8 i)
{	 
	static u8 key_up=1;	//按键按松开标志
	if(i)key_up=1;  		//支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(120);			//延时 120ms 去抖动 
		key_up=0;
		if(KEY0==0)return 0;
		else if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)	key_up=1; 
 	return 100;					//无按键按下
}
