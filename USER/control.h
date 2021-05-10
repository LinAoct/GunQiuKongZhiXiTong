#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define PI 3.14159265

void Mode_Stop(void);
void Mode_Base_1(void);
void Mode_Base_2(void);
void Mode_Base_3(void);
void Mode_Base_4(void);
void Mode_Plus_1(void);
void Mode_Plus_2(void);
void Mode_Plus_3(void);
void Mode_Go_Num(u8 num);
void Mode_Go_1_To_9(void);
void Mode_Draw_Square(void);
void PID_Limit(int *x_pid, int *y_pid);
void Mode_Init(void);

#endif
