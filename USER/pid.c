#include "pid.h"

//float x_side_Kp = 0.0, x_side_Ki = 0.0, x_side_Kd = 0.0;		//X 轴位置环 PID相关参数
//float y_side_Kp = 0.0, y_side_Ki = 0.0, y_side_Kd = 0.0;			//Y 轴位置环 PID相关参数
//float x_speed_Kp = 0.0, x_speed_Ki = 0.0, x_speed_Kd = 0.0;					//PID相关参数
//float y_speed_Kp = 0.0, y_speed_Ki = 0.0, y_speed_Kd = 0.0;		//PID相关参数	

float x_side_Kp = 20.0, x_side_Ki = 0.1, x_side_Kd = 145.0;				//X轴位置环 PID参数
float y_side_Kp = 20.0, y_side_Ki = 0.1, y_side_Kd = 150.0;				//Y轴位置环 PID参数

float x_speed_Kp = -10.0, x_speed_Ki = 0.0, x_speed_Kd = 50.0;			//X轴速度环 PID参数
float y_speed_Kp = -8.0, y_speed_Ki = 0.0, y_speed_Kd = 55.0;	//Y轴速度环 PID参数

/*
*===================================================================
*		说明：X方向平衡PID控制
*		参数：current_side <float> 当前位置信息
*		返回：X方向位置控制PWM
*===================================================================
*/
int PID_Side_X(float current_side)
{
	float Differential, Bias;		//定义差分变量和偏差变量
	static float Last_Bias, Integration, Balance_Integration;  //上一次的偏差值

	Bias = current_side - target_point_X;  	//计算当前偏差值 
	Differential = Bias - Last_Bias;  			//计算偏差的变化率	 
//	if(++Flag_Target > 30) 						//错频处理积分控制
//	{
//		Flag_Target = 0;
		Integration += Bias;
		if(Integration<-40) Integration = -40;	//积分限幅
		if(Integration>40)  Integration = 40;	
		Balance_Integration = Integration * x_side_Ki;  //积分控制
	//}		
	Last_Bias = Bias;  //保存本次的偏差值
	return (int)(x_side_Kp * Bias)/100 + x_side_Kd*Differential/10 + Balance_Integration;   //计算位置控制输出量
}

/*
*===================================================================
*		说明：Y方向平衡PID控制
*		参数：current_side <float> 当前位置信息
*		返回：Y方向位置控制PWM
*===================================================================
*/
int PID_Side_Y(float current_side)
{  
	float Differential, Bias;		//定义差分变量和偏差变量
	static float Last_Bias, Integration, Balance_Integration;  //上一次的偏差值

	Bias = current_side - target_point_Y;		//计算当前偏差值  
	Differential = Bias - Last_Bias;  			//计算偏差的变化率	 
//	if(++Flag_Target > 30) 								//错频处理积分控制
//	{
//		Flag_Target = 0;
		Integration += Bias;
		if(Integration<-40) Integration = -40;					//积分限幅
		if(Integration>40)  Integration = 40;	
		Balance_Integration = Integration * y_side_Ki;	//积分控制
	//}		

	Last_Bias = Bias;  //保存本次的偏差值
	return (int)(y_side_Kp * Bias)/100 + y_side_Kd*Differential/10 + Balance_Integration;   //计算位置控制输出量
}

/*
*===================================================================
*		说明：X方向速度环PD控制
*		参数：current_side <float> 当前位置信息
*		返回：X方向速度控制PWM
*===================================================================
*/
int PID_Speed_X(float current_side)
{
	float Differential, Bias;		//定义差分变量和偏差变量
	static float Last_Bias, Integration, Balance_Integration, last_side;  //上一次的偏差值
	int speed_now;
	
	speed_now = current_side - last_side;
	Bias = speed_now - speed_max;  		//计算速度偏差
	Differential = Bias - Last_Bias;  //计算速度偏差变化率	 

	Integration += Bias;										//积分作用 累计误差
	if(Integration<-40) Integration = -40;					//积分限幅
	if(Integration>40)  Integration = 40;	
	Balance_Integration = Integration * x_speed_Ki;	//积分控制
	
	Last_Bias = Bias;  					//保存本次偏差值
	last_side = current_side;		//保存本次位置值
	return (int)(x_speed_Kp * Bias)/100 + x_speed_Kd*Differential/10 + Balance_Integration;   //计算速度控制输出量
}

/*
*===================================================================
*		说明：Y方向速度环PD控制
*		参数：current_side <float> 当前位置信息
*		返回：Y方向速度控制PWM
*===================================================================
*/
int PID_Speed_Y(float current_side)
{  
	float Differential, Bias;		//定义差分变量和偏差变量
	static float Last_Bias, Integration, Balance_Integration, last_side;  //上一次的偏差值
	int speed_now;
	
	speed_now = current_side - last_side;
	Bias = speed_now - speed_max;  		//计算速度偏差
	Differential = Bias - Last_Bias;  //计算速度偏差变化率 

	Integration += Bias;										//积分作用 累计误差
	if(Integration<-40) Integration = -40;					//积分限幅
	if(Integration>40)  Integration = 40;	
	Balance_Integration = Integration * y_speed_Ki;	//积分控制

	Last_Bias = Bias;  					//保存本次偏差值
	last_side = current_side;		//保存本次位置值
	return (int)(y_speed_Kp * Bias)/100 + y_speed_Kd*Differential/10 + Balance_Integration;   //计算速度控制输出量
}
