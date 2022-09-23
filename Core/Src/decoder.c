
#include <stdio.h>
#include <string.h>

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

#define MAX_VEHICLE_CNT 100
#define IN_PROSESS_STRING_OUTPUT

struct vehicle {
	u8  cat;
	u16 xVal;
	u16 yVal;
	u8  alt;
} data[MAX_VEHICLE_CNT];

u8 cnt;

u16 readNum (u8 **str) {
	u16 ret = 0ll;
	while (**str< '0' || **str> '9') ++ *str;
	while (**str>='0' && **str<='9') {
		ret = (ret<<1) + (ret<<3) + ((**str)^'0');
		++ *str;
	} return ret;
}

int dispose (unsigned char *str) {
	u8 *p = (u8*) strstr((const char *) str, "-=t:");
	cnt = readNum(&p);
	for (int i=0; i<cnt; ++ i) {
		data[i].cat  = readNum(&p);
		// printf("[s]%s\r\n", p);
		if (*p ^ ',') return 1;
		data[i].xVal = readNum(&p);
		// printf("[s]%s\r\n", p);
		if (*p ^ ',') return 1;
		data[i].yVal = readNum(&p);
		// printf("[s]%s\r\n", p);
		if (*p ^ ',') return 1;
		data[i].alt  = readNum(&p);
		// printf("[s]%s\r\n", p);
		if (*p ^ ';') return 1;
	} return 0;
}

void decoderDebugOutput () {
	printf("n=%d\r\n", cnt);
	for (int i=0; i<cnt; ++ i) {
		printf("[%d] ALT: %d\r\n", i, data[i].alt);
		printf("cat = %d\r\n", data[i].cat);
		printf("map = %d, %d\r\n", data[i].xVal, data[i].yVal);
	}
}
