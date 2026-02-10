/*
 * ssd1316_adv_graphics.c
 *
 *  Created on: Jan 31, 2026
 *      Author: cshapard26
 */

#include "../Inc/ssd1316_adv_graphics.h"


uint8_t SSD1316_Continuous_Horizontal_Scroll_Setup(bool direction, uint8_t start_page_addr, uint8_t end_page_addr, uint8_t start_col_addr, uint8_t end_col_addr, uint8_t interval) {
	uint8_t command =  SSD1316_CONT_HORZ_SCROLL_SETUP | direction;
	uint8_t data[] = {0x00, start_page_addr, interval, end_page_addr, 0x00, start_col_addr, end_col_addr};
	return _SSD1316_Send_Command(command, data, 7);

}


uint8_t SSD1316_Continuous_Horizontal_And_Vertical_Scroll_Setup(bool horz_direction, bool horz_scroll, uint8_t vert_scroll_offset, uint8_t start_page_addr, uint8_t end_page_addr, uint8_t start_col_addr, uint8_t end_col_addr, uint8_t interval) {
	uint8_t command =  SSD1316_CONT_VERT_AND_HORZ_SCROLL_SETUP + horz_direction;
	uint8_t data[] = {horz_scroll, start_page_addr, interval, end_page_addr, vert_scroll_offset, start_col_addr, end_col_addr};
	return _SSD1316_Send_Command(command, data, 7);
}


uint8_t SSD1316_Set_Scroll_Enable(bool enabled) {
	uint8_t command =  SSD1316_SET_SCROLL_ENABLE | enabled;
	return _SSD1316_Send_Command(command, 0, 0);
}


uint8_t SSD1316_Set_Vertical_Scroll_Area(uint8_t fixed_area, uint8_t num_scroll_rows) {
	uint8_t command = SSD1316_SET_VERT_SCROLL_AREA;
	uint8_t data[] = {fixed_area, num_scroll_rows};
	return _SSD1316_Send_Command(command, data, 2);
}


uint8_t SSD1316_Content_Scroll_Setup(bool direction, uint8_t start_page_addr, uint8_t end_page_addr, uint8_t start_col_addr, uint8_t end_col_addr) {
	uint8_t command =  SSD1316_CONTENT_SCROLL_SETUP | direction;
	uint8_t data[] = {0x00, start_page_addr, 0x01, end_page_addr, 0x00, start_col_addr, end_col_addr};
	return _SSD1316_Send_Command(command, data, 7);
}



uint8_t SSD1316_Set_Fade_and_Blink(bool fade_in, bool fade_out, uint8_t interval) {
	uint8_t command =  SSD1316_SET_FADE_AND_BLINKING;
	uint8_t data = (fade_in << 4) | (fade_out << 5) | interval;
	return _SSD1316_Send_Command(command, &data, 1);
}


uint8_t SSD1316_Set_Zoom_Mode(bool enabled) {
	uint8_t command =  SSD1316_SET_ZOOM_IN;
	uint8_t data = enabled;
	return _SSD1316_Send_Command(command, &data, 1);
}

