/*
 * TFT_control.h
 *
 *  Created on: Oct 7, 2022
 *      Author: Etern
 */

#ifndef INC_TFT_CONTROL_H_
#define INC_TFT_CONTROL_H_

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

extern TIM_HandleTypeDef htim2;
extern SPI_HandleTypeDef hspi1;

void Set_TFT_Backlight_PWM(u8);
void Cnange_TFT_Backlight(void);
void TFT_INIT(void);
void TFT_ReStart(void);
void TFT_Clear(u16);
void TFT_DrawPoint(u16, u16, u16);
void TFT_DrawRect(u16, u16, u16, u16, u16);

#endif /* INC_TFT_CONTROL_H_ */
