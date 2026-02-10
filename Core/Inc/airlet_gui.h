/*
 * airlet_gui.h
 *
 *  Created on: Feb 4, 2026
 *      Author: cooper
 */

#ifndef INC_AIRLET_GUI_H_
#define INC_AIRLET_GUI_H_

#include <stdint.h>
#include "sensor_cycling.h"
#include "images.h"

enum gui_state {
	OFF,

	HOME,

	CO2_DISPLAY,
		CO2_GRAPH,
	VOC_DISPLAY,
		VOC_GRAPH,
	NOX_DISPLAY,
		NOX_GRAPH,
	OPT_DISPLAY,
		OPT_GRAPH,
	TMP_DISPLAY,
		TMP_GRAPH,
	RH_DISPLAY,
		RH_GRAPH,
	BAT_DISPLAY,
		BAT_GRAPH,

	SETTINGS_BASE,
		SETTINGS_CONTRAST,
		SETTINGS_SCREEN_SLEEP,
		SETTINGS_POLL_RATE,
		SETTINGS_GRAPH_TIME,
		SETTINGS_HOME_SELECT,
		SETTINGS_UNIT_SELECT,
		SETTINGS_LIGHTDARK_SELECT,

	CHARGE_MODE,
	CALIBRATION_MODE,
	USB_MODE,
};

enum gui_retval {
	GUI_SUCCESS,
	GUI_PROCESSING,
	GUI_ERROR
};

enum button_state {
	BUTTON_OFF,
	BUTTON_PRESS,
	BUTTON_HOLD
};

enum gui_state Get_Screen_State();
enum gui_retval Change_State(enum gui_state new_state);
uint8_t Get_Setting_Value(enum gui_state setting);
void Update_Settings(enum gui_state setting, uint8_t state);

enum gui_retval Display_Sensor_Value(uint16_t sensor_val, const unsigned char* label_image);
enum gui_retval Display_Sensor_Graph(uint16_t* sensor_val_history, char* label_string, uint16_t scale_max, uint16_t* reading_index);
enum gui_retval Display_Home_Page(struct SensorReadings* values);
enum gui_retval Display_Charge_Page(uint8_t battery_percentage);
enum gui_retval Display_Calibration_Page();
enum gui_retval Display_Settings_Page(char** options, uint8_t* selection);
enum gui_retval Display_Transistion(enum gui_state old_state, enum gui_state new_state);
enum gui_retval Display_Off();

enum gui_retval Update_Display(struct SensorReadings* values, struct SensorHistory* sensor_val_history);
enum gui_retval Check_Button_Navigation(enum button_state button1, enum button_state button2);





#endif /* INC_AIRLET_GUI_H_ */
