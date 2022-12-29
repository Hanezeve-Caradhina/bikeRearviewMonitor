
#include <stdio.h>
#include <string.h>
#include "TFT_control.h"
#include "main.h"

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

#define MAX_VEHICLE_CNT 10
#define IN_PROSESS_STRING_OUTPUT

char strtmp[30];

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

#define SHOW_MODE

#define	ALT_L_SET  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET)
#define	ALT_L_CLR  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET)
#define	ALT_R_SET  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define	ALT_R_CLR  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)

void situDraw (void) {
	dataPos = !dataPos;

#ifdef SHOW_MODE
#endif

	for (int i=0; i<cnt; ++ i) {
		Draw_Circle(data[i][dataPos].xVal, data[i][dataPos].yVal, data[i][dataPos].cat==2? 30:15, BLACK);
		sprintf(strtmp, "%d", data[i][dataPos].yVal/27);
		if (data[i][dataPos].yVal/27>10) LCD_ShowString(data[i][dataPos].xVal-5, data[i][dataPos].yVal-6, (const u8*) strtmp, BLACK, BLACK, 12, 0);
		else LCD_ShowString(data[i][dataPos].xVal-2, data[i][dataPos].yVal-6, (const u8*) strtmp, BLACK, BLACK, 12, 0);
		if (data[i][dataPos].alt) {
			Draw_Circle(data[i][dataPos].xVal, data[i][dataPos].yVal, data[i][dataPos].cat==2? 28:13, BLACK);
		}
	}


	dataPos = !dataPos;


	  Draw_Circle(190, 300, 30, GBLUE);
	  sprintf(strtmp, "%d", 11);
	  LCD_ShowString(190-6, 300-6, (const u8*) strtmp, WHITE, WHITE, 12, 1);

	  Draw_Circle(263, 299, 30, GBLUE);
	  sprintf(strtmp, "%d", 11);
	  LCD_ShowString(263-6, 300-6, (const u8*) strtmp, WHITE, WHITE, 12, 1);

	ALT_L_SET;
	ALT_R_SET;

	//for (int i=0; i<cnt; ++ i)
	//	Draw_Circle(data[i][dataPos].xVal, data[i][dataPos].yVal, data[i][dataPos].cat==2? 30:15, data[i][dataPos].alt? YELLOW:(data[i][dataPos].cat==2? GREEN:GBLUE));

	for (int i=0; i<cnt; ++ i) {
		Draw_Circle(data[i][dataPos].xVal, data[i][dataPos].yVal, data[i][dataPos].cat==2? 30:15, data[i][dataPos].alt? YELLOW:(data[i][dataPos].cat==2? GREEN:GBLUE));
		sprintf(strtmp, "%d", data[i][dataPos].yVal/27);
		if (data[i][dataPos].yVal/27>10) LCD_ShowString(data[i][dataPos].xVal-5, data[i][dataPos].yVal-6, (const u8*) strtmp, WHITE, WHITE, 12, 1);
		else LCD_ShowString(data[i][dataPos].xVal-2, data[i][dataPos].yVal-6, (const u8*) strtmp, WHITE, WHITE, 12, 1);
		if (data[i][dataPos].alt) {
			if(data[i][dataPos].xVal < (LCD_W>>1)) ALT_L_CLR;
			else ALT_R_CLR;
			Draw_Circle(data[i][dataPos].xVal, data[i][dataPos].yVal, data[i][dataPos].cat==2? 28:13, YELLOW);
		}
	}


	//Draw_Circle(xpos, ypos, r, color, 1);
}
