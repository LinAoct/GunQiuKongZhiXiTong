#ifndef __USART3_H
#define __USART3_H
#include "sys.h" 


#define USART3_REC_LEN		200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1			//使能（1）/禁止（0）串口1接收

//extern u16 USART3_RX_STA;         				//接收状态标记	

void uart3_init(u32 bound);
//void u3_printf(unsigned char *ch);
void u3_printf(char* fmt,...);

#endif
