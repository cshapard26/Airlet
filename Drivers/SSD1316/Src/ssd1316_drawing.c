/*
 * ssd1316_drawing.c
 *
 *  Created on: Jan 30, 2026
 *      Author: cshapard26
 */

#include "../Inc/ssd1316_drawing.h"


uint8_t SSD1316_Draw_Point(uint8_t x, uint8_t y, bool color) {
	if (x > SSD1316_COL_COUNT - 1 || y / 8 > SSD1316_PAGE_COUNT - 1) return 1;
	if (color == 1) {
		uint8_t test =  0x1 << (y % 8);
		buffer[y / 8][x] |= test;
	}
	if (color == 0) buffer[y / 8][x] &= 0xFF ^ (0x0 << (y % 8));

	return 0;
}
uint8_t SSD1316_Draw_Line(uint8_t* points, bool color) {
	uint8_t x1 = points[0];
	uint8_t y1 = points[1];
	uint8_t x2 = points[2];
	uint8_t y2 = points[3];

	if (x1 > x2) {
		x1 = x1 ^ x2;
		x2 = x1 ^ x2;
		x1 = x1 ^ x2;
		y1 = y1 ^ y2;
		y2 = y1 ^ y2;
		y1 = y1 ^ y2;
	}

	if (x2 > SSD1316_COL_COUNT || y1 / 8 > SSD1316_PAGE_COUNT || y2 / 8 > SSD1316_PAGE_COUNT) return 1;

	uint8_t x_steps = x2 - x1 + 1;
	int8_t y_sign = (y2 - y1 > 0) - (y2 - y1 < 0);
	uint8_t y_steps = y_sign > 0 ? y2 - y1 + 1: y1 - y2 + 1;
	uint8_t x_curr = x1;
	uint8_t y_curr = y1;
	uint8_t index = 1;

	if (x_steps > y_steps) {
		float step_size = ((float) x_steps / (float) y_steps);
		float step = step_size;
		while (x_curr < x2) {
			SSD1316_Draw_Point(x_curr, y_curr, color);
			x_curr++;
			if (index > step) {
				y_curr += y_sign;
				step += step_size;
			}
			index++;
			//SSD1316_Update_Screen();
			//HAL_Delay(10);
		}
	} else if (x_steps < y_steps) {
		float step_size = ((float) y_steps / (float) x_steps);
		float step = step_size;
		while ((y_sign > 0 ? y_curr < y2 : y_curr > y2)) {
			SSD1316_Draw_Point(x_curr, y_curr, color);
			y_curr += y_sign;
			if (index > step) {
				x_curr++;
				step += step_size;
			}
			index++;
		}
	} else {
		while (x_curr <= x2) {
			SSD1316_Draw_Point(x_curr, y_curr, color);
			x_curr++;
			y_curr += y_sign;

			index++;
		}

	}

	return 0;

}

uint8_t SSD1316_Draw_X_Gon(uint8_t* dimensions, uint8_t edges, bool color) {
	uint8_t end_dim[4] = {dimensions[2 * edges - 2], dimensions[2 * edges - 1], dimensions[0], dimensions[1]};
	for (uint8_t index = 0; index < edges - 1; index++) {
		SSD1316_Draw_Line(dimensions + 2 * index, color);
	}
	SSD1316_Draw_Line(end_dim, color);

	return 0;
}

uint8_t SSD1316_Draw_Rectangle(uint8_t* dimensions, bool color) {
	return SSD1316_Draw_X_Gon(dimensions, 4, color);

}

uint8_t SSD1316_Draw_Filled_Rectangle( //GPT
    uint8_t x1, uint8_t y1,
    uint8_t x2, uint8_t y2,
    bool color)
{
    /* Normalize coordinates */
    if (x1 > x2) {
        uint8_t t = x1; x1 = x2; x2 = t;
    }
    if (y1 > y2) {
        uint8_t t = y1; y1 = y2; y2 = t;
    }

    /* Bounds check */
    if (x2 >= SSD1316_COL_COUNT || y2 >= SSD1316_PAGE_COUNT * 8)
        return 1;

    for (uint8_t y = y1; y <= y2; y++) {
        for (uint8_t x = x1; x <= x2; x++) {
            SSD1316_Draw_Point(x, y, color);
        }
    }

    return 0;
}


uint8_t SSD1316_Draw_Triangle(uint8_t* dimensions, bool color) {
	return SSD1316_Draw_X_Gon(dimensions, 3, color);

}
uint8_t SSD1316_Draw_Circle(uint8_t* dimensions, bool color) {
	// Uses Bresenham Circle Algorithm
	uint8_t cx = dimensions[0];
	uint8_t cy = dimensions[1];
	uint8_t r = dimensions[2];


	int8_t x = r;
	int8_t y = 0;
	int8_t d = 1 - r;

	while (x >= y) {
		SSD1316_Draw_Point(cx + x, cy + y, color);
		SSD1316_Draw_Point(cx + y, cy + x, color);
		SSD1316_Draw_Point(cx - y, cy + x, color);
		SSD1316_Draw_Point(cx - x, cy + y, color);
		SSD1316_Draw_Point(cx - x, cy - y, color);
		SSD1316_Draw_Point(cx - y, cy - x, color);
		SSD1316_Draw_Point(cx + y, cy - x, color);
		SSD1316_Draw_Point(cx + x, cy - y, color);

		y++;

		if (d <= 0) {
			d += 2 * y + 1;
		} else {
			x--;
			d += 2 * (y - x) + 1;
		}

	}

	return 0;

}
