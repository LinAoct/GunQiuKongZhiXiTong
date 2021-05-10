#include "motor.h"

u8 MOTOR_LEFT_PWM = 75, MOTOR_RIGHT_PWM = 60;		//左右电机 PWM


//***************************配置电机驱动IO口***************************//
void MOTO_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;																					//定义一个 GPIO_InitTypeDef 类型的结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);                                	  /*开启GPIO的外设时钟*/																   
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStructure.GPIO_Pin = MOTO_Pin1 | MOTO_Pin2 | MOTO_Pin3 | MOTO_Pin4;	/*选择要控制的GPIO引脚*/	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                              /*设置引脚模式为通用推挽输出*/   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                             /*设置引脚速率为50MHz */ 
	GPIO_Init(MOTO_PORT, &GPIO_InitStructure);                                    /*调用库函数，初始化GPIO*/
	
	GPIO_InitStructure.GPIO_Pin = MOTO_Pin_STBY;																	/*选择要控制的GPIO引脚*/	
	GPIO_Init(MOTO_PORT, &GPIO_InitStructure);                                    /*调用库函数，初始化GPIO*/
	GPIO_SetBits(GPIOB, MOTO_Pin_STBY);
}

//***************************左右电机向前转动***************************//
//只要配置INx()的状态就可以改变电机转动方向
//左电机控制
void Set_MOTOR_Left(char state)
{
	if(state == GO)		//左电机前进
	{
		IN1(0);
		IN2(1);
	}
	if(state == BACK)	//左电机后退
	{
		IN1(1);
		IN2(0);
	}
	if(state == STOP)	//左电机停转
	{
	  IN1(1);
		IN2(1);
	}
}


//右电机控制
void Set_MOTOR_Right(char state)
{
	if(state == GO)		//右电机前进
	{
		IN3(1);
		IN4(0);
	}
	if(state == BACK)	//右电机后退
	{
		IN3(0);
		IN4(1);
	}
	if(state == STOP)//右电机停转
	{
	  IN3(1);
		IN4(1);
	}
}


void Car_Go(void)
{
	Set_MOTOR_Left(GO);
	Set_MOTOR_Right(GO);
}

//***************************左右电机向后转动***************************//
void Car_Back(void)
{
	Set_MOTOR_Left(BACK);
	Set_MOTOR_Right(BACK);
}

//***************************小车原地右转***************************//
void Car_Turn_Right(void)
{
	Set_MOTOR_Left(GO);
	Set_MOTOR_Right(BACK);
}

//***************************小车原地左转***************************//
void Car_Turn_Left(void)
{
	Set_MOTOR_Left(BACK);
	Set_MOTOR_Right(GO);
}

//***************************停车***************************//
void Car_Stop(void)
{
	Set_MOTOR_Left(STOP);
	Set_MOTOR_Right(STOP);
}



void Speed_Up(void)
{
	if(MOTOR_LEFT_PWM != 0 && MOTOR_LEFT_PWM <100)
		MOTOR_LEFT_PWM++;
	if(MOTOR_RIGHT_PWM != 0 && MOTOR_RIGHT_PWM <100)
		MOTOR_RIGHT_PWM++;
}

void Speed_Down(void)
{
	if(MOTOR_LEFT_PWM > 1)		//最小速度减为1
		MOTOR_LEFT_PWM--;
	if(MOTOR_RIGHT_PWM > 1)
		MOTOR_RIGHT_PWM--;
}
