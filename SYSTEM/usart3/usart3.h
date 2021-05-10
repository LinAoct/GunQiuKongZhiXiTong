#ifndef __USART3_H
#define __USART3_H
#include "sys.h" 
#include "stdio.h"
#include "stdarg.h"


#define USART3_REC_LEN		200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1			//使能（1）/禁止（0）串口1接收

extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART3_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         				//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void uart3_init(u32 bound);
//void u3_printf(unsigned char *ch);
void u3_printf(char* fmt,...);

#endif
