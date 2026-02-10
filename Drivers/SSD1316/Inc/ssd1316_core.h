/*
 * ssd1316_core.h
 *
 *  Created on: Jan 28, 2026
 *      Author: cshapard26
 */




#ifndef SSD1316_CORE_H_
#define SSD1316_CORE_H_

#include "stm32l4xx_hal.h"
#include <stdbool.h>


#define SSD1316_I2C_ADDR	0x3C << 1

#define SSD1316_COL_COUNT 	128
#define SSD1316_ROW_COUNT 	40 // Really 39, but prevents constant boundary checks
#define SSD1316_PAGE_COUNT 	5


#define SSD1316_WRITE_COMMAND	0x00
#define SSD1316_WRITE_DATA		0x40
#define SSD1316_READ_DATA		0xC0


#define SSD1316_SET_CONTRAST_CONTROL	0x81
#define SSD1316_SET_RAM_MODE			0xA4
#define SSD1316_SET_DISPLAY_INVERSION	0xA6
#define SSD1316_SET_REFERENCE_TYPE		0xAD
#define SSD1316_SET_DISPLAY_ENABLE		0xAE

#define SSD1316_SET_LOWER_COL_PAGE_MODE	0x00
#define SSD1316_SET_UPPER_COL_PAGE_MODE	0x10
#define SSD1316_SET_MEMORY_ADDR_MODE	0x20
#define SSD1316_SET_COL_ADDR_VH_MODE	0x21
#define SSD1316_SET_PAGE_ADDR_VH_MODE	0x22
#define SSD1316_SET_PAGE_ADDR_PAGE_MODE	0xB0

#define SSD1316_SET_SEGMENT_REMAP		0xA0
#define SSD1316_SET_CHARGE_PUMP_MODE	0x8D
#define SSD1316_NOP		0xE3


extern char buffer[SSD1316_PAGE_COUNT][SSD1316_COL_COUNT];

void SSD1316_Set_I2C_And_Reset_Ports(I2C_HandleTypeDef hi2c1, GPIO_TypeDef* GPIO_P, uint16_t GPIO_Pin1);

uint8_t _SSD1316_Send_Command(uint8_t command, uint8_t* data, uint8_t bytes);
uint8_t _SSD1316_Write_Byte(uint8_t data);


uint8_t SSD1316_Init();
void SSD1316_Reset();


uint8_t SSD1316_Set_Screen_Enable(bool enable);
uint8_t SSD1316_Set_Charge_Pump_Enable(bool enable);
uint8_t	SSD1316_Set_Screen_Invert(bool enable);
uint8_t SSD1316_Set_RAM_Mode(bool enable);
uint8_t SSD1316_Set_Reference_Type(bool iref, bool vref);
uint8_t SSD1316_Set_Contrast(uint8_t level);
uint8_t SSD1316_Set_Pre_Charge_Period(uint8_t period1, uint8_t period2);
uint8_t SSD1316_Set_Page_Address_Mode(bool page_addressing);


uint8_t SSD1316_Flip_Screen(bool enable);

void SSD1316_Set_Cursor_Position(uint8_t x, uint8_t y);
uint8_t SSD1316_Update_Screen();
uint8_t SSD1316_Update_Screen_Portion(uint8_t* rect);
uint8_t SSD1316_Clear_Screen();
void SSD1316_Clear_Buffer();

uint8_t SSD1316_Write_Character(char value, bool append);
uint8_t SSD1316_Write_Large_Digit(char value, bool append);
uint8_t SSD1316_Write_Tiny_Digit(char value, bool append);
uint8_t SSD1316_Write_String(char* string, bool wrap, bool append);
uint8_t SSD1316_Write_Large_Number(char* string, bool wrap, bool append);
uint8_t SSD1316_Write_Tiny_Number(char* string, bool wrap, bool append);
uint8_t SSD1316_Write_Image(const unsigned char* array, uint8_t dim_x, uint8_t dim_y, bool append);




#endif /* SSD1306_INC_SSD1316_CORE_H_ */
