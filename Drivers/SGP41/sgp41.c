/*
 * sgp41.c
 *
 *  Created on: Feb 1, 2026
 *      Author: cshapard26
 */

#include "sgp41.h"

I2C_HandleTypeDef sgphi2c;

void SGP41_Set_I2C(I2C_HandleTypeDef hi2c1) {
	sgphi2c = hi2c1;
}

uint8_t SGP41_CalcCrc(uint8_t data[2]) { // Function from datasheet
	uint8_t crc = 0xFF;
	for(int i = 0; i < 2; i++) {
		crc ^= data[i];
		for(uint8_t bit = 8; bit > 0; --bit) {
			if(crc & 0x80) {
				crc = (crc << 1) ^ 0x31u;
			} else {
				crc = (crc << 1);
			}
		}
	}
	return crc;
}


uint8_t SGP41_Execute_Conditioning() {
	uint8_t write_data[8] = {0x26, 0x12, 0x80, 0x00, 0xA2, 0x66, 0x66, 0x93};
	uint8_t status = HAL_I2C_Master_Transmit(&sgphi2c, SGP41_I2C_ADDR << 1, write_data, 8, 10);
	return status;
}

uint8_t SGP41_Request_Raw_Data(uint8_t temp, uint8_t rh) {
	if(!temp) temp = 50;
	if(!rh) rh = 25;
	uint16_t temp_data = ((uint16_t)temp + 45) * 65535 / 175;
	uint16_t rh_data = ((uint16_t)rh) * 65535 / 100;

	uint8_t trh_data[4] = {(uint8_t)(rh_data >> 8), (uint8_t)(rh_data & 0xFF), (uint8_t)(temp_data >> 8), (uint8_t)(temp_data & 0xFF)};


	uint8_t write_data[8] = {0x26, 0x19, trh_data[0], trh_data[1], SGP41_CalcCrc(trh_data), trh_data[2], trh_data[3], SGP41_CalcCrc(trh_data + 2)};
	uint8_t status = HAL_I2C_Master_Transmit(&sgphi2c, SGP41_I2C_ADDR << 1, write_data, 8, 10);
	return status;
}

uint8_t SGP41_Read_Data(uint16_t voc_nox_results[2]) {
	uint8_t reg = 0;
	uint8_t results[6];
	HAL_I2C_Master_Transmit(&sgphi2c, SGP41_I2C_ADDR << 1, &reg, 0, 10);
	uint8_t status = HAL_I2C_Master_Receive(&sgphi2c, (SGP41_I2C_ADDR << 1) | 0x1, results, 6, 10);

	voc_nox_results[0] = ((uint16_t)results[0]) << 8 | results[1];
	voc_nox_results[1] = ((uint16_t)results[3]) << 8 | results[4];
	return status;
}

uint8_t SGP41_Activate_Sleep_Mode() {
	uint8_t write_data[2] = {0x36, 0x15};
	uint8_t status = HAL_I2C_Master_Transmit(&sgphi2c, SGP41_I2C_ADDR << 1, write_data, 2, 10);
	return status;
}


