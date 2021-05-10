#include "USART2.h"	  


//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_MAX_REC_LEN];		//接收缓冲
u8 USART2_TX_BUF[100]; 									//发送缓冲
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA = 0;       	//接收状态标记	  
u8 IS_USART2_RX_HEAD = 0;			//标志是否已经接收到数据头

/*
*===================================================================
*		说明：串口2初始化函数
*		参数：无
*		返回：无
*===================================================================
*/
void uart2_init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	//使能 USART2 AFIO 时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能 USART2 GPIOA 时钟
	
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 				//PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);						//初始化GPIOA.2
   
  //USART2_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;							//PA.3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.3

  //USART2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
  //USART 配置
	USART_InitStructure.USART_BaudRate = bound;									//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//收发模式

  USART_Init(USART2, &USART_InitStructure); 			//初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    	//使能串口2
}


/*
*===================================================================
*		说明：串口2中断服务程序
*		参数：无
*		返回：无
*===================================================================
*/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET || (USART2->SR & (1 << 3)))  	//接收中断 读取SR寄存器第三位防止产生过载错误 ORE 
	{	
		u8 current_data;
		static u8 data_index, last_data, last_last_data;
		//current_data = USART2 -> SR;
		current_data = USART2 -> DR;	//读取当前字节
		if(IS_USART2_RX_HEAD == 0)		//未获取到数据头 0xff 0xfe
		{	
			if(last_data==0xfe && last_last_data==0xff) 
				IS_USART2_RX_HEAD=1, data_index=0;
		}
		if(IS_USART2_RX_HEAD == 1)	//已经获取到数据头 0xff 0xfe
		{	
			USART2_RX_BUF[data_index] = current_data;
			data_index++;															//数据位索引加1
			if(data_index == 2) IS_USART2_RX_HEAD=0;	//接收到2字节数据  已经接收完毕  准备重新接收
		}
		last_last_data = last_data;		//保存前一次接收到的字节
		last_data = current_data;			//保存本次接收到的字节
	}
}


void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(USART2,USART2_TX_BUF[j]); 
	} 
}
