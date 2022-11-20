/*
 * battery.c
 *
 *  Created on: Sep 30, 2022
 *      Author: Etern
 */

#include "main.h"
#include <stdio.h>
#include "TFT_control.h"

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

u32 adcVal;
u8  batVal;
u16 adcBase = 1<<6;

char strtmp[30];

void ADC_Calibration() {
	// HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_Delay(100);
}

u32 ADC_Read_Data() {
	HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 50);
	return HAL_ADC_GetValue(&hadc1);
}

void Read_Battery_Life() {
	adcVal = ADC_Read_Data(&hadc1);
	// double RealVoltage = adcVal * (double)3.3 / (double) adcBase;
	printf("[ADC Voltage] %d\r\n", (int)adcVal);
	sprintf(strtmp, "BAT:%d%%", (int)adcVal*100/256);
	LCD_ShowString(225, 0, (const u8*) strtmp, 0X01CF, 0XC618, 16, 0);
}
