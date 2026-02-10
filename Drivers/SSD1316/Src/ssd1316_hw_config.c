/*
 * ssd1316_hw_config.c
 *
 *  Created on: Jan 31, 2026
 *      Author: cshapard26
 */

#include "../Inc/ssd1316_hw_config.h"

uint8_t SSD1316_Set_Display_Start_Line(uint8_t offset) {
	uint8_t command =  SSD1316_SET_DISP_START_LINE | offset;
	return _SSD1316_Send_Command(command, 0, 0);
}


uint8_t SSD1316_Set_Multiplex_Ratio(uint8_t rows) {
	uint8_t command =  SSD1316_SET_MPLEX_RATIO;
	uint8_t data = rows;
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t SSD1316_Set_COM_Scan_Direction(bool reversed) {
	uint8_t command =  SSD1316_SET_COM_DIRECTION | reversed << 3;
	return _SSD1316_Send_Command(command, 0, 0);
}


uint8_t SSD1316_Set_Display_Offset(uint8_t offset) {
	uint8_t command =  SSD1316_SET_DISP_OFFSET;
	uint8_t data = offset;
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t SSD1316_Set_SEG_Pin_Config(bool even_odd, bool left_right_remap) {
	uint8_t command =  SSD1316_SET_SEG_PIN_CONFIG;
	uint8_t data = 0x00 | (even_odd << 4) | (left_right_remap << 5);
	return _SSD1316_Send_Command(command, &data, 1);
}



uint8_t SSD1316_Set_Clock_Frequency(uint8_t divide_ratio, uint8_t osc_frequency) {
	uint8_t command =  SSD1316_SET_CLOCK_DIVIDE;
	uint8_t data = divide_ratio | (osc_frequency << 4);
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t SSD1316_Set_Pre_Charge_Period(uint8_t phase1, uint8_t phase2) {
	uint8_t command =  SSD1316_SET_CHARGE_PERIOD;
	uint8_t data = phase1 | (phase2 << 4);
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t SSD1316_Set_VCOMH_Level(uint8_t level) {
	uint8_t command =  SSD1316_SET_VCOMH_LEVEL;
	uint8_t data = level << 4;
	return _SSD1316_Send_Command(command, &data, 1);
}


