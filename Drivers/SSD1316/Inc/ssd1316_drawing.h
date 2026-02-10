/*
 * ssd1316_drawing.h
 *
 *  Created on: Jan 28, 2026
 *      Author: cshapard26
 */

#ifndef SSD1316_DRAWING_H_
#define SSD1316_DRAWING_H_

#include "ssd1316_core.h"


uint8_t SSD1316_Draw_Point(uint8_t x, uint8_t y, bool color);
uint8_t SSD1316_Draw_Line(uint8_t* points, bool color);

uint8_t SSD1316_Draw_X_Gon(uint8_t* dimensions, uint8_t edges, bool color);

uint8_t SSD1316_Draw_Rectangle(uint8_t* dimensions, bool color);
uint8_t SSD1316_Draw_Triangle(uint8_t* dimensions, bool color);
uint8_t SSD1316_Draw_Circle(uint8_t* dimensions, bool color);
uint8_t SSD1316_Draw_Filled_Rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool color);


#endif /* SSD1306_INC_SSD1316_DRAWING_H_ */
