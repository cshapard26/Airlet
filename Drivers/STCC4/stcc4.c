/*
 * stcc4.c
 *
 *  Created on: Feb 1, 2026
 *      Author: cshapard26
 */


#include "stcc4.h"

I2C_HandleTypeDef stcchi2c;


void STCC4_Set_I2C(I2C_HandleTypeDef hi2c1) {
	stcchi2c = hi2c1;
}


uint8_t STCC4_Request_Measurement() {
	uint8_t reg[2] = {STCC4_MEASURE_SINGLE_SHOT >> 8, STCC4_MEASURE_SINGLE_SHOT & 0xFF};
	return HAL_I2C_Master_Transmit(&stcchi2c, STCC4_I2C_ADDR << 1, reg, 2, 10);
}


uint8_t STCC4_Read_Measurement(uint16_t co2rht[3]) {
	uint8_t reg[2] = {STCC4_READ_MEASUREMENT >> 8, STCC4_READ_MEASUREMENT & 0xFF};
	uint8_t status = HAL_I2C_Master_Transmit(&stcchi2c, STCC4_I2C_ADDR << 1, reg, 2, 10);

	uint8_t received_data[12];
	status = HAL_I2C_Master_Receive(&stcchi2c, (STCC4_I2C_ADDR << 1) | 0x1, received_data, 12, 10);

	co2rht[0] = ((uint16_t)received_data[0] << 8) | (uint16_t)received_data[1];
	uint16_t raw_temp = ((uint16_t)received_data[3] << 8) | (uint16_t)received_data[4];
	co2rht[1] = (175 * raw_temp / 65535) - 45;
	uint16_t raw_rh = ((uint16_t)received_data[6] << 8) | (uint16_t)received_data[7];
	co2rht[2] = (125 * raw_rh / 65535) - 6;

	return status;
}


uint8_t STCC4_Set_Enabled(bool enabled) {
	if (enabled) {
		uint8_t reg = STCC4_EXIT_SLEEP_MODE;
		return HAL_I2C_Master_Transmit(&stcchi2c, STCC4_I2C_ADDR << 1, &reg, 1, 10);
	} else {
		uint8_t reg[2] = {STCC4_ENTER_SLEEP_MODE >> 8, STCC4_ENTER_SLEEP_MODE & 0xFF};
		return HAL_I2C_Master_Transmit(&stcchi2c, STCC4_I2C_ADDR << 1, reg, 2, 10);
	}
}


uint8_t STCC4_Perform_Conditioning() {
	uint8_t reg[2] = {STCC4_PERFORM_CONDITIONING >> 8, STCC4_PERFORM_CONDITIONING & 0xFF};
	return HAL_I2C_Master_Transmit(&stcchi2c, STCC4_I2C_ADDR << 1, reg, 2, 10);
}





