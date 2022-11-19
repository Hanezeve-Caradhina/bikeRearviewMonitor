/*
 * TFT_control.c
 *
 *  Created on: Oct 7, 2022
 *      Author: Etern
 */


#include "main.h"
#include <stdio.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

u8 BgPWMLight = 99;

#define XMAX 240
#define YMAX 400
#define TTOT 96000

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

u8 SPI_WriteByte(u8 *content, u16 size) {
	return HAL_SPI_Transmit(&hspi1, content, size, 10);
}

void SPI_WriteByte_u8(u8 content) {
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

void TFT_SelectRange(u16 xfr, u16 xto, u16 yfr, u16 yto) {
	TFT_WriteComm(0x02); TFT_WriteData(xfr >> u'an8);
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

void TFT_INIT(void) {
	TFT_ReStart();

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

}
