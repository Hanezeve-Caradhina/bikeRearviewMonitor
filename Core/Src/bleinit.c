/*
 * bleinit.c
 *
 *  Created on: 2022年9月24日
 *      Author: Etern
 */


#include <stdio.h>
#include <string.h>
#include "AT.h"

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

extern struct tok tok;
extern char Buff[256];

char BAUD[6]     = "9600";
char ROLE[2][7]  = {"S", "Slave"};
char NAME[2][10] = {"HCRCVBLE", "HCRCVSPP"};
char PIN[6]      = "2501";
char TUUID[6]    = "FFE1";

char hexToSend[256], strToSend[256];

int BLE_state;

u8 Set_Default(void) {
	strcpy(tok.name, "AT+DEFAULT");
	tok.num = 0;
	strcpy(tok.ret, "OK");
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] set to default fail!\r\n");
	else printf("[BLEINIT][SUC] set to default.\r\n");
	return ret;
}

u8 BLE_reboot(void) {
	strcpy(tok.name, "AT+RESET");
	tok.num = 0;
	strcpy(tok.ret, "OK");
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] restart fail!\r\n");
	else printf("[BLEINIT][SUC] restarted.\r\n");
	return ret;
}

u8 Qst_UART_Baud(void) {
	strcpy(tok.name, "AT+BAUD");
	tok.num = 1;
	strcpy(tok.sendstr[0], "?");
	sprintf(tok.ret, "OK+BAUD=%s,NONE", BAUD);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR]Q: BAUD not correct\r\n");
	else printf("[BLEINIT][SUC]Q: BAUD now is %s\r\n", BAUD);
	return ret;
}

u8 Set_UART_Baud(void) {
	if (!Qst_UART_Baud()) return 0;
	strcpy(tok.name, "AT+BAUD");
	tok.num = 1;
	strcpy(tok.sendstr[0], BAUD);
	sprintf(tok.ret, "OK+BAUD=%s", BAUD);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] BAUD set fail!\r\n");
	else printf("[BLEINIT][SUC] BAUD set as %s\r\n", BAUD);
	return ret;
}

u8 Qst_Role(void) {
	strcpy(tok.name, "AT+ROLE");
	tok.num = 1;
	strcpy(tok.sendstr[0], "?");
	sprintf(tok.ret, "OK+ROLE=%s", ROLE[1]);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR]Q: ROLE not correct\r\n");
	else printf("[BLEINIT][SUC]Q: ROLE now is %s\r\n", ROLE[1]);
	return ret;
}

u8 Set_Role(void) {
	if (!Qst_Role()) return 0;
	strcpy(tok.name, "AT+ROLE");
	tok.num = 1;
	strcpy(tok.sendstr[0], ROLE[0]);
	sprintf(tok.ret, "OK+ROLE=%s", ROLE[1]);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] ROLE set fail!\r\n");
	else printf("[BLEINIT][SUC] ROLE set as %s\r\n", ROLE[1]);
	return ret;
}

u8 Qst_SPP_Name(void) {
	strcpy(tok.name, "AT+NAME");
	tok.num = 1;
	strcpy(tok.sendstr[0], "?");
	sprintf(tok.ret, "OK+NAME=%s", NAME[1]);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR]Q: SPP name not correct\r\n");
	else printf("[BLEINIT][SUC]Q: SPP name now is %s\r\n", NAME[1]);
	return ret;
}

u8 Set_SPP_Name(void) {
	if (!Qst_SPP_Name()) return 0;
	strcpy(tok.name, "AT+NAME");
	tok.num = 1;
	strcpy(tok.sendstr[0], NAME[1]);
	strcpy(tok.ret, "OKsetNAME");
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] SPP name set fail!\r\n");
	else printf("[BLEINIT][SUC] SPP name set as %s\r\n", NAME[1]);
	return ret;
}

u8 Qst_BLE_Name(void) {
	strcpy(tok.name, "AT+BNAME");
	tok.num = 1;
	strcpy(tok.sendstr[0], "?");
	sprintf(tok.ret, "OK+BNAME=%s", NAME[0]);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR]Q: BLE name not correct\r\n");
	else printf("[BLEINIT][SUC]Q: BLE name now is %s\r\n", NAME[0]);
	return ret;
}

u8 Set_BLE_Name(void) {
	if (!Qst_BLE_Name()) return 0;
	strcpy(tok.name, "AT+BNAME");
	tok.num = 1;
	strcpy(tok.sendstr[0], NAME[0]);
	strcpy(tok.ret, "OKsetNAME");
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] BLE name set fail!\r\n");
	else printf("[BLEINIT][SUC] BLE name set as %s\r\n", NAME[0]);
	return ret;
}

u8 Qst_PIN(void) {
	strcpy(tok.name, "AT+PIN");
	tok.num = 1;
	strcpy(tok.sendstr[0], "?");
	sprintf(tok.ret, "OK+PIN=%s", PIN);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR]Q: PIN not correct\r\n");
	else printf("[BLEINIT][SUC]Q: PIN now is %s\r\n", PIN);
	return ret;
}

u8 Set_PIN(void) {
	if (!Qst_PIN()) return 0;
	strcpy(tok.name, "AT+PIN");
	tok.num = 1;
	strcpy(tok.sendstr[0], PIN);
	strcpy(tok.ret, "OKsetPIN");
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] PIN set fail!\r\n");
	else printf("[BLEINIT][SUC] PIN set as %s\r\n", PIN);
	return ret;
}

u8 Qst_BLE(void) {
	strcpy(tok.name, "AT+BLE");
	tok.num = 1;
	strcpy(tok.sendstr[0], "?");
	sprintf(tok.ret, "OK+BLE=%s", "1");
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR]Q: BLE not correct\r\n");
	else printf("[BLEINIT][SUC]Q: BLE now is %s\r\n", "1");
	return ret;
}

u8 Set_BLE(void) {
	if (!Qst_BLE()) return 0;
	strcpy(tok.name, "AT+BLE");
	tok.num = 1;
	strcpy(tok.sendstr[0], "1");
	strcpy(tok.ret, "OK+BLE=1");
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] BLE set fail!\r\n");
	else printf("[BLEINIT][SUC] BLE set as %s\r\n", "1");
	return ret;
}

u8 Qst_TUUID(void) {
	strcpy(tok.name, "AT+TUUID");
	tok.num = 1;
	strcpy(tok.sendstr[0], "?");
	sprintf(tok.ret, "OK+TUUID=%s", TUUID);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR]Q: TUUID not correct\r\n");
	else printf("[BLEINIT][SUC]Q: TUUID now is %s\r\n", TUUID);
	return ret;
}

u8 Set_TUUID(void) {
	if (!Qst_TUUID()) return 0;
	strcpy(tok.name, "AT+TUUID");
	tok.num = 1;
	strcpy(tok.sendstr[0], TUUID);
	sprintf(tok.ret, "OK+TUUID=%s", TUUID);
	u8 ret = AT_CMD_Dispose(&tok);
	Buff_clear(&tok);
	if(ret) printf("[BLEINIT][ERR] TUUID set fail!\r\n");
	else printf("[BLEINIT][SUC] TUUID set as %s\r\n", TUUID);
	return ret;
}

#define NAN_NAN   0
#define CONN_OK   1
#define PARA_OK   2
#define ALL_GREEN 3

void BLE_INIT() {
	switch(BLE_state) {
	case NAN_NAN:
		BLE_reboot();
		HAL_Delay(1000);
		if(Set_UART_Baud()) return;
		BLE_state = CONN_OK;
		break;
	case CONN_OK:
		if(Set_Role()) return;
		if(Set_PIN()) return;
		if(Set_TUUID()) return;
		BLE_state = PARA_OK;
		break;
	case PARA_OK:
		if(Set_SPP_Name()) return;
		if(Set_BLE_Name()) return;
		if(Set_BLE()) return;
		BLE_state = ALL_GREEN;
		break;
	}
}
