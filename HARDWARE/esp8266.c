#include "esp8266.h"




/*
*===================================================================
*		说明：ESP8266初始化及进入透传模式
*		参数：无
*		返回：无
*===================================================================
*/
void esp8266_start_trans(void)
{
	u8 i;
	
	//esp8266_quit_trans();		//退出透传模式
	//delay_ms(100);
	//ESP6266_Send_Cmd((u8*)"+", (u8*)"OK", 0);
	//delay_ms(100);
	//printf("AT+SAVETRANSLINK=1,\"192.168.43.121\",15322,\"UDP\"");
//	
//	ESP6266_Send_Cmd((u8*)"AT+RST", (u8*)"OK", 300);				//ESP8266 重启  延时3S等待重启成功
//	
//	ESP6266_Send_Cmd((u8*)"AT+CWMODE=1", (u8*)"OK", 20);		//设置工作模式 1：station 模式 | 2：AP 模式 | 3：兼容 AP+station 模式
//	
//	ESP6266_Send_Cmd((u8*)"AT+CWJAP=\"o2\",\"qwertyuiopp\"", (u8*)"OK", 300);	//ESP8266 连接 WIFI

//	printf("正在连接wifi...");

//	delay_ms(1000);
//	
//	ESP6266_Send_Cmd((u8*)"AT+CIPMUX=0", (u8*)"OK", 20);		//设置连接数量 0：单路连接模式 | 1：多路连接模式

//	ESP6266_Send_Cmd((u8*)"AT+CIPMODE=1", (u8*)"OK", 20);			//进入透传模式  0：表示关闭 1：表示开启透传

//	ESP6266_Send_Cmd((u8*)"AT+CIPSTART=\"UDP\",\"192.168.43.47\",15322", (u8*)"Linked", 30);	//建立 UDP 连接   连接类型  远程服务器IP地址  远程服务器端口号 43.47  43.102 

//	ESP6266_Send_Cmd((u8*)"AT+CIPSEND", (u8*)"OK", 20);			//进入透传模式 之后可直接发送数据

	//ESP6266_Send_Cmd((u8*)"AT+CIPSTA?", (u8*)"OK", 20);
	
	for(i=0;i<2;i++)
	{
		printf("HELLO FROM STM32");
		delay_ms(500);
	}
	printf("ESP8266 Ready");
	IS_ESP8266_READY = 1;

	//建立TCP连接  这四项分别代表了 要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
//	while(ESP6266_Send_Cmd("AT+CIPSTART=\"TCP\",\"47.107.145.82\",320","Linked",5000))
//	{
//		printf("正在连接远程服务器...");
//	}
	
}

/*
*===================================================================
*		说明：ESP8266退出透传模式 连续发送3个 +
					每个 + 号之间不超过10ms 可认为是连续三次发送 +
*		参数：无
*		返回：退出成功1 | 退出失败 0
*===================================================================
*/
u8 esp8266_quit_trans(void)
{
	u8 result;				//保存返回结果变量
	USART_SendData(USART1, '+');
	USART_SendData(USART1, '+');
	USART_SendData(USART1, '+');
	delay_ms(200);		//等待 200ms 退出
	result = ESP6266_Send_Cmd((u8*)"AT", (u8*)"OK", 20);		//退出透传判断.
	if(result)
		printf("退出透传模式失败!");
	else
		printf("透传模式退出成功!");
	return result;
}


//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 ESP6266_Send_Cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART_RX_STA=0;
	printf("%c", cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ESP8266:%s",(u8*)ack);
					break;//得到有效数据 
				}
				//USART_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 

////ESP8266发送命令后,检测接收到的应答
////str:期待的应答结果
////返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART_RX_STA&0X8000)		//接收到一次数据了
	{
		USART_RX_BUF[USART_RX_STA&0X7FFF]=0;//添加结束符
		printf("%c",USART_RX_BUF[0]);
		strx=strstr((const char*)USART_RX_BUF,(const char*)str);
	}
	return (u8*)strx;
}


////向ESP8266发送数据
////cmd:发送的命令字符串;waittime:等待时间(单位:10ms)
////返回值:发送数据后，服务器的返回验证码

//char* esp8266_send_data(u8 *cmd,u16 waittime)
//{
//	char temp[20];
//	char *ack=temp;
//	USART2_RX_STA=0;
//	u2_printf("%s\r\n", cmd);	//发送命令
//	if(waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_ms(10);
//			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
//			{
//				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
//				ack=(char*)USART2_RX_BUF;
//				printf("服务器指令:%s\t",(u8*)ack);
//				USART2_RX_STA=0;
//				break;//得到有效数据 
//			} 
//		}
//	}
//	return ack;
//} 
