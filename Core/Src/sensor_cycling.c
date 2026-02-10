/*
 * sensor_cycling.c
 *
 *  Created on: Feb 3, 2026
 *      Author: cshapard26
 */


#include "../Inc/sensor_cycling.h"
//#include <math.h>

bool cycling_enabled = true;

uint16_t co2_history_index = 0;
uint16_t co2_reading_history[SENSOR_HISTORY_COUNT];

uint16_t voc_history_index = 0;
static uint16_t voc_reading_history[SENSOR_HISTORY_COUNT];

uint16_t nox_history_index = 0;
static uint16_t nox_reading_history[SENSOR_HISTORY_COUNT];

uint16_t tmp_history_index = 0;
static uint16_t tmp_reading_history[SENSOR_HISTORY_COUNT];

uint16_t rh_history_index = 0;
static uint16_t rh_reading_history[SENSOR_HISTORY_COUNT];

uint16_t opt_history_index = 0;
static uint16_t opt_reading_history[SENSOR_HISTORY_COUNT];

uint16_t bat_history_index = 0;
static uint16_t bat_reading_history[SENSOR_HISTORY_COUNT];

struct SensorHistory sensor_val_history = {co2_reading_history, voc_reading_history, nox_reading_history, tmp_reading_history, rh_reading_history, opt_reading_history, bat_reading_history,
										   &co2_history_index, &voc_history_index, &nox_history_index, &tmp_history_index, &rh_history_index, &opt_history_index, &bat_history_index, };


struct SensorActive sensor_state = {0x00, 0x00, 0x00, 0x00, 0x00};
struct LastSensorReading measurement_tick = {0x00, 0x00, 0x00, 0x00, 0x00};
struct SensorSamplingRateS sampling_rate = {0x01, 0x01, 0x01, 0x01, 0x01};

ADC_HandleTypeDef* hadc;
GasIndexAlgorithmParams* voc_params;
GasIndexAlgorithmParams* nox_params;


void Set_ADC_And_Gas_Params(ADC_HandleTypeDef* hadc1, GasIndexAlgorithmParams* vocparams, GasIndexAlgorithmParams* noxparams) {
	hadc = hadc1;
	voc_params = vocparams;
	nox_params = noxparams;
}


uint32_t int_sqrt(uint32_t x) // GPT
{
    if (x == 0 || x == 1)
        return x;

    uint32_t res = 0;
    uint32_t bit = 1UL << 30;  // The highest power of four <= 2^32

    // "bit" starts at the highest power of four <= x
    while (bit > x)
        bit >>= 2;

    while (bit != 0)
    {
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }

    return res;
}


uint32_t Refresh_Sensor_Readings(struct SensorReadings* readings) {
	if (!cycling_enabled) return 0;

	uint32_t tick = HAL_GetTick();

	// STCC4
	if (sensor_state.stcc4 == 3) {
		STCC4_Perform_Conditioning();
		if ((tick - measurement_tick.stcc4) >= STCC4_CONDITIONING_DELAY_MS) {
			sensor_state.stcc4 = 1;
			STCC4_Request_Measurement();

			measurement_tick.stcc4 = tick;
		}
	}
	else if ((sensor_state.stcc4 > 0) && ((tick - measurement_tick.stcc4) >= 1000 * sampling_rate.stcc4)) {
		uint16_t stcc4data[3] = {0x00, 0x00, 0x00};
		STCC4_Read_Measurement(stcc4data);
		uint16_t temp_data = (stcc4data[1] * 9 / 5 + 24);
		readings->co2 = stcc4data[0];
		readings->temp = temp_data;
		readings->rh = stcc4data[2];
		co2_reading_history[co2_history_index] = stcc4data[0];
		tmp_reading_history[tmp_history_index] = temp_data;
		rh_reading_history[rh_history_index] = stcc4data[2];

		co2_history_index = (co2_history_index + 1) % SENSOR_HISTORY_COUNT;
		tmp_history_index = (tmp_history_index + 1) % SENSOR_HISTORY_COUNT;
		rh_history_index = (rh_history_index + 1) % SENSOR_HISTORY_COUNT;

		if (sensor_state.stcc4 == 1) {
			sensor_state.stcc4 = 0;
		} else {
			STCC4_Request_Measurement();
		}

		measurement_tick.stcc4 = tick;
	}

	// SGP41
	if (sensor_state.sgp41 == 3) {
		SGP41_Execute_Conditioning();
		if ((tick - measurement_tick.sgp41) >= SGP41_CONDITIONING_DELAY_MS) {
			sensor_state.sgp41 = 1;
			SGP41_Request_Raw_Data(readings->temp, readings->rh);


			measurement_tick.sgp41 = tick;
		}
	}
	else if ((sensor_state.sgp41 > 0) && ((tick - measurement_tick.sgp41) >= 1000 * sampling_rate.sgp41)) {
		uint16_t sgp41data[2] = {0x00, 0x00};
		SGP41_Read_Data(sgp41data);

		int32_t voc_index_value;
		GasIndexAlgorithm_process(voc_params, sgp41data[0], &voc_index_value);

		int32_t nox_index_value;
		GasIndexAlgorithm_process(nox_params, sgp41data[1], &nox_index_value);

		readings->voc = voc_index_value;
		readings->nox = nox_index_value;
		voc_reading_history[voc_history_index] = voc_index_value;
		nox_reading_history[nox_history_index] = nox_index_value;

		voc_history_index = (voc_history_index + 1) % SENSOR_HISTORY_COUNT;
		nox_history_index = (nox_history_index + 1) % SENSOR_HISTORY_COUNT;

		if (sensor_state.sgp41 == 1) {
			sensor_state.sgp41 = 0;
		} else {
			SGP41_Request_Raw_Data(readings->temp, readings->rh);
		}

		measurement_tick.sgp41 = tick;
	}

	// OPT3002
	if (sensor_state.opt3002 == 3) {
		if ((tick - measurement_tick.opt3002) >= OPT3002_SLOW_REQUEST_DELAY_MS) {
			OPT3002_Request_Sensor_Reading(false);

			measurement_tick.opt3002 = tick;
		}
	}
	else if ((sensor_state.opt3002 > 0) && ((tick - measurement_tick.opt3002) >= 1000 * sampling_rate.opt3002)) {
		readings->lux = int_sqrt(OPT3002_Get_Sensor_Reading());
		opt_reading_history[opt_history_index] = readings->lux;

		opt_history_index = (opt_history_index + 1) % SENSOR_HISTORY_COUNT;

		if (sensor_state.opt3002 == 1) {
			sensor_state.opt3002 = 0;
		} else {
			OPT3002_Request_Sensor_Reading(true);
		}

		measurement_tick.opt3002 = tick;
	}

	// BMI323
	/* not implemented */


	if (sensor_state.batt == 3) {
		if ((tick - measurement_tick.batt) >= ADC_REQUEST_DELAY_MS) {
			sensor_state.batt = 1;
			HAL_ADC_Start(hadc);

			measurement_tick.batt = tick;
		}
	}
	else if ((sensor_state.batt > 0) && ((tick - measurement_tick.batt) >= 1000 * sampling_rate.batt)) {

		uint16_t batt_reading = HAL_ADC_GetValue(hadc);
		HAL_ADC_Stop(hadc);

		uint8_t batt_percentage = (batt_reading - 1861) * 100 / 747;

		readings->batt = batt_percentage;



		bat_reading_history[bat_history_index] = batt_percentage;


		bat_history_index = (bat_history_index + 1) % SENSOR_HISTORY_COUNT;

		if (sensor_state.batt == 1) {
			sensor_state.batt = 0;
		} else {
			HAL_ADC_Start(hadc);
		}

		measurement_tick.batt = tick;
	}



	return tick;
}

void Set_Sensor_States(uint8_t states, uint8_t batt) {
	sensor_state.stcc4 = states >> 6;
	sensor_state.sgp41 = (states & 0x30) >> 4;
	sensor_state.opt3002 = (states & 0x0C) >> 2;
	sensor_state.bmi323 = states & 0x03;
	sensor_state.batt = batt;
}

void Set_Cycling_Enabled(bool enable) {
	cycling_enabled = enable;
}

struct SensorHistory* Get_Sensor_History() {
	return &sensor_val_history;
}
