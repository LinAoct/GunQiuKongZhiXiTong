#include "control.h"	

//数字点位置坐标
u8 Point[10][2]={{0,0},{40,35},{115,35},{190,35},
												{45,110},{117,110},{190,110},
												{45,185},{120,185},{193,185}};
//画圆坐标
u8 Squre_Point[9][2]={{0,0},{120,50},{150,80},{180,110},
														{150,140},{120,170},{90,140},
														{60,110},{90,80}};

u8 Mode_Plus_2_Target_Side[4];	//存储发挥部分2的目标数字

u8 max_target_PID = 60;					//舵机最大CCR变化值  50
u8 speed_max = 25;							//速度环最大速度限制 30

u8 Position_X, Position_Y;

int Last_X_target_PID, Last_Y_target_PID;			//上次位置 PID 输出值变量
int X_target_PID, Y_target_PID;								//当前 PID 输出值变量

u8 target_point_X, target_point_Y;						//小球坐标目标值
u8 target_point_X_temp, target_point_Y_temp;	//小球目标值缓存

int square_r = 50, square_current_x, square_current_y;
														
u16 count, count_spacing = 50, init_count, step;

u8 USART2_RX_DATA[2];	//位置信息接收变量
u8 Action_Mode;				//动作变量
												
void TIM1_UP_IRQHandler(void)
{
	static u8 last_action_mode;		//保存上次一次动作变量
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)	//检查TIM1是否发生中断
	{
		//if(Action_Mode != last_action_mode)	Mode_Init();
		switch(Action_Mode)
		{
			case 100:	Mode_Stop(); break; 
			case 1: Mode_Base_1(); break;
			case 2: Mode_Base_2(); break;
			case 3: Mode_Base_3(); break;
			case 4: Mode_Base_4(); break;
			case 11: Mode_Plus_1(); break;
			case 12: Mode_Plus_1(); break;
			case 13: Mode_Draw_Square(); break;
			case 21: Mode_Go_Num(1); break;
			case 22: Mode_Go_Num(2); break;
			case 23: Mode_Go_Num(3); break;
			case 24: Mode_Go_Num(4); break;
			case 25: Mode_Go_Num(5); break;
			case 26: Mode_Go_Num(6); break;
			case 27: Mode_Go_Num(7); break;
			case 28: Mode_Go_Num(8); break;
			case 29: Mode_Go_Num(9); break;
			case 30: Mode_Go_Target_Point(target_point_X_temp, target_point_Y_temp); break;
			default: break;
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);			//清除TIM1的中断待处理位
   }       	
}

/*
*===================================================================
*		说明：停止运动 板恢复平衡
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Stop(void)
{
	Set_PWM(0, 0);
	Action_Mode = 0;
}

/*
*===================================================================
*		说明：基础任务1 
*					将小球放置在区域2 控制使小球在区域内停留不少于5秒
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Base_1(void)
{
	Mode_Go_Num(2);
}

/*
*===================================================================
*		说明：基础任务2
*					在15秒内 控制小球从区域1进入区域5 在区域5停留不少于2秒
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Base_2(void)
{
	printf("%d, %d", count, step);
	if(step == 0) count = 101, step++;
	if(step == 1 && count>0) Mode_Go_Num(1), count--;	//延时 1500ms
	if(step == 1 && count==0) Mode_Go_Num(5);
}

/*
*===================================================================
*		说明：基础任务3
*					控制小球从区域1进入区域4 在区域4停留不少于2秒
*					然后再进入区域5 小球在区域5停留不少于2秒
*					完成以上两个动作总时间不超过20秒
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Base_3(void)
{
	if(step == 0) count = 21, step++;									//准备延时 300ms
	if(step == 1 && count>0) Mode_Go_Num(1), count--;	//定点1
	if(step == 1 && count==0) count = 181, step++;			//准备延时 2700ms
	if(step == 2 && count>0) Mode_Go_Num(4), count--;	//定点4
	if(step == 2 && count==0) count = 181, step++;			//准备延时 2700ms
	if(step == 3 && count>0) Mode_Go_Num(5);						//定点5
}

/*
*===================================================================
*		说明：基础任务4
*					在30秒内 控制小球从区域1进入区域9 且在区域9停留不少于2秒
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Base_4(void)
{
	if(step == 0) count = 31, step++;									//准备延时 450ms
	if(step == 1 && count>0) Mode_Go_Num(1), count--;	//定点1
	if(step == 1 && count==0) step++;			//等待延时完毕
	if(step == 2) Mode_Go_Num(9);					//定点9
}

/*
*===================================================================
*		说明：发挥任务1
*					在40 秒内控制小球从区域1 出发 先后进入区域2、区域6
*					停止于区域9 在区域9 中停留时间不少于2 秒
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Plus_1(void)
{
	if(step == 0) count = 31, step++;										//准备延时 450ms
	if(step == 1 && count > 0) Mode_Go_Num(2), count--;	//定点2
	if(step == 1 && count == 0) count = 181, step++;		//准备延时 2700ms
	if(step == 2 && count > 0) Mode_Go_Num(6), count--;	//定点6
	if(step == 2 && count == 0) step++;									//准备延时 2700ms
	if(step == 3) Mode_Go_Num(9);												//定点9
}


/*
*===================================================================
*		说明：运动到指定数字序号位置
*		参数：num	<u8>	目标位置序号
*		返回：无
*===================================================================
*/
void Mode_Go_Num(u8 num)
{
	target_point_X = Point[num][0];
	target_point_Y = Point[num][1];
	
	if(IS_USART2_RX_HEAD == 0) memcpy(USART2_RX_DATA, USART2_RX_BUF, 2*sizeof(u8));				 //判断当前是否正在接收数据
	Position_X = USART2_RX_DATA[0];
	Position_Y = USART2_RX_DATA[1];

	if(Position_Y!=253||Position_X!=253)			//检测到小球
	{
		X_target_PID = PID_Side_X(Position_X);
		Y_target_PID = PID_Side_Y(Position_Y);

		X_target_PID -= PID_Speed_X(Position_X);
		Y_target_PID -= PID_Speed_Y(Position_Y);
		PID_Limit(&X_target_PID, &Y_target_PID);	//输出量限幅 防止卡死
		Set_PWM(X_target_PID, Y_target_PID);			//舵机输出
	}
}

/*
*===================================================================
*		说明：从点1运动到点9
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Go_1_To_9(void)
{
	//走数字键盘1~9
	if(count<count_spacing) target_point_X = Point[5][0], target_point_Y = Point[5][1];
	else if(count>=count_spacing && count<2*count_spacing) target_point_X = Point[1][0], target_point_Y = Point[1][1];
	else if(count>=2*count_spacing && count<3*count_spacing) target_point_X = Point[2][0], target_point_Y = Point[2][1];
	else if(count>=3*count_spacing && count<4*count_spacing) target_point_X = Point[3][0], target_point_Y = Point[3][1];
	else if(count>=4*count_spacing && count<5*count_spacing) target_point_X = Point[6][0], target_point_Y = Point[6][1];
	else if(count>=5*count_spacing && count<6*count_spacing) target_point_X = Point[5][0], target_point_Y = Point[5][1];
	else if(count>=6*count_spacing && count<7*count_spacing) target_point_X = Point[4][0], target_point_Y = Point[4][1];
	else if(count>=7*count_spacing && count<8*count_spacing) target_point_X = Point[7][0], target_point_Y = Point[7][1];
	else if(count>=8*count_spacing && count<9*count_spacing) target_point_X = Point[8][0], target_point_Y = Point[8][1];
	else if(count>=9*count_spacing && count<10*count_spacing) target_point_X = Point[9][0], target_point_Y = Point[9][1];
	else if(count>=10*count_spacing && count<11*count_spacing) target_point_X = Point[5][0], target_point_Y = Point[5][1];
	else count=0;
	count++;

	if(IS_USART2_RX_HEAD == 0) memcpy(USART2_RX_DATA, USART2_RX_BUF, 2*sizeof(u8));				 //判断当前是否正在接收数据
	Position_X = USART2_RX_DATA[0];
	Position_Y = USART2_RX_DATA[1];

	if(Position_Y!=253||Position_X!=253)			//检测到小球
	{
		X_target_PID = PID_Side_X(Position_X);
		Y_target_PID = PID_Side_Y(Position_Y);

		X_target_PID -= PID_Speed_X(Position_X);
		Y_target_PID -= PID_Speed_Y(Position_Y);
		PID_Limit(&X_target_PID, &Y_target_PID);	//输出量限幅 防止卡死
		Set_PWM(X_target_PID, Y_target_PID);			//舵机输出
	}
	//printf("坐标:%d, %d\t PID:%d, %d\t count:%d\n", Position_X, Position_Y, X_target_PID, Y_target_PID, count);
}

/*
*===================================================================
*		说明：去目标点
*		参数：x	<u8>	目标点x
					y	<u8>	目标点y
*		返回：无
*===================================================================
*/
void Mode_Go_Target_Point(u8 x, u8 y)
{
	target_point_X = x;
	target_point_Y = y;

	if(IS_USART2_RX_HEAD == 0) memcpy(USART2_RX_DATA, USART2_RX_BUF, 2*sizeof(u8));	//判断当前是否正在接收数据
	Position_X = USART2_RX_DATA[0];
	Position_Y = USART2_RX_DATA[1];

	if(Position_Y!=253||Position_X!=253)			//检测到小球
	{
		X_target_PID = PID_Side_X(Position_X);
		Y_target_PID = PID_Side_Y(Position_Y);

		X_target_PID -= PID_Speed_X(Position_X);
		Y_target_PID -= PID_Speed_Y(Position_Y);
		PID_Limit(&X_target_PID, &Y_target_PID);	//输出量限幅 防止卡死
		Set_PWM(X_target_PID, Y_target_PID);			//舵机输出
	}
}

/*
*===================================================================
*		说明：画圆
*		参数：无
*		返回：无
*===================================================================
*/
void Mode_Draw_Square(void)
{
	if(step == 0) square_current_x = Point[5][0]-square_r, step++;
	if(step==1)
	{
		if(count == 2)
		{
			count=0;
			square_current_x++;
			square_current_y = sqrt((square_r*square_r)-((square_current_x-Point[5][0])*(square_current_x-Point[5][0]))) + Point[5][0];
			if(square_current_x == Point[5][0]+square_r) step++;
		}
	}
	if(step==2)
	{
		if(count == 2)
		{
			count=0;
			square_current_x--;
			square_current_y = -sqrt((square_r*square_r)-((square_current_x-Point[5][0])*(square_current_x-Point[5][0]))) + Point[5][0];
			if(square_current_x == Point[5][0]-square_r) step--;
		}
	}
	count++;
	target_point_X = square_current_x, target_point_Y = square_current_y;
	
//	if(count<count_spacing) target_point_X = Squre_Point[1][0], target_point_Y = Squre_Point[1][1];
//	else if(count>=count_spacing && count<2*count_spacing) target_point_X = Squre_Point[2][0], target_point_Y = Squre_Point[2][1];
//	else if(count>=2*count_spacing && count<3*count_spacing) target_point_X = Squre_Point[3][0], target_point_Y = Squre_Point[3][1];
//	else if(count>=3*count_spacing && count<4*count_spacing) target_point_X = Squre_Point[4][0], target_point_Y = Squre_Point[4][1];
//	else if(count>=4*count_spacing && count<5*count_spacing) target_point_X = Squre_Point[5][0], target_point_Y = Squre_Point[5][1];
//	else if(count>=5*count_spacing && count<6*count_spacing) target_point_X = Squre_Point[6][0], target_point_Y = Squre_Point[6][1];
//	else if(count>=6*count_spacing && count<7*count_spacing) target_point_X = Squre_Point[7][0], target_point_Y = Squre_Point[7][1];
//	else if(count>=7*count_spacing && count<8*count_spacing) target_point_X = Squre_Point[8][0], target_point_Y = Squre_Point[8][1];
//	else count=0;
//	count++;

	if(IS_USART2_RX_HEAD == 0) memcpy(USART2_RX_DATA, USART2_RX_BUF, 2*sizeof(u8));				 //判断当前是否正在接收数据
	Position_X = USART2_RX_DATA[0];
	Position_Y = USART2_RX_DATA[1];

	if(Position_Y!=253||Position_X!=253)				//检测到小球
	{
		X_target_PID = PID_Side_X(Position_X);
		Y_target_PID = PID_Side_Y(Position_Y);

		X_target_PID -= PID_Speed_X(Position_X);
		Y_target_PID -= PID_Speed_Y(Position_Y);
		PID_Limit(&X_target_PID, &Y_target_PID);	//输出量限幅 防止卡死
		Set_PWM(X_target_PID, Y_target_PID);			//舵机输出
	}
	//printf("坐标:%d, %d\t PID:%d, %d\t count:%d\n", Position_X, Position_Y, X_target_PID, Y_target_PID, count);
}

/*
*===================================================================
*		说明：PID输出量限幅
*		参数：*pid	<int>	输出量指针地址
*		返回：无
*===================================================================
*/
void PID_Limit(int *x_pid, int *y_pid)
{
	if(*x_pid < -max_target_PID) *x_pid = -max_target_PID;	
	if(*x_pid > max_target_PID)  *x_pid = max_target_PID;	 
	if(*y_pid < -max_target_PID) *y_pid = -max_target_PID;	
	if(*y_pid > max_target_PID)  *y_pid = max_target_PID;	
}

void Mode_Init(void)
{
	count = 0;
	step = 0;
}
