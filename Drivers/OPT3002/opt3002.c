/*
 * opt3002.c
 *
 *  Created on: Feb 1, 2026
 *      Author: cshapard26
 */

#include "opt3002.h"

I2C_HandleTypeDef opthi2c;

void OPT3002_Set_I2C(I2C_HandleTypeDef hi2c1) {
	opthi2c = hi2c1;
}


uint8_t OPT3002_Request_Sensor_Reading(bool fast) {
	uint16_t write_data = 0xC210 | ((!(uint16_t)fast) << 11);
	OPT3002_Set_Config_Register(write_data);

	uint8_t reg = OPT3002_RESULT_REGISTER_ADDR;
	return HAL_I2C_Master_Transmit(&opthi2c, OPT3002_I2C_ADDR << 1, &reg, 1, 10);
}


uint32_t OPT3002_Get_Sensor_Reading() {
	uint8_t recieved_data[2] = {0x00, 0x00};
	HAL_I2C_Master_Receive(&opthi2c, (OPT3002_I2C_ADDR << 1) | 0x1, recieved_data, 2, 10);

	uint8_t optexp = (recieved_data[0] & 0xF0) >> 4;
	uint16_t optval = ((recieved_data[0] & 0x0F) << 8) | recieved_data[1];
	uint32_t total = (0x1 << optexp) * 6 * optval / 5;
	return OPT3002_Irradiance_To_Lux(total); // nW/cm^2
}

uint32_t OPT3002_Irradiance_To_Lux(unsigned int irradiance) {
	// convert to W/m^2 (*10^-5), then multiply by average luminous efficacy factor (~100 indoors)
	return (irradiance / 68);
}


uint8_t OPT3002_Set_Config_Register(uint16_t data) {
	uint8_t write_data[3] = {OPT3002_CONFIG_REGISTER_ADDR, data >> 8, data & 0xFF};
	return HAL_I2C_Master_Transmit(&opthi2c, OPT3002_I2C_ADDR << 1, write_data, 3, 10);
}


