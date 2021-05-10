#ifndef __USART2_H
#define __USART2_H
#include "sys.h" 

#define USART2_MAX_REC_LEN		100  	//定义最大接收字节数
#define USART2_MAX_SEND_LEN		100

extern u8  USART2_RX_BUF[USART2_MAX_REC_LEN]; 	//接收缓冲,末字节为换行符 
extern u16 USART2_RX_STA;         							//接收状态标记	
extern u8 IS_USART2_RX_HEAD;


void uart2_init(u32 bound);
void u2_printf(char* fmt,...);

#endif
