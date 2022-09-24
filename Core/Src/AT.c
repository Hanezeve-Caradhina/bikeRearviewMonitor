/*
 * AT.c
 *
 *  Created on: 2022年9月23日
 *      Author: Etern
 */


#include <stdio.h>
#include <string.h>
#include "main.h"
#include "AT.h"

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

struct tok tok;

struct at_cmd_hanld_t at_cmd_hanld[] = {
	{"AT+BAUD", AT_Send, AT_Return},

	{"AT+LED", AT_Send, AT_Return},

	{"AT+RESET", AT_Send, AT_Return},
	{"AT+CLEAR", AT_Send, AT_Return},

	{"AT+BTMODE", AT_Send, AT_Return},
	{"AT+ROLE", AT_Send, AT_Return},

	{"AT+NAME", AT_Send, AT_Return},
	{"AT+PIN", AT_Send, AT_Return},
	{"AT+ADDR", AT_Send, AT_Return},
	{"AT+CLASS", AT_Send, AT_Return},

	{"AT+BLE", AT_Send, AT_Return},
	{"AT+BNAME", AT_Send, AT_Return},
	{"AT+BADDR", AT_Send, AT_Return},

	{"AT+AINT", AT_Send, AT_Return},
	{"AT+CTOUT", AT_Send, AT_Return},
	{"AT+LATENCY", AT_Send, AT_Return},
	{"AT+LUUID", AT_Send, AT_Return},
	{"AT+SUUID", AT_Send, AT_Return},
	{"AT+TUUID", AT_Send, AT_Return},

	{NULL, NULL , NULL}
};

char Buff[2048];

void str_Capitalized (char *dest, char *str) {
	int pos = 0, len = strlen (str);
	for (; pos<len; ++ pos) {
		if (str[pos]<='z' && str[pos]>='a') dest[pos]=str[pos]-32;
		else dest[pos] = str[pos];
	} dest[pos] = '\0';
}

u8 AT_CMD_Dispose(struct tok *tok) {
	struct at_cmd_hanld_t *atcmd, *match = NULL;
	char name[32];
	atcmd = at_cmd_hanld;
	str_Capitalized(name, tok->name);
	while(atcmd->atcmd) {
		if(strcmp(atcmd->atcmd, name) == 0) {
			match = atcmd; break;
		} atcmd ++;
	} if(match) return match->send_hanld(match->atcmd, tok);
	else {return 1;}
}

void CMD_Send(char *buff, char *atcmd, struct tok *tok) {
	int i = 0; char temp[256];
	sprintf (buff, "%s", atcmd);
	if (tok->num != 0) {
		for (i=0; i<tok->num; i++) {
			if(i == 0 && tok->sendstr[i][0] == '?') {
				sprintf(temp,"=%s",tok->sendstr[i]);
				strcat(buff,temp);
			} else if(i == 0 && tok->sendstr[i][0] != '?') {
				sprintf(temp,"=%s",tok->sendstr[i]);
				strcat(buff,temp);
			} else {
				sprintf(temp,",%s",tok->sendstr[i]);
				strcat(buff,temp);
			}
		}
	} strcat(buff,"\r\n");
}

u8 AT_Send(char *atcmd, struct tok *tok) {
	int i; char buff[256];
	for(i=0; i<Retime; ++ i) {
		CMD_Send(buff, atcmd, tok);
		HAL_UART_Transmit_IT(&huart2, (uint8_t*)buff, strlen(buff));
		if(!AT_Return(tok->ret, 1)) {return 0;}
	} return 1;
}

u8 AT_Return(char *str, int flag) {
	uint32_t Time_count = 2;
	Time_count = Timeout;
	memset(Buff, 0, sizeof Buff);
	while(Time_count --) {
		if (flag) HAL_UART_Receive(&huart2, (uint8_t *)Buff, sizeof Buff, 100);
		if(strstr((const char *)Buff,str)!=NULL) {return 0;}
		HAL_Delay(1);
	} return 1;
}

void Buff_clear(struct tok *tok) {
	tok->num = 0;
	memset(tok->sendstr, 0, sizeof(tok->sendstr));
	memset(tok->ret, 0, sizeof(tok->ret));
	memset(Buff, 0, sizeof(Buff));
}

