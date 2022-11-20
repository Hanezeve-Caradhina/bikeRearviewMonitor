
#include <stdio.h>
#include <string.h>
#include "TFT_control.h"
#include "main.h"

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

#define MAX_VEHICLE_CNT 10
#define IN_PROSESS_STRING_OUTPUT

struct vehicle {
	u8  cat;
	u16 xVal;
	u16 yVal;
	u8  alt;
} data[10][2];

u8 cnt, dataPos;

u16 readNum (u8 **str) {
	u16 ret = 0ll;
	while (**str< '0' || **str> '9') ++ *str;
	while (**str>='0' && **str<='9') {
		ret = (ret<<1) + (ret<<3) + ((**str)^'0');
		++ *str;
	} return ret;
}

int dispose (unsigned char *str) {
	dataPos = !dataPos;
	u8 *p = (u8*) strstr((const char *) str, "-=t:");
	cnt = readNum(&p);
	for (int i=0; i<cnt; ++ i) {
		data[i][dataPos].cat  = readNum(&p);
		if (*p ^ ',') return 1;
		data[i][dataPos].xVal = readNum(&p);
		if (*p ^ ',') return 1;
		data[i][dataPos].yVal = readNum(&p);
		if (*p ^ ',') return 1;
		data[i][dataPos].alt  = readNum(&p);
		if (*p ^ ';') return 1;
	} return 0;
}

void decoderDebugOutput () {
	printf("n=%d\r\n", cnt);
	for (int i=0; i<cnt; ++ i) {
		printf("[%d] ALT: %d\r\n", i, data[i][dataPos].alt);
		printf("cat = %d\r\n", data[i][dataPos].cat);
		printf("map = %d, %d\r\n", data[i][dataPos].xVal, data[i][dataPos].yVal);
	}
}

void situDraw (void) {
	dataPos = !dataPos;

	for (int i=0; i<cnt; ++ i)
		Draw_Circle(data[i][dataPos].xVal, data[i][dataPos].yVal, data[i][dataPos].cat==2? 30:15, BLACK);

	dataPos = !dataPos;

	for (int i=0; i<cnt; ++ i)
		Draw_Circle(data[i][dataPos].xVal, data[i][dataPos].yVal, data[i][dataPos].cat==2? 30:15, data[i][dataPos].alt? YELLOW:(data[i][dataPos].cat==2? GREEN:GBLUE));

	//Draw_Circle(xpos, ypos, r, color, 1);
}
