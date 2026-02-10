/*
 * stcc4.h
 *
 *  Created on: Feb 1, 2026
 *      Author: cshapard26
 */

#ifndef STCC4_STCC4_H_
#define STCC4_STCC4_H_

#include "stm32l4xx_hal.h"
#include <stdbool.h>

#define STCC4_I2C_ADDR		0x64

#define STCC4_START_CONTINUOUS_MEASUREMENT	0x218B
#define STCC4_STOP_CONTINUOUS_MEASUREMENT	0x3F86
#define STCC4_READ_MEASUREMENT				0xEC05
#define STCC4_SET_RHT_COMPENSATION			0xE000
#define STCC4_SET_PRESSURE_COMPENSATION		0xE016
#define STCC4_MEASURE_SINGLE_SHOT			0x219D
#define STCC4_ENTER_SLEEP_MODE				0x3650
#define STCC4_EXIT_SLEEP_MODE				0x00
#define STCC4_PERFORM_CONDITIONING			0x29BC
#define STCC4_PERFORM_SOFT_RESET			0x06
#define STCC4_PERFORM_FACTORY_RESET			0x3632
#define STCC4_PERFORM_SELF_TEST				0x278C
#define STCC4_ENABLE_TESTING_MODE			0x3FBC
#define STCC4_DISABLE_TESTING_MODE			0x3F3D
#define STCC4_PERFORM_FORCED_RECALIBRATION	0x362F
#define STCC4_GET_PRODUCT_ID				0x365B




void STCC4_Set_I2C(I2C_HandleTypeDef hi2c1);

uint8_t STCC4_Request_Measurement(); // 500ms
uint8_t STCC4_Read_Measurement(uint16_t co2rht[3]);
uint8_t STCC4_Set_Enabled(bool enabled);
uint8_t STCC4_Perform_Conditioning(); // 22s






#endif /* STCC4_STCC4_H_ */
