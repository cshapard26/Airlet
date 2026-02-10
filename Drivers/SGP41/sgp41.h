/*
 * sgp41.h
 *
 *  Created on: Feb 1, 2026
 *      Author: cshapard26
 */

#ifndef SGP41_SGP41_H_
#define SGP41_SGP41_H_

#include "stm32l4xx_hal.h"
#include <stdbool.h>

#define SGP41_I2C_ADDR		0x59


void SGP41_Set_I2C(I2C_HandleTypeDef hi2c1);

uint8_t SGP41_CalcCrc(uint8_t data[2]);
uint8_t SGP41_Execute_Conditioning(); // 50ms
uint8_t SGP41_Request_Raw_Data(uint8_t temp, uint8_t rh); // 50ms
uint8_t SGP41_Read_Data(uint16_t voc_nox_results[2]); // [VOC, NOx]
uint8_t SGP41_Activate_Sleep_Mode();


#endif /* SGP41_SGP41_H_ */
