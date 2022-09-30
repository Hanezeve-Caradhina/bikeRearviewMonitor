/*
 * AT.h
 *
 *  Created on: 2022年9月23日
 *      Author: Etern
 */

#ifndef INC_AT_H_
#define INC_AT_H_



#endif /* INC_AT_H_ */


#include "main.h"
#include <stdio.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

struct tok {
	char name[32];
	char num;
	char sendstr[20][256];
	char ret[256];
};

struct at_cmd_hanld_t {
	char *atcmd;
	u8 (*send_hanld)   (char *atcmd, struct tok *tok);
	u8 (*return_hanld) (char *str, int flag);
};

#define Timeout            10
#define Retime              2

u8 AT_CMD_Dispose(struct tok *tok);

void CMD_Send(char *buff, char *atcmd, struct tok *tok);

u8 AT_Send(char *atcmd, struct tok *tok);

u8 AT_Return(char *str, int flag);

void Buff_clear(struct tok *tok);
