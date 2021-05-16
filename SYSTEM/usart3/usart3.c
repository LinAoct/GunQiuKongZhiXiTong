#include "USART3.h"	  


//#if EN_USART3_RX   //如果使能了接收
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
//u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART3_REC_LEN个字节.
u8 USART3_TX_BUF[100]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  

u8 IS_USART3_RX_HEAD;		//标志是否接收到数据头
u8 IS_USART3_RX_Success=0;
u8 USART3_RX_BUF[4];		//USART3接收缓冲
u8 U3_Mode;
 
 
void uart3_init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	//使能 USART3 AFIO 时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能 USART3 GPIOA 时钟
	
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);						//初始化GPIOB.10
   
  //USART3_RX	  GPIOB.11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						//PB.11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化GPIOB.11

  //USART3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
  //USART 配置
	USART_InitStructure.USART_BaudRate = bound;									//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//收发模式

  USART_Init(USART3, &USART_InitStructure); 			//初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    	//使能串口3
}


/*
*===================================================================
*		说明：串口3中断服务程序
*		参数：无
*		返回：无
*===================================================================
*/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{	  
		u8 current_data;
		static u8 data_index, last_data, last_last_data;
		current_data = USART3 -> DR;
		//printf("u3中断[%d]",current_data);
		if(IS_USART3_RX_HEAD == 0)	//未获取到数据头 0xff 0xfe
		{	
			if(last_data==0xfe && last_last_data==0xff) 
				IS_USART3_RX_HEAD=1, data_index=0;
		}
		if(IS_USART3_RX_HEAD == 1)	//已经获取到数据头 0xff 0xfe
		{	
			if(current_data == 0xfd && U3_Mode == 0)
				U3_Mode=1;
			else if(U3_Mode == 1)
			{
				USART3_RX_BUF[data_index] = current_data;
				data_index++;															//数据位索引加一
				if(data_index == 2) IS_USART3_RX_HEAD=0, IS_USART3_RX_Success=1;	//接收到2字节数据 准备重新接收
			}
			else
			{
				USART3_RX_BUF[data_index] = current_data;
				data_index++;															//数据位索引加一
				if(data_index == 1) IS_USART3_RX_HEAD=0, IS_USART3_RX_Success=1;	//接收到1字节数据 准备重新接收
			}
		}
		last_last_data = last_data;		//保存前一次接收到的位
		last_data = current_data;			//保存本次接收到的位
	}
	
	
	
	
	
	
	
//	u8 Res;
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res =USART_ReceiveData(USART3);		//读取接收到的数据
//		
//		if((USART3_RX_STA&0x8000)==0)			//接收未完成
//		{
//			if(USART3_RX_STA&0x4000)				//接收到了0x0d
//			{
//				if(Res!=0x0a)USART3_RX_STA=0;	//接收错误,重新开始
//				else USART3_RX_STA|=0x8000;		//接收完成了 
//			}
//			else //还没收到0x0D
//			{	
//				if(Res==0x0d)USART3_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;						//按位与 与前14位得到有效字节长度
//					USART3_RX_STA++;
//					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;	//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
//	} 
}


/*
*===================================================================
*		说明：串口3发送数据函数
*		参数：ch：要发送的字符串指针
*		返回：无
*===================================================================
*/
//void u3_printf(unsigned char *ch)
//{      
//	while(*ch!='\0')
//	{ 
//		while(USART_GetFlagStatus(USART3,USART_FLAG_TC ) == RESET);	
//		USART_SendData(USART3,*ch);
//		ch++;
//	}
//}

void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}


//void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )
//{
//	const char *s;
//	int d;   
//	char buf[16];
//	
//	va_list ap;
//	va_start(ap, Data);
// 
//	while ( * Data != 0 )     // 判断是否到达字符串结束符
//	{				                          
//		if ( * Data == 0x5c )  //'\'
//		{									  
//			switch ( *++Data )
//			{
//				case 'r':							          //回车符
//				USART_SendData(USARTx, 0x0d);
//				Data ++;
//				break;
// 
//				case 'n':							          //换行符
//				USART_SendData(USARTx, 0x0a);	
//				Data ++;
//				break;
// 
//				default:
//				Data ++;
//				break;
//			}			 
//		}
//		
//		else if ( * Data == '%')
//		{									  //
//			switch ( *++Data )
//			{				
//				case 's':										  //字符串
//				s = va_arg(ap, const char *);
//				
//				for ( ; *s; s++) 
//				{
//					USART_SendData(USARTx,*s);
//					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
//				}
//				
//				Data++;
//				
//				break;
// 
//				case 'd':			
//					//十进制
//				d = va_arg(ap, int);
//				
//				itoa(d, buf, 10);
//				
//				for (s = buf; *s; s++) 
//				{
//					USART_SendData(USARTx,*s);
//					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
//				}
//				
//				Data++;
//				
//				break;
//				
//				default:
//				Data++;
//				
//				break;
//				
//			}		 
//		}
//		
//		else USART_SendData(USARTx, *Data++);
//		
//		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
//		
//	}
//}














////串口接收缓存区 	
//u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
//u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

////通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
////如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
////任何数据,则表示此次接收完毕.
////接收到的数据状态
////[15]:0,没有接收到数据;1,接收到了一批数据.
////[14:0]:接收到的数据长度
//vu16 USART3_RX_STA=0;   	

//void USART3_IRQHandler(void)
//{
//	u8 res;	      
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
//	{	 
//		
//				u8 temp;
//				static u8 count,last_data,last_last_data;
//				temp=USART3->DR;
//				  LED = 0; 
//					if(Usart_Flag==0)
//						{	
//						if(last_data==0xfe&&last_last_data==0xff) 
//						Usart_Flag=1,count=0;	
//						}
//					 if(Usart_Flag==1)
//						{	
//							Urxbuf[count]=temp;     
//							count++;                
//							if(count==2)Usart_Flag=0;
//						}
//						last_last_data=last_data;
//						last_data=temp;
//		
//		
//		res =USART_ReceiveData(USART3);		 
//		if((USART3_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
//		{ 
//			if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//还可以接收数据
//			{
//				TIM_SetCounter(TIM2,0);//计数器清空          				//计数器清空
//				if(USART3_RX_STA==0) 				//使能定时器3的中断 
//				{
//					TIM_Cmd(TIM2,ENABLE);//使能定时器3
//				}
//				USART3_RX_BUF[USART3_RX_STA++]=res;	//记录接收到的值	 
//			}
//			else 
//			{
//				USART3_RX_STA|=1<<15;				//强制标记接收完成
//			} 
//		}
//	}  				 											 
//}   



////初始化IO 串口3
////pclk1:PCLK1时钟频率(Mhz)
////bound:波特率	  
//void usart3_init(u32 bound)
//{  
//	NVIC_InitTypeDef NVIC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能

// 	USART_DeInit(USART3);  //复位串口3
//		 //USART3_TX   PB10
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
//   
//    //USART3_RX	  PB11
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11
//	
//	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//  
//	USART_Init(USART3, &USART_InitStructure); //初始化串口	3
//  

//	USART_Cmd(USART3, ENABLE);                    //使能串口 
//	
//	//使能接收中断
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
//	
//	//设置中断优先级
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
//	
//	
//	TIM2_Int_Init(1000-1,7200-1);		//10ms中断
//	USART3_RX_STA=0;		//清零
//	TIM_Cmd(TIM2,DISABLE);			//关闭定时器3

//}

////串口3,printf 函数
////确保一次发送数据不超过USART3_MAX_SEND_LEN字节
//void u3_printf(char* fmt,...)  
//{  
//	u16 i,j; 
//	va_list ap; 
//	va_start(ap,fmt);
//	vsprintf((char*)USART3_TX_BUF,fmt,ap);
//	va_end(ap);
//	i=strlen((const char*)USART3_TX_BUF);		//此次发送数据的长度
//	for(j=0;j<i;j++)							//循环发送数据
//	{
//	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
//		USART_SendData(USART3,USART3_TX_BUF[j]); 
//	} 
//}
