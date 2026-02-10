/*
 * ssd1316_adv_graphics.h
 *
 *  Created on: Jan 28, 2026
 *      Author: cshapard26
 */

#ifndef SSD1316_ADV_GRAPHICS_H_
#define SSD1316_ADV_GRAPHICS_H_

#include "ssd1316_core.h"

#define SSD1316_CONT_HORZ_SCROLL_SETUP			0x26
#define SSD1316_CONT_VERT_AND_HORZ_SCROLL_SETUP	0x29
#define SSD1316_SET_SCROLL_ENABLE				0x2E
#define SSD1316_SET_VERT_SCROLL_AREA			0xA3
#define SSD1316_CONTENT_SCROLL_SETUP			0x2C

#define SSD1316_SET_FADE_AND_BLINKING	0x23
#define SSD1316_SET_ZOOM_IN				0xD6



uint8_t SSD1316_Continuous_Horizontal_Scroll_Setup(bool direction, uint8_t start_page_addr, uint8_t end_page_addr, uint8_t start_col_addr, uint8_t end_col_addr, uint8_t interval);
uint8_t SSD1316_Continuous_Horizontal_And_Vertical_Scroll_Setup(bool horz_direction, bool horz_scroll, uint8_t vert_scroll_offset, uint8_t start_page_addr, uint8_t end_page_addr, uint8_t start_col_addr, uint8_t end_col_addr, uint8_t interval);
uint8_t SSD1316_Set_Scroll_Enable(bool enabled);
uint8_t SSD1316_Set_Vertical_Scroll_Area(uint8_t fixed_area, uint8_t num_scroll_rows);
uint8_t SSD1316_Content_Scroll_Setup(bool direction, uint8_t start_page_addr, uint8_t end_page_addr, uint8_t start_col_addr, uint8_t end_col_addr);


uint8_t SSD1316_Set_Fade_and_Blink(bool fade_in, bool fade_out, uint8_t interval);
uint8_t SSD1316_Set_Zoom_Mode(bool enabled);




#endif /* SSD1306_INC_SSD1316_ADV_GRAPHICS_H_ */
