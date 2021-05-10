#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"
#include "stdio.h"
#include "string.h"

u8 ESP6266_Send_Cmd(u8 *cmd,u8 *ack,u16 waittime);
char* esp8266_send_data(u8 *cmd,u16 waittime);
u8* esp8266_check_cmd(u8 *str);
void esp8266_start_trans(void);
u8 esp8266_quit_trans(void);

#endif
