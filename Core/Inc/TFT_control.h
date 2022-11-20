/*
 * TFT_control.h
 *
 *  Created on: Oct 7, 2022
 *      Author: Etern
 */

#include "main.h"

#ifndef INC_TFT_CONTROL_H_
#define INC_TFT_CONTROL_H_

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

extern TIM_HandleTypeDef htim2;
extern SPI_HandleTypeDef hspi1;

extern u8 cnt;

void Set_TFT_Backlight_PWM(u8);
void Cnange_TFT_Backlight(void);
void TFT_INIT(void);
void TFT_ReStart(void);
void TFT_Clear(u16);
void TFT_DrawPoint(u16, u16, u16);
void TFT_DrawRect(u16, u16, u16, u16, u16);

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_DrawPoint(u16 x,u16 y,u16 color);
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);

void situDraw(void);

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);

#define LCD_W 480
#define LCD_H 320

#define XMAX 240
#define YMAX 400
#define TTOT 96000

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40
#define BRRED 			 0XFC07
#define GRAY  			 0X8430
#define DARKBLUE      	 0X01CF
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458
#define LIGHTGREEN     	 0X841F
#define LGRAY 			 0XC618
#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12

#endif /* INC_TFT_CONTROL_H_ */

