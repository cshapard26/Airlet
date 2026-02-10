/*
 * opt3002.h
 *
 *  Created on: Feb 1, 2026
 *      Author: cshapard26
 */

#ifndef OPT3002_OPT3002_H_
#define OPT3002_OPT3002_H_

#include "stm32l4xx_hal.h"
#include <stdbool.h>

#define OPT3002_RESULT_REGISTER_ADDR	0x0
#define OPT3002_CONFIG_REGISTER_ADDR	0x1
#define OPT3002_I2C_ADDR				0x44


void OPT3002_Set_I2C(I2C_HandleTypeDef hi2c1);
uint8_t OPT3002_Request_Sensor_Reading(bool fast); // fast ? 110ms : 880ms
uint32_t OPT3002_Get_Sensor_Reading();
uint8_t OPT3002_Set_Config_Register(uint16_t data);
uint32_t OPT3002_Irradiance_To_Lux(unsigned int irradiance);


#endif /* OPT3002_OPT3002_H_ */
