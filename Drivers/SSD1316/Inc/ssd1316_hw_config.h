/*
 * ssd1316_hardware_config.h
 *
 *  Created on: Jan 28, 2026
 *      Author: cshapard26
 */

#ifndef SSD1316_HW_CONFIG_H_
#define SSSD1316_HW_CONFIG_H_

#include "ssd1316_core.h"

#define SSD1316_SET_DISP_START_LINE	0x40
#define SSD1316_SET_MPLEX_RATIO		0xA8
#define SSD1316_SET_COM_DIRECTION	0xC0
#define SSD1316_SET_DISP_OFFSET		0xD3
#define SSD1316_SET_SEG_PIN_CONFIG	0xDA

#define SSD1316_SET_CLOCK_DIVIDE	0xD5
#define SSD1316_SET_CHARGE_PERIOD	0xD9
#define SSD1316_SET_VCOMH_LEVEL		0xDB


uint8_t SSD1316_Set_Display_Start_Line(uint8_t offset);
uint8_t SSD1316_Set_Multiplex_Ratio(uint8_t rows);
uint8_t SSD1316_Set_COM_Scan_Direction(bool reversed);
uint8_t SSD1316_Set_Display_Offset(uint8_t offset);
uint8_t SSD1316_Set_SEG_Pin_Config(bool even_odd, bool left_right_remap);

uint8_t SSD1316_Set_Clock_Frequency(uint8_t divide_ratio, uint8_t osc_frequency);
uint8_t SSD1316_Set_Pre_Charge_Period(uint8_t phase1, uint8_t phase2);
uint8_t SSD1316_Set_VCOMH_Level(uint8_t level);


#endif /* SSD1306_INC_SSD1316_HW_CONFIG_H_ */
