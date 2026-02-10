/*
 * ssd1316_core.c
 *
 *  Created on: Jan 28, 2026
 *      Author: cshapard26
 */

#include "../Inc/ssd1316_core.h"
#include "../Inc/fonts.h"
#include <string.h>



uint8_t cursor_x;
uint8_t cursor_y;

I2C_HandleTypeDef hi2c;
GPIO_TypeDef* RES_Port;
uint16_t RES_Pin;

char buffer[SSD1316_PAGE_COUNT][SSD1316_COL_COUNT];



void SSD1316_Set_I2C_And_Reset_Ports(I2C_HandleTypeDef hi2c1, GPIO_TypeDef* GPIO_P, uint16_t GPIO_Pin1) {
	hi2c = hi2c1;
	RES_Port = GPIO_P;
	RES_Pin = GPIO_Pin1;
}


uint8_t _SSD1316_Send_Command(uint8_t command, uint8_t* data, uint8_t bytes) {
	uint8_t full_command[2 + bytes];
	full_command[0] = SSD1316_WRITE_COMMAND;
	full_command[1] = command;
	if (bytes > 0 || (data == 0)) {
		memcpy(full_command + 2, data, bytes);
	}


	return (HAL_I2C_Master_Transmit(&hi2c, SSD1316_I2C_ADDR, full_command, 2 + bytes, 10) != HAL_OK);
}


uint8_t _SSD1316_Write_Byte(uint8_t data) {
	uint8_t full_command[2] = {SSD1316_WRITE_DATA,  data};
	return (HAL_I2C_Master_Transmit(&hi2c, SSD1316_I2C_ADDR, full_command, 2 , 10) != HAL_OK);
}


uint8_t SSD1316_Init() {
	if (!RES_Pin) return 1;		// Must run _SSD1316_Set_I2C_And_Reser_Ports() first


	SSD1316_Reset();
	if (SSD1316_Set_Charge_Pump_Enable(true) != HAL_OK) return 1;
	HAL_Delay(100);

	if (SSD1316_Set_Page_Address_Mode(false) != HAL_OK) return 1;
	if (SSD1316_Set_Reference_Type(true, false) != HAL_OK) return 1;
	if (SSD1316_Flip_Screen(true) != HAL_OK) return 1;


	SSD1316_Set_Cursor_Position(0, 0);
	//if (SSD1316_Clear_Screen() != HAL_OK) return 1;

	return 0;
}


void SSD1316_Reset() {
	HAL_GPIO_WritePin(RES_Port, RES_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(RES_Port, RES_Pin, GPIO_PIN_SET);
	return;
}

uint8_t SSD1316_Set_Screen_Enable(bool enable) {
	uint8_t command =  SSD1316_SET_DISPLAY_ENABLE | enable;
	return _SSD1316_Send_Command(command, 0, 0);
}


uint8_t SSD1316_Set_Charge_Pump_Enable(bool enable) {
	uint8_t command =  SSD1316_SET_CHARGE_PUMP_MODE;
	uint8_t data = 0x00 | (enable << 2);
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t	SSD1316_Set_Screen_Invert(bool enable) {
	uint8_t command =  SSD1316_SET_DISPLAY_INVERSION | enable;
	return _SSD1316_Send_Command(command, 0, 0);
}


uint8_t SSD1316_Set_RAM_Mode(bool enable) {
	uint8_t command =  SSD1316_SET_RAM_MODE | !enable;
	return _SSD1316_Send_Command(command, 0, 0);
}


uint8_t SSD1316_Set_Reference_Type(bool iref, bool vref) {
	uint8_t command =  SSD1316_SET_REFERENCE_TYPE;
	uint8_t data = 0x00 | (iref << 4) | (vref << 1);
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t SSD1316_Set_Contrast(uint8_t level) {
	uint8_t command =  SSD1316_SET_CONTRAST_CONTROL;
	return _SSD1316_Send_Command(command, &level, 1);
}


uint8_t SSD1316_Set_Page_Address_Mode(bool page_addressing) {
	uint8_t command =  SSD1316_SET_MEMORY_ADDR_MODE;
	uint8_t data = 0x00 | (page_addressing << 1);
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t SSD1316_Flip_Screen(bool enable) {
	uint8_t command =  SSD1316_SET_SEGMENT_REMAP | enable;
	return _SSD1316_Send_Command(command, 0, 0);
}


void SSD1316_Set_Cursor_Position(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;
	return;
}


uint8_t SSD1316_Update_Screen() {
	uint8_t col_data[2]= {0x00, 0x7F};
	_SSD1316_Send_Command(SSD1316_SET_COL_ADDR_VH_MODE, col_data, 2);	// Col position 0

	uint8_t page_data[2]= {0x00, 0x04};
	 _SSD1316_Send_Command(SSD1316_SET_PAGE_ADDR_VH_MODE, page_data, 2); // Row position 0


	 for (uint8_t page = 0; page < SSD1316_PAGE_COUNT; page++) {
		 for (uint8_t col = 0; col < SSD1316_COL_COUNT; col++) {
			 _SSD1316_Write_Byte(buffer[page][col]);
		 }
	 }

	 return 0;
}


//uint8_t SSD1316_Update_Screen_Portion(uint8_t* rect);


uint8_t SSD1316_Clear_Screen() {
	SSD1316_Clear_Buffer();
	return SSD1316_Update_Screen();
}


void SSD1316_Clear_Buffer() {
	memset(buffer, 0, sizeof(buffer));
	return;
}


uint8_t SSD1316_Write_Character(char value, bool append) {
	if (cursor_x > SSD1316_COL_COUNT - SMALL_CHARS_COLS_LENGTH) return 1;
	if (cursor_y > SSD1316_ROW_COUNT - SMALL_CHARS_COLS_HEIGHT) return 2;

	//uint8_t char_top = (uint8_t) value >> (cursor_y % 8);
	uint8_t data_top[SMALL_CHARS_COLS_LENGTH];
	uint8_t data_bottom[SMALL_CHARS_COLS_LENGTH];
	for (uint8_t i = 0; i < SMALL_CHARS_COLS_LENGTH; i++) {
		data_bottom[i] = FONTSMALL[value - 0x20][i] << (cursor_y % 8);
		data_top[i] = FONTSMALL[value - 0x20][i] >> (8 - (cursor_y % 8));
	}

	bool single_page = (cursor_y % 8) == 0;

	for (uint8_t i = 0; i < SMALL_CHARS_COLS_LENGTH; i++) {
		if (append) {
			if (!single_page)buffer[cursor_y / 8 + 1][cursor_x + i] |= data_top[i];
			buffer[(cursor_y / 8)][cursor_x + i] |= data_bottom[i];
		} else {
			if (!single_page) buffer[(cursor_y / 8) + 1][cursor_x + i] = data_top[i];
			buffer[cursor_y / 8][cursor_x + i] = data_bottom[i];
		}
	}

	return 0;
}

uint8_t SSD1316_Write_Tiny_Digit(char value, bool append) {
	if (cursor_x > SSD1316_COL_COUNT - TINY_CHARS_COLS_LENGTH) return 1;
	if (cursor_y > SSD1316_ROW_COUNT - TINY_CHARS_COLS_HEIGHT) return 2;

	//uint8_t char_top = (uint8_t) value >> (cursor_y % 8);
	uint8_t data_top[TINY_CHARS_COLS_LENGTH];
	uint8_t data_bottom[TINY_CHARS_COLS_LENGTH];
	for (uint8_t i = 0; i < TINY_CHARS_COLS_LENGTH; i++) {
		data_bottom[i] = FONTTINY[value - 0x30][i] << (cursor_y % 8);
		data_top[i] = FONTTINY[value - 0x30][i] >> (8 - (cursor_y % 8));
	}

	bool single_page = (cursor_y % 8) < 5;

	for (uint8_t i = 0; i < TINY_CHARS_COLS_LENGTH; i++) {
		if (append) {
			if (!single_page) buffer[cursor_y / 8 + 1][cursor_x + i] |= data_top[i];
			buffer[(cursor_y / 8)][cursor_x + i] |= data_bottom[i];
		} else {
			if (!single_page) buffer[(cursor_y / 8) + 1][cursor_x + i] = data_top[i];
			buffer[cursor_y / 8][cursor_x + i] = data_bottom[i];
		}
	}

	return 0;
}

uint8_t SSD1316_Write_Tiny_Number(char* string, bool wrap, bool append) {
	uint8_t orig_x = cursor_x;

	for (uint8_t len = 0; len < strlen(string); len++) {
		uint8_t status = SSD1316_Write_Tiny_Digit(string[len], append);
		if (status == 2) return 2;
		if (status == 1) {
			if (len == 0) return 1;
			if (!wrap) return 1;
			cursor_x = orig_x;
			cursor_x -= TINY_CHARS_COLS_LENGTH + TINY_CHARS_COLS_SPACING;
			cursor_y += TINY_CHARS_COLS_HEIGHT;
			len--;
		}

		cursor_x += TINY_CHARS_COLS_LENGTH + TINY_CHARS_COLS_SPACING;
	}

	return 0;
}

uint8_t SSD1316_Write_Large_Digit(char value, bool append) {
	if (cursor_x > SSD1316_COL_COUNT - LARGE_CHARS_COLS_LENGTH) return 1;
	if (cursor_y > SSD1316_ROW_COUNT - LARGE_CHARS_COLS_HEIGHT) return 2;

	bool single_page = (cursor_y % 8) == 0;

	for (uint8_t i = 0; i < LARGE_CHARS_COLS_LENGTH; i++) {
			if (!single_page)buffer[cursor_y / 8 + 2][cursor_x + i] |= FONTLARGE[value - 0x30][i + LARGE_CHARS_COLS_LENGTH] >> (8 - (cursor_y % 8));
			buffer[(cursor_y / 8 + 1)][cursor_x + i] |= (FONTLARGE[value - 0x30][i] >> (8 - (cursor_y % 8))) + (FONTLARGE[value - 0x30][i + LARGE_CHARS_COLS_LENGTH] << (cursor_y % 8));
			buffer[(cursor_y / 8)][cursor_x + i] |= FONTLARGE[value - 0x30][i] << (cursor_y % 8);
	}

	return 0;
}

uint8_t SSD1316_Write_Large_Number(char* string, bool wrap, bool append) {
	uint8_t orig_x = cursor_x;

	for (uint8_t len = 0; len < strlen(string); len++) {
		uint8_t status = SSD1316_Write_Large_Digit(string[len], append);
		if (status == 2) return 2;
		if (status == 1) {
			if (len == 0) return 1;
			if (!wrap) return 1;
			cursor_x = orig_x;
			cursor_x -= LARGE_CHARS_COLS_LENGTH + LARGE_CHARS_COLS_SPACING;
			cursor_y += LARGE_CHARS_COLS_HEIGHT;
			len--;
		}

		cursor_x += LARGE_CHARS_COLS_LENGTH + LARGE_CHARS_COLS_SPACING;
	}

	return 0;
}

uint8_t SSD1316_Write_String(char* string, bool wrap, bool append) {
	uint8_t orig_x = cursor_x;

	for (uint8_t len = 0; len < strlen(string); len++) {
		uint8_t status = SSD1316_Write_Character(string[len], append);
		if (status == 2) return 2;
		if (status == 1) {
			if (len == 0) return 1;
			if (!wrap) return 1;
			cursor_x = orig_x;
			cursor_x -= SMALL_CHARS_COLS_LENGTH + SMALL_CHARS_COLS_SPACING;
			cursor_y += SMALL_CHARS_COLS_HEIGHT;
			len--;
		}

		cursor_x += SMALL_CHARS_COLS_LENGTH + SMALL_CHARS_COLS_SPACING;
	}

	return 0;
}


uint8_t SSD1316_Write_Image(const unsigned char* array, uint8_t dim_x, uint8_t dim_y, bool append) {
	if (cursor_y % 8 != 0) return 3;
	if (cursor_x + dim_x > SSD1316_COL_COUNT) return 1;
	if (cursor_y + dim_y > SSD1316_ROW_COUNT) return 2;

	for (uint8_t row = 0; row < (dim_y / 8); row++) {
		for (uint8_t col = 0; col < dim_x; col++) {
			if (append) {
				buffer[row + cursor_y / 8][col + cursor_x] |= array[col + row * dim_x];
			} else {
				buffer[row + cursor_y / 8][col + cursor_x] = array[col + row * dim_x];
			}
		}
	}

	return 0;
}



