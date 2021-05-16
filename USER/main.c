#include "sys.h"


u16 printf_count = 65534, printf_count_temp = 0;	//串口打印时间间隔
u32 count_flag;					//Debug

u8 USART1_RX_DATA[4];		//存储USART1接收到的数据
u8 USART3_RX_DATA[4];		//存储USART3接收到的数据
u8 u1_action_mode = 0x25;			//串口1动作指令变量
u8 u3_action_mode = 0x25;			//串口3动作指令变量

int	main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//配置中断优先级分组
	delay_init();								//延时函数初始化
	//KEY_Init();								//按键初始化
	uart_init(115200);					//USART1初始化
	uart2_init(115200);					//USART2初始化
	uart3_init(9600);						//USART3初始化
	TIM1_Init(150, 7199);      	//TIM1 15ms 中断	149, 7199
	TIM3_PWM_Init(1800, 799);   //舵机PWM初始化   199,7199  9999, 143	1799, 800
	Set_PWM(0, 0);
	printf("初始化完成\n");

	while(1)
	{
		//SERVO_Test();
		//判断是否收到串口1动作
		if(IS_USART1_RX_Success==1) 
		{
			memcpy(USART1_RX_DATA, USART1_RX_BUF, 4*sizeof(u8));	//获取串口1缓冲区数据
			if(U1_Mode == 0)	//按钮功能模式
			{
				u1_action_mode = USART1_RX_DATA[0];
				printf("收到u1指令：%x\n", u1_action_mode);
				switch(u1_action_mode)
				{
					case 0x0: Mode_Init(); Action_Mode=100; break;	//停止运动
					case 0x1: Mode_Init(); Action_Mode=1; break;		//基础任务1
					case 0x2: Mode_Init(); Action_Mode=2; break;		//基础任务2
					case 0x3: Mode_Init(); Action_Mode=3; break;		//基础任务3
					case 0x4: Mode_Init(); Action_Mode=4; break;		//基础任务4
					case 0x5: Mode_Init(); Action_Mode=11; break;		//发挥部分1
					case 0x6: Mode_Init(); Action_Mode=12; break;		//发挥部分2
					case 0x7: Mode_Init(); Action_Mode=13; break;		//发挥部分3
					case 0x20: Mode_Init();	Action_Mode=20; break;	//发挥部分2设置目标点
					case 0x21: Mode_Init(); Action_Mode=21; break;	//去点1
					case 0x22: Mode_Init(); Action_Mode=22; break;	//去点2
					case 0x23: Mode_Init(); Action_Mode=23; break;	//去点3
					case 0x24: Mode_Init(); Action_Mode=24; break;	//去点4
					case 0x25: Mode_Init(); Action_Mode=25; break;	//去点5
					case 0x26: Mode_Init(); Action_Mode=26; break;	//去点6
					case 0x27: Mode_Init(); Action_Mode=27; break;	//去点7
					case 0x28: Mode_Init(); Action_Mode=28; break;	//去点8
					case 0x29: Mode_Init(); Action_Mode=29; break;	//去点9
					default: break;
				}
				u1_action_mode = 100;
			}
			else if(U1_Mode == 1)	//摇杆模式
			{
				target_point_X_temp = USART1_RX_DATA[0];
				target_point_Y_temp = USART1_RX_DATA[1];
				Mode_Init(); 
				Action_Mode=30;	//去指定点
				printf("摇杆定点模式[x:%d, y:%d]", USART1_RX_DATA[0], USART1_RX_DATA[1]);
			}
			U1_Mode = 0;
			IS_USART1_RX_Success = 0;
		}
		
		
		//判断是否收到串口3动作
		if(IS_USART3_RX_Success==1) 
		{
			memcpy(USART3_RX_DATA, USART3_RX_BUF, 4*sizeof(u8));	//获取串口1缓冲区数据
			if(U3_Mode == 0)	//按钮功能模式
			{
				u3_action_mode = USART3_RX_DATA[0];
				printf("收到u3指令：%x\n", u3_action_mode);
				switch(u3_action_mode)
				{
					case 0x0: Mode_Init(); Action_Mode=100; break;	//停止运动
					case 0x1: Mode_Init(); Action_Mode=1; break;		//基础任务1
					case 0x2: Mode_Init(); Action_Mode=2; break;		//基础任务2
					case 0x3: Mode_Init(); Action_Mode=3; break;		//基础任务3
					case 0x4: Mode_Init(); Action_Mode=4; break;		//基础任务4
					case 0x5: Mode_Init(); Action_Mode=11; break;		//发挥部分1
					case 0x6: Mode_Init(); Action_Mode=12; break;		//发挥部分2
					case 0x7: Mode_Init(); Action_Mode=13; break;		//发挥部分3
					case 0x20: Mode_Init();	Action_Mode=20; break;	//发挥部分2设置目标点
					case 0x21: Mode_Init(); Action_Mode=21; break;	//去点1
					case 0x22: Mode_Init(); Action_Mode=22; break;	//去点2
					case 0x23: Mode_Init(); Action_Mode=23; break;	//去点3
					case 0x24: Mode_Init(); Action_Mode=24; break;	//去点4
					case 0x25: Mode_Init(); Action_Mode=25; break;	//去点5
					case 0x26: Mode_Init(); Action_Mode=26; break;	//去点6
					case 0x27: Mode_Init(); Action_Mode=27; break;	//去点7
					case 0x28: Mode_Init(); Action_Mode=28; break;	//去点8
					case 0x29: Mode_Init(); Action_Mode=29; break;	//去点9
					default: break;
				}
				u3_action_mode = 100;
			}
			else if(U3_Mode == 1)	//摇杆模式
			{
				target_point_X_temp = USART3_RX_DATA[0];
				target_point_Y_temp = USART3_RX_DATA[1];
				Mode_Init();
				Action_Mode=30;	//去指定点
				printf("u3摇杆定点模式[x:%d, y:%d]", USART3_RX_DATA[0], USART3_RX_DATA[1]);
			}
			U3_Mode = 0;
			IS_USART3_RX_Success = 0;
		}
	}
}
