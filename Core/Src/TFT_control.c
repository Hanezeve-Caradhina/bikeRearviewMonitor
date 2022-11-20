/*
 * TFT_control.c
 *
 *  Created on: Oct 7, 2022
 *      Author: Etern
 */


#include "main.h"
#include <stdio.h>
#include "TFT_control.h"
#include "ascii_table.h"

//#define HX_USAGE
#define NWE_USAGE

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

u8 BgPWMLight = 99;

#define ADDR_PREFIX 0x70
#define DATA_PREFIX 0x72

void Set_TFT_Backlight_PWM(u8 PWMtoSet) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, PWMtoSet);
}

void Cnange_TFT_Backlight() {
	BgPWMLight = BgPWMLight<99? BgPWMLight+33:0;
	Set_TFT_Backlight_PWM(BgPWMLight);
	printf("[INT0] BGLT: %d\r\n", BgPWMLight);
}

#define	DISPLAY_CS_SET  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define	DISPLAY_CS_CLR  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)

#define	DISPLAY_RS_SET  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define	DISPLAY_RS_CLR  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)

#define	DISPLAY_RST_SET  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define	DISPLAY_RST_CLR  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)

#define	DISPLAY_MOSI_SET  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define	DISPLAY_MOSI_CLR  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)

#define	DISPLAY_SCK_SET  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define	DISPLAY_SCK_CLR  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)

const int USE_HORIZONTAL = 2;

uint8_t oldMap[LCD_W][LCD_H];

void TFT_WriteBus(u8 dat) {
	DISPLAY_CS_CLR;
	for (u8 i=0; i<8; ++ i) {
		DISPLAY_SCK_CLR;
		if (dat & 0x80) DISPLAY_MOSI_SET;
		else DISPLAY_MOSI_CLR;
		DISPLAY_SCK_SET;
		dat <<= 1;
	} DISPLAY_CS_SET;
}

u8 SPI_WriteByte(u8 *content, u16 size) {
	return 0;
	//return HAL_SPI_Transmit(&hspi1, content, size, 10);
}

void SPI_WriteByte_u8(u8 content) {
	//TFT_WriteBus(content);
	HAL_SPI_Transmit(&hspi1, &content, 1, 10);
	//SPI_WriteByte(&content, 1);
}

void TFT_WriteComm(u8 addr) {
	DISPLAY_CS_CLR;
	DISPLAY_RS_CLR;
	//SPI_WriteByte_u8(ADDR_PREFIX);
	SPI_WriteByte_u8(addr);
	DISPLAY_CS_SET;
}

void TFT_WriteData(u8 data) {
	DISPLAY_CS_CLR;
	DISPLAY_RS_SET;
	//SPI_WriteByte_u8(DATA_PREFIX);
	SPI_WriteByte_u8(data);
	DISPLAY_CS_SET;
}

void TFT_WriteData_u16(u16 data) {
	DISPLAY_CS_CLR;
	DISPLAY_RS_SET;
	//SPI_WriteByte_u8(DATA_PREFIX);
	SPI_WriteByte_u8(data >> 8);
	SPI_WriteByte_u8(data&0xFF);
	DISPLAY_CS_SET;
}

void TFT_WriteRegi(u8 addr, u16 data) {
	TFT_WriteComm(addr);
	TFT_WriteData_u16(data);
}

void TFT_ReStart(void) {
	DISPLAY_RST_CLR;
	HAL_Delay(200);
	DISPLAY_RST_SET;
	HAL_Delay(100);
}

void LCD_WR_REG(u8 dat) {
	DISPLAY_RS_CLR;
	//SPI_WriteByte_u8(dat>>8);
	SPI_WriteByte_u8(dat);
	DISPLAY_RS_SET;
}

void LCD_WR_DATA8(u16 dat) {
	DISPLAY_RS_SET;
	SPI_WriteByte_u8(dat);
}

void LCD_WR_DATA(u16 dat) {
	DISPLAY_RS_SET;
	SPI_WriteByte_u8(dat>>8);
	SPI_WriteByte_u8(dat);
}

void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2) {
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c);
}

void TFT_SelectRange(u16 xfr, u16 xto, u16 yfr, u16 yto) {
	TFT_WriteComm(0x02); TFT_WriteData(xfr >> 8);
	TFT_WriteComm(0x03); TFT_WriteData(xfr&0xFF);
	TFT_WriteComm(0x04); TFT_WriteData(xto >> 8);
	TFT_WriteComm(0x05); TFT_WriteData(xto&0xFF);

	TFT_WriteComm(0x06); TFT_WriteData(yfr >> 8);
	TFT_WriteComm(0x07); TFT_WriteData(yfr&0xFF);
	TFT_WriteComm(0x08); TFT_WriteData(yto >> 8);
	TFT_WriteComm(0x09); TFT_WriteData(yto&0xFF);

	TFT_WriteComm(0x22);
}

void TFT_DrawRect(u16 xpos, u16 ypos, u16 w, u16 h, u16 color) {
	TFT_SelectRange(xpos, xpos+w-1, ypos, ypos+h-1);
	DISPLAY_CS_CLR;
	DISPLAY_RS_SET;
	for (u16 i=0; i<h; ++ i) {
		for (u16 j=0; j<w; ++ j) {
			SPI_WriteByte_u8(color >> 8);
			SPI_WriteByte_u8(color&0xFF);
		}
	} DISPLAY_CS_SET;
}

void TFT_DrawPoint(u16 xpos, u16 ypos, u16 color) {
	TFT_SelectRange(xpos, xpos, ypos, ypos);
	DISPLAY_CS_CLR;
	DISPLAY_RS_SET;
	SPI_WriteByte_u8(color>>8);
	SPI_WriteByte_u8(color);
	DISPLAY_CS_SET;
}

void TFT_Clear(u16 color) {
	TFT_SelectRange(0, XMAX-1, 0, YMAX-1);
	DISPLAY_CS_CLR;
	DISPLAY_RS_SET;
	for (u8 i=0; i < TTOT; ++ i) {
		SPI_WriteByte_u8(color >> 8);
		SPI_WriteByte_u8(color&0xFF);
	} DISPLAY_CS_SET;
}

void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color) {
	u16 i,j;
	LCD_Address_Set(xsta, ysta, xend-1, yend-1);
	for(i = ysta; i<yend; ++ i) {
		for(j = xsta; j<xend; ++ j) {
			LCD_WR_DATA(color);
		}
	}
}


void LCD_DrawPoint(u16 x, u16 y, u16 color) {
	if (x<0||x>LCD_W||y<0||y>LCD_H) return;
	LCD_Address_Set(x, y, x, y);
	LCD_WR_DATA(color);
}

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color) {
	u16 t;
	int xerr=0, yerr=0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2-x1; //计算坐标增量
	delta_y = y2-y1;
	uRow = x1;//画线起点坐标
	uCol = y1;
	if(delta_x > 0) incx = 1; //设置单步方向
	else if (delta_x == 0) incx = 0;//垂直线
	else {incx = -1; delta_x = -delta_x;}
	if(delta_y > 0) incy = 1;
	else if (delta_y == 0) incy = 0;//水平线
	else {incy = -1; delta_y = -delta_y;}
	if(delta_x > delta_y) distance = delta_x; //选取基本增量坐标轴
	else distance = delta_y;
	for(t = 0; t<distance+1; ++ t) {
		LCD_DrawPoint(uRow, uCol, color);
		xerr += delta_x;
		yerr += delta_y;
		if(xerr > distance) {
			xerr -= distance;
			uRow += incx;
		} if(yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
}

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color) {
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}

void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color) {
	int a,b;
	a = 0; b = r;
	while(a <= b) {
		LCD_DrawPoint(x0-b, y0-a, color);             //3
		LCD_DrawPoint(x0+b, y0-a, color);             //0
		LCD_DrawPoint(x0-a, y0+b, color);             //1
		LCD_DrawPoint(x0-a, y0-b, color);             //2
		LCD_DrawPoint(x0+b, y0+a, color);             //4
		LCD_DrawPoint(x0+a, y0-b, color);             //5
		LCD_DrawPoint(x0+a, y0+b, color);             //6
		LCD_DrawPoint(x0-b, y0+a, color);             //7
		++ a;
		if((a*a+b*b)>(r*r)) b --;
	}
}


void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode) {
	u8 temp, sizex, t, m = 0;
	u16 i, TypefaceNum;
	u16 x0 = x;
	sizex = sizey/2;
	TypefaceNum = (sizex/8+((sizex%8)?1:0))*sizey;
	num = num-' ';
	LCD_Address_Set(x, y, x+sizex-1, y+sizey-1);
	for(i = 0; i<TypefaceNum; ++ i) {
		if(sizey==12) temp=ascii_1206[num][i];
		else if(sizey==16) temp=ascii_1608[num][i];
		//else if(sizey==24) temp=ascii_2412[num][i];
		//else if(sizey==32) temp=ascii_3216[num][i];
		else return;
		for(t = 0; t<8; ++ t) {
			if(!mode) {
				if(temp&(0x01<<t)) LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				++ m;
				if(m%sizex==0) {m=0; break;}
			} else {
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);
				++ x;
				if((x-x0)==sizex) {x=x0; y++;break;}
			}
		}
	}
}


void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode) {
	while(*p!='\0') {
		LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
		x += sizey/2; ++ p;
	}
}


void TFT_INIT(void) {
	TFT_ReStart();

#ifdef HX_USAGE

	//HX8352B
	TFT_WriteComm(0x00E2); TFT_WriteData(0x0015);
	TFT_WriteComm(0x00E5); TFT_WriteData(0x0018);
	TFT_WriteComm(0x00E7); TFT_WriteData(0x0018);
	TFT_WriteComm(0x00E8); TFT_WriteData(0x0040);
	TFT_WriteComm(0x00EC); TFT_WriteData(0x0009);
	TFT_WriteComm(0x00ED); TFT_WriteData(0x0006);
	TFT_WriteComm(0x00EE); TFT_WriteData(0x0020);//0x0020
	TFT_WriteComm(0x00EF); TFT_WriteData(0x0050);
	TFT_WriteComm(0x0029); TFT_WriteData(0x0001);
	TFT_WriteComm(0x002B); TFT_WriteData(0x0003);
	TFT_WriteComm(0x002E); TFT_WriteData(0x0085);

	// Power on Setting
	TFT_WriteComm(0x0017); TFT_WriteData(0x0005);  //18BIT:0006,16BIT:0005,
	TFT_WriteComm(0x0023); TFT_WriteData(0x0076);
	TFT_WriteComm(0x0024); TFT_WriteData(0x0050); //0x57,0x4e
	TFT_WriteComm(0x0025); TFT_WriteData(0x0071); //0x67
	TFT_WriteComm(0x002f); TFT_WriteData(0x0001);  //1: 1_LINE INVERSION,0:FRAME INVERSION
	TFT_WriteComm(0x001B); TFT_WriteData(0x001E);
	TFT_WriteComm(0x0001); TFT_WriteData(0x0000);
	TFT_WriteComm(0x001C); TFT_WriteData(0x0004);

	// Power on sequence
	TFT_WriteComm(0x0018); TFT_WriteData(0x00CC);
	TFT_WriteComm(0x0019); TFT_WriteData(0x0001);
	HAL_Delay(5);
	TFT_WriteComm(0x001F); TFT_WriteData(0x008C);
	TFT_WriteComm(0x001F); TFT_WriteData(0x0084);
	HAL_Delay(10);
	TFT_WriteComm(0x001F); TFT_WriteData(0x0094);
	HAL_Delay(10);
	TFT_WriteComm(0x001F); TFT_WriteData(0x00D4);
	HAL_Delay(5);

	// Gamma Setting
	TFT_WriteComm(0x0040); TFT_WriteData(0x0008);
	TFT_WriteComm(0x0041); TFT_WriteData(0x0031);
	TFT_WriteComm(0x0042); TFT_WriteData(0x002F);
	TFT_WriteComm(0x0043); TFT_WriteData(0x003E);
	TFT_WriteComm(0x0044); TFT_WriteData(0x003D);
	TFT_WriteComm(0x0045); TFT_WriteData(0x003F);
	TFT_WriteComm(0x0046); TFT_WriteData(0x002F);
	TFT_WriteComm(0x0047); TFT_WriteData(0x0079);
	TFT_WriteComm(0x0048); TFT_WriteData(0x0008);
	TFT_WriteComm(0x0049); TFT_WriteData(0x0006);
	TFT_WriteComm(0x004A); TFT_WriteData(0x0008);
	TFT_WriteComm(0x004B); TFT_WriteData(0x000E);
	TFT_WriteComm(0x004C); TFT_WriteData(0x0017);
	TFT_WriteComm(0x0050); TFT_WriteData(0x0000);
	TFT_WriteComm(0x0051); TFT_WriteData(0x0002);
	TFT_WriteComm(0x0052); TFT_WriteData(0x0001);
	TFT_WriteComm(0x0053); TFT_WriteData(0x0010);
	TFT_WriteComm(0x0054); TFT_WriteData(0x000E);
	TFT_WriteComm(0x0055); TFT_WriteData(0x0037);
	TFT_WriteComm(0x0056); TFT_WriteData(0x0006);
	TFT_WriteComm(0x0057); TFT_WriteData(0x0050);
	TFT_WriteComm(0x0058); TFT_WriteData(0x0008);
	TFT_WriteComm(0x0059); TFT_WriteData(0x0011);
	TFT_WriteComm(0x005A); TFT_WriteData(0x0017);
	TFT_WriteComm(0x005B); TFT_WriteData(0x0019);
	TFT_WriteComm(0x005C); TFT_WriteData(0x0017);
	TFT_WriteComm(0x005D); TFT_WriteData(0x00FF);

	 //Window set
	TFT_WriteComm(0x0002); TFT_WriteData(0x0000); // Column Address Start Register, High 8 bit
	TFT_WriteComm(0x0003); TFT_WriteData(0x0000); // Column Address Start Register, Low 8bit
	TFT_WriteComm(0x0004); TFT_WriteData(0x0000); // Column Address end Register, High 8 bit
	TFT_WriteComm(0x0005); TFT_WriteData(0x00EF); // Column Address end Start Register, Low 8 bit
	TFT_WriteComm(0x0006); TFT_WriteData(0x0000); // Row Address Start Register, High 8 bit
	TFT_WriteComm(0x0007); TFT_WriteData(0x0000); // Row Address Start Register, Low 8bit
	TFT_WriteComm(0x0008); TFT_WriteData(0x0001); // Row Address end Register, High 8 bit
	TFT_WriteComm(0x0009); TFT_WriteData(0x008F); // Row Address end Register, Low 8bit

	// Set start address
	TFT_WriteComm(0x80); TFT_WriteData(0x00);
	TFT_WriteComm(0x81); TFT_WriteData(0x00);//CAC[8:0]
	TFT_WriteComm(0x82); TFT_WriteData(0x00);
	TFT_WriteComm(0x83); TFT_WriteData(0x00);//RAC[8:0]

	// Display ON Setting
	TFT_WriteComm(0x0016); TFT_WriteData(0x00C8);//0x0B,0x09,0xC8
	TFT_WriteComm(0x0028); TFT_WriteData(0x0020);
	HAL_Delay(40);
	TFT_WriteComm(0x0028); TFT_WriteData(0x0038);
	HAL_Delay(40); // Waiting 2 frames al least
	TFT_WriteComm(0x0028); TFT_WriteData(0x003C);
#endif
#ifdef NWE_USAGE
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out
	HAL_Delay(120);    //Delay 120ms
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0xc3);
	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0x96);
	LCD_WR_REG(0x36);    // Memory Access Control
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x48);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0x88);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x28);
	else LCD_WR_DATA8(0xE8);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0Xe8);
	LCD_WR_DATA8(0x40);
	LCD_WR_DATA8(0x82);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x0a);
	LCD_WR_DATA8(0xb6);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0Xc5);
	LCD_WR_DATA8(0x27);

	LCD_WR_REG(0Xc2);
	LCD_WR_DATA8(0xa7);

	LCD_WR_REG(0Xe0);
	LCD_WR_DATA8(0xf0);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0f);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x1d);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x0c);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x15);

	LCD_WR_REG(0Xe1);
	LCD_WR_DATA8(0xf0);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x0a);
	LCD_WR_DATA8(0x0b);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x0c);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x17);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x16);

	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0x3c);

	LCD_WR_REG(0Xf0);
	LCD_WR_DATA8(0x69);

	LCD_WR_REG(0X29);

	//LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
#endif
}
