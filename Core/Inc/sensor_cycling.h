/*
 * sensor_cycling.h
 *
 *  Created on: Feb 3, 2026
 *      Author: cshapard26
 *
*/

#ifndef INC_SENSOR_CYCLING_H_
#define INC_SENSOR_CYCLING_H_

#include "usb_device.h"
#include "stm32l4xx_hal.h"
#include "../../Drivers/OPT3002/opt3002.h"
#include "../../Drivers/SGP41/sgp41.h"
#include "../../Drivers/SGP41/sensirion_gas_index_algorithm.h"
#include "../../Drivers/STCC4/stcc4.h"
#include "../../Drivers/BMI323/bmi323.h"


#define STCC4_REQUEST_DELAY_MS 550
#define OPT3002_FAST_REQUEST_DELAY_MS 110
#define OPT3002_SLOW_REQUEST_DELAY_MS 880
#define SGP41_REQUEST_DELAY_MS 55
#define BMI323_REQUEST_DELAY_MS 1000
#define ADC_REQUEST_DELAY_MS 47

#define STCC4_CONDITIONING_DELAY_MS 22000
#define SGP41_CONDITIONING_DELAY_MS 10000
#define BMI323_CONDITIONING_DELAY_MS 1000

#define SENSOR_HISTORY_COUNT	100

struct SensorSamplingRateS {
	uint32_t stcc4;
	uint32_t sgp41;
	uint32_t opt3002;
	uint32_t bmi323;
	uint32_t batt;
};


struct SensorReadings {
	uint32_t lux;
	uint16_t voc;
	uint16_t nox;
	uint16_t co2;
	uint16_t temp;
	uint16_t rh;
	uint16_t batt;
};

struct LastSensorReading {
	uint32_t stcc4;
	uint32_t sgp41;
	uint32_t opt3002;
	uint32_t bmi323;
	uint32_t batt;
};

struct SensorActive {	// 0: not active, 1: single measurement, 2: continuous measurements, 3: conditioning
	uint8_t stcc4;
	uint8_t sgp41;
	uint8_t opt3002;
	uint8_t bmi323;
	uint8_t batt;
};

struct SensorHistory {
	uint16_t* co2_history;
	uint16_t* voc_history;
	uint16_t* nox_history;
	uint16_t* tmp_history;
	uint16_t* rh_history;
	uint16_t* lux_history;
	uint16_t* bat_history;

	uint16_t* co2_history_index;
	uint16_t* voc_history_index;
	uint16_t* nox_history_index;
	uint16_t* tmp_history_index;
	uint16_t* rh_history_index;
	uint16_t* opt_history_index;
	uint16_t* bat_history_index;
};


void Set_ADC_And_Gas_Params(ADC_HandleTypeDef* hadc1, GasIndexAlgorithmParams* vocparams, GasIndexAlgorithmParams* noxparams);
uint32_t Refresh_Sensor_Readings(struct SensorReadings* readings);
void Set_Sensor_States(uint8_t states, uint8_t batt); //2 bits each with SensorActive settings and ordering
void Set_Cycling_Enabled(bool enable);
uint16_t Add_Measurements_To_History(struct SensorReadings* readings, struct SensorReadings* history);
struct SensorHistory* Get_Sensor_History();








#endif /* INC_SENSOR_CYCLING_H_ */
