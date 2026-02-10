/*
 * airlet_gui.c
 *
 *  Created on: Feb 4, 2026
 *      Author: cshapard26
 */


/* TODO
 * 	--Sleep Mode--
 * 	Deep Sleep Mode (MCU off)
 * 	--Calibration--
 * 	Median Sampling Intervals
 * 	--Revamp Good/Caution/Poor Calculation/symbols--
 * 	oF to oC conversion
 *
 * 	--Settings Menu--
 *
 *	Make Case
 *
 *	More Animations/Transitions/Icons?
 *	Button Debounce/Hold?
 *
 * 	USB Data Export
 * 	Data Perpetuity/Power Loss
 *
 * 	Flash Optimzation
 *
 *	Testing
 *
 */





#include "../Inc/airlet_gui.h"
#include "../../Drivers/SSD1316/Inc/ssd1316_drawing.h"

static void u16_to_str(uint16_t v, char *buf)
{
    char tmp[6];
    int i = 0, j = 0;

    do {
        tmp[i++] = '0' + (v % 10);
        v /= 10;
    } while (v);

    while (i--)
        buf[j++] = tmp[i];

    buf[j] = '\0';
}



uint8_t contrast = 3;
uint8_t screen_sleep = 1;
uint8_t poll_rate = 3;
uint8_t graph_time = 2;
uint8_t home_select = 1;
uint8_t unit_select = 1;
uint8_t lightdark = 1;


enum gui_state screen_state = HOME;
uint32_t screen_ticks = 0;
uint32_t screen_off_ticks = 420; // 14 ticks / second



uint8_t Get_Setting_Value(enum gui_state setting) {
	switch (setting) {
		case SETTINGS_CONTRAST:
			return contrast;
		case SETTINGS_SCREEN_SLEEP:
			return screen_sleep;
		case SETTINGS_POLL_RATE:
			return poll_rate;
		case SETTINGS_GRAPH_TIME:
			return graph_time;
		case SETTINGS_HOME_SELECT:
			return home_select;
		case SETTINGS_UNIT_SELECT:
			return unit_select;
		case SETTINGS_LIGHTDARK_SELECT:
			return lightdark;
		default:
			return 0;

	}
}

void Update_Settings(enum gui_state setting, uint8_t state) {
	switch (setting) {
		case SETTINGS_CONTRAST:
			if (state != contrast) {
				contrast = state;
				SSD1316_Set_Contrast(contrast == 1 ? 1 : (contrast == 2 ? 64 : (contrast == 3 ? 128 : 255)));
			}
			return;

		case SETTINGS_SCREEN_SLEEP:
			if (state != screen_sleep) {
				screen_sleep = state;
				screen_off_ticks = (screen_sleep == 1 ? 420 : (screen_sleep == 2 ? 840 : (screen_sleep == 3 ? 4200 : 0xFFFFFFFF)));
			}
			return;

		case SETTINGS_POLL_RATE:
			if (state != poll_rate) {
				poll_rate = state;

			}
			return;

		case SETTINGS_GRAPH_TIME:
			if (state != graph_time) {
				if (state == 4) state = 1;
				graph_time = state;

			}
			return;

		case SETTINGS_HOME_SELECT:
			if (state != home_select) {
				home_select = state;
			}
			return;

		case SETTINGS_UNIT_SELECT:
			if (state != unit_select) {
				if (state == 3) state = 1;
				unit_select = state;
			}
			return;

		case SETTINGS_LIGHTDARK_SELECT:
			if (state != lightdark) {
				if (state == 3) state = 1;
				lightdark = state;
				SSD1316_Set_Screen_Invert(lightdark - 1);
			}
			return;

		default:
			return;

	}
}



enum gui_state Get_Screen_State() {
	return screen_state;
}

enum gui_retval Change_State(enum gui_state new_state) {
	if (screen_state == OFF && new_state != OFF) SSD1316_Set_Screen_Enable(true);
	enum gui_retval transition = Display_Transistion(screen_state, new_state);

	if (screen_state == new_state && (screen_state <= SETTINGS_LIGHTDARK_SELECT && screen_state >= SETTINGS_CONTRAST)) {
		Update_Settings(screen_state, (Get_Setting_Value(screen_state) % 4) + 1);
	}

	screen_state = new_state;

	return transition;
}

enum gui_retval Display_Sensor_Value(uint16_t sensor_val, const unsigned char* label_image) {
	char str[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	u16_to_str(sensor_val, str);
	uint8_t str_len = strlen(str);

	SSD1316_Set_Cursor_Position(64 + 5 * (5 - str_len), 8);
	SSD1316_Write_Large_Number(str, false, false);

	SSD1316_Set_Cursor_Position(18 + 5 * (5 - str_len), 0);
	SSD1316_Write_Image(label_image, 32, 32, false);
	SSD1316_Set_Cursor_Position(50 + 5 * (5 - str_len), 8);
	SSD1316_Write_Image(colon, 8, 16, true);

	SSD1316_Set_Cursor_Position(108 - 3*(5 - str_len), 16);
	char str2[4] = "\0\0\0";
	switch (screen_state) {
		case CO2_DISPLAY:
			sprintf(str2, "ppm");
			break;
		case VOC_DISPLAY:
			sprintf(str2, "idx");
			break;
		case NOX_DISPLAY:
			sprintf(str2, "idx");
			break;
		case TMP_DISPLAY:
			if (unit_select == 1) {
				sprintf(str2, "oF");
			} else {
				sprintf(str2, "oC");
			}
			break;
		case RH_DISPLAY:
			sprintf(str2, "%%");
			break;
		case OPT_DISPLAY:
			sprintf(str2, "lux");
			break;
		case BAT_DISPLAY:
			sprintf(str2, "%%");
			break;
		default:
			break;

	};

	if (screen_state == OPT_DISPLAY) {
		SSD1316_Write_Image(sqrtlux, 19, 8, true);
	} else {
		SSD1316_Write_String(str2, false, false);
	}



	return GUI_SUCCESS;
}


static uint8_t normalize_reading(uint16_t reading, uint16_t max_reading, uint8_t steps) {
	uint8_t data = reading * steps / max_reading;
	return (data > steps) ? steps : data;

}




enum gui_retval Display_Sensor_Graph(uint16_t* sensor_val_history, char* label_string, uint16_t scale_max, uint16_t* reading_index) {
	SSD1316_Set_Cursor_Position(1, 25);
	SSD1316_Write_String(label_string, false, true);


	static uint8_t line_dims[4] = {22, 1, 22, 29};
	SSD1316_Draw_Line(line_dims, 1);
	static uint8_t line_dims2[4] = {18, 25, 125, 25};
	SSD1316_Draw_Line(line_dims2, 1);

	// y-axis
	SSD1316_Set_Cursor_Position(17, 20);
	SSD1316_Write_Tiny_Digit('0', true);

	static char str[6] = "\0\0\0\0\0";
	u16_to_str(scale_max, str);
	SSD1316_Set_Cursor_Position(21 - 4*strlen(str), 1);
	SSD1316_Write_Tiny_Number(str, false, true);

	static char str1[6] = "\0\0\0\0\0";
	u16_to_str(scale_max / 2, str1);
	SSD1316_Set_Cursor_Position(21 - 4*strlen(str1), 11);
	SSD1316_Write_Tiny_Number(str1, false, true);


	// x-axis
	static char str5[6] = ";4:"; // -4h
	SSD1316_Set_Cursor_Position(23, 27);
	SSD1316_Write_Tiny_Number(str5, false, true);

	static char str2[6] = ";3:"; // -3h
	SSD1316_Set_Cursor_Position(47, 27);
	SSD1316_Write_Tiny_Number(str2, false, true);

	static char str3[6] = ";2:"; // -2h
	SSD1316_Set_Cursor_Position(71, 27);
	SSD1316_Write_Tiny_Number(str3, false, true);

	static char str4[6] = ";1:"; // -1h
	SSD1316_Set_Cursor_Position(95, 27);
	SSD1316_Write_Tiny_Number(str4, false, true);

	SSD1316_Set_Cursor_Position(122, 27);
	SSD1316_Write_Tiny_Digit('0', true);


	//draw the graph
	static const uint8_t x_resolution = 124 - 27 + 1; // Graph area
	static const uint8_t y_resolution = 21 - 1 + 2;	// Graph area
	uint8_t line_dimensions[4] = {0, 0, 0, 0};

	uint8_t normalized_data = normalize_reading(sensor_val_history[((*reading_index - 1) % 100 + 100) % 100 /* SENSOR_HISTORY_COUNT*/], scale_max, y_resolution);
	line_dimensions[2] = 124 - 2;
	line_dimensions[3] = y_resolution - normalized_data + 1;

	for (uint16_t i = 1; i < x_resolution; i++) {
		uint8_t normalized_data = normalize_reading(sensor_val_history[((*reading_index - i) % 100 + 100) % 100 /* SENSOR_HISTORY_COUNT*/], scale_max, y_resolution);
		line_dimensions[0] = 124 - i - 2;
		line_dimensions[1] = y_resolution - normalized_data + 1;
		SSD1316_Draw_Line(line_dimensions, 1);
		line_dimensions[2] = line_dimensions[0];
		line_dimensions[3] = line_dimensions[1];
	}





	return GUI_SUCCESS;
}


enum gui_retval Display_Home_Page(struct SensorReadings* values) {
	SSD1316_Set_Cursor_Position(0, 0);
	SSD1316_Write_Image(home_screen, 128, 32, false);


	uint8_t bar_speed = 12;
	uint32_t bar_progress = screen_ticks * bar_speed;

	SSD1316_Draw_Filled_Rectangle(4, 27, 12, 27 - (bar_progress <= values->batt ? bar_progress : values->batt) * 21 / 100, 1);

	uint8_t co2_score = (1500 - (values->co2 > 1500 ? 1500 : values->co2)) / 10;
	if (co2_score > 100) co2_score = 100;
	SSD1316_Draw_Filled_Rectangle(23, 21, 32, 21 - (bar_progress <= co2_score ? bar_progress : co2_score) * 20 / 100, 1);

	uint8_t voc_score = (400 - (values->voc > 400 ? 400 : values->voc)) / 4;
	if (voc_score > 100) voc_score = 100;
	SSD1316_Draw_Filled_Rectangle(47, 21, 56, 21 - (bar_progress <= voc_score ? bar_progress : voc_score) * 20 / 100, 1);

	uint8_t opt_score = (values->lux > 50 ? 50 : values->lux) * 2;
	if (opt_score > 100) opt_score = 100;
	SSD1316_Draw_Filled_Rectangle(71, 21, 80, 21 - (bar_progress <= opt_score ? bar_progress : opt_score) * 20 / 100, 1);

	SSD1316_Set_Cursor_Position(92, 0);
	uint8_t total_score = co2_score * 2 / 3 + voc_score / 6 + opt_score / 12 + (100 - values->rh) / 10 + ((500 - values->nox) / 5) / 12;
	if (total_score > 60) {
		SSD1316_Write_Image(good_ind, 32, 32, false);
	} else if (total_score > 40) {
		SSD1316_Write_Image(caution_ind, 32, 32, false);
	} else {
		SSD1316_Write_Image(poor_ind, 32, 32, false);
	}




	return GUI_SUCCESS;
}


enum gui_retval Display_Charge_Page(uint8_t battery_percentage) {
	SSD1316_Set_Cursor_Position(0, 0);
	SSD1316_Write_Image(battery_blank, 128, 32, false);
	uint8_t dimensions[8] = {0x2A, 0x5, 0x5C, 0x5, 0x5C, 0x1A, 0x2A, 0x1A};
	uint8_t dim_by_percentage = (100 - battery_percentage) * 46 / 100; // 46 is the number of pixels for the charge indicator
	dimensions[0] += dim_by_percentage;
	dimensions[6] += dim_by_percentage;
	SSD1316_Draw_Filled_Rectangle(dimensions[0], dimensions[1], dimensions[4], dimensions[5], 1);

	return GUI_SUCCESS;
}

enum gui_retval Display_Calibration_Page() {
	SSD1316_Set_Cursor_Position(36, 8);
	char* str = "Calibrating...";
	SSD1316_Write_String(str, false, false);

	SSD1316_Set_Cursor_Position(59, 19);
	static char str1[6] = "\0\0\0\0\0";
	u16_to_str(30 - screen_ticks / 14, str1);
	SSD1316_Write_String(str1, false, true);


	if (screen_ticks == 1) {
		Set_Sensor_States(0xFF, 0x03);
	}


	if (screen_ticks >= 410) {
		Set_Sensor_States(0xAA, 0x02);
		return Change_State(HOME);
	}


	return GUI_SUCCESS;
}


enum gui_retval Display_Settings_Page( char** options, uint8_t* selection) {
	if (screen_state == SETTINGS_BASE) {
		SSD1316_Set_Cursor_Position(48, 0);
		SSD1316_Write_Image(settings_icon, 32, 32, false);
	} else {
		SSD1316_Set_Cursor_Position(8, 12);
		SSD1316_Write_String(options[0], false, false);
		for (uint8_t i = 0; i < sizeof(options); i++) {
			SSD1316_Set_Cursor_Position(92, 1 + 8*i);
			SSD1316_Write_String(options[i + 1], false, false);
		}

		SSD1316_Set_Cursor_Position(87, 8 * (*selection - 1));
		SSD1316_Write_Image(settings_pointer, 4, 8, true);

	}

	return GUI_SUCCESS;
}


enum gui_retval Display_Transistion(enum gui_state old_state, enum gui_state new_state) {
	screen_ticks = 0;
	return GUI_SUCCESS;
}


enum gui_retval Display_Off() {
	SSD1316_Set_Screen_Enable(false);
	return GUI_SUCCESS;
}



enum gui_retval Update_Display(struct SensorReadings* values, struct SensorHistory* sensor_val_history) {
	screen_ticks++;
	if (screen_ticks > screen_off_ticks) screen_state = OFF;
	SSD1316_Clear_Buffer();
	enum gui_retval return_val;
	switch (screen_state) {
		case OFF:
			return_val = Display_Off();
			break;

		case HOME:
			return_val = Display_Home_Page(values);
			break;



		case CO2_DISPLAY:
			return_val = Display_Sensor_Value(values->co2, co2_icon);
			break;

		case VOC_DISPLAY:
				return_val = Display_Sensor_Value(values->voc, voc_icon);
				break;

		case NOX_DISPLAY:
				return_val = Display_Sensor_Value(values->nox, nox_icon);
				break;

		case OPT_DISPLAY:
				return_val = Display_Sensor_Value(values->lux, opt_icon);
				break;

		case TMP_DISPLAY:
				return_val = Display_Sensor_Value(values->temp, tmp_icon);
				break;

		case RH_DISPLAY:
				return_val = Display_Sensor_Value(values->rh, rh_icon);
				break;

		case BAT_DISPLAY:
			return_val = Display_Sensor_Value(values->batt, bat_icon);
			break;



		case CO2_GRAPH:
				static char co2_label[4] = "CO2";
				return_val = Display_Sensor_Graph(sensor_val_history->co2_history, co2_label, 2000, sensor_val_history->co2_history_index);
				break;

		case VOC_GRAPH:
				static char voc_label[4] = "VOC";
				return_val = Display_Sensor_Graph(sensor_val_history->voc_history, voc_label, 400, sensor_val_history->voc_history_index);
				break;

		case NOX_GRAPH:
				static char nox_label[4] = "NOX";
				return_val = Display_Sensor_Graph(sensor_val_history->nox_history, nox_label, 100, sensor_val_history->nox_history_index);
				break;

		case OPT_GRAPH:
				static char opt_label[4] = "OPT";
				return_val = Display_Sensor_Graph(sensor_val_history->lux_history, opt_label, 100, sensor_val_history->opt_history_index);
				break;

		case TMP_GRAPH:
				static char tmp_label[4] = "TMP";
				return_val = Display_Sensor_Graph(sensor_val_history->tmp_history, tmp_label, 100, sensor_val_history->tmp_history_index);
				break;

		case RH_GRAPH:
				static char rh_label[4] = "HUM";
				return_val = Display_Sensor_Graph(sensor_val_history->rh_history, rh_label, 100, sensor_val_history->rh_history_index);
				break;

		case BAT_GRAPH:
			static char bat_label[4] = "BAT";
			return_val = Display_Sensor_Graph(sensor_val_history->bat_history, bat_label, 100, sensor_val_history->bat_history_index);
			break;



		case CHARGE_MODE:
			return_val = Display_Charge_Page(values->batt);
			break;

		case CALIBRATION_MODE:
			return_val = Display_Calibration_Page();
			break;



		case SETTINGS_BASE:
			char* base_options[1] = {" "};
			uint8_t base_selection = 1;
			return_val = Display_Settings_Page(base_options, &base_selection);
			break;

		case SETTINGS_CONTRAST:
			char* contrast_options[5] = {"Contrast", "10%", "25%", "50%", "100%"};
			uint8_t contrast_selection = Get_Setting_Value(SETTINGS_CONTRAST);
			return_val = Display_Settings_Page(contrast_options, &contrast_selection);
			break;

		case SETTINGS_SCREEN_SLEEP:
			char* screensleep_options[5] = {"Screen Sleep", "30s", "1m", "5m", "Off"};
			uint8_t screensleep_selection = Get_Setting_Value(SETTINGS_SCREEN_SLEEP);
			return_val = Display_Settings_Page(screensleep_options, &screensleep_selection);
			break;

		case SETTINGS_POLL_RATE:
			char* pollrate_options[5] = {"Sampling Rate", "1m", "5m", "10m", "30m"};
			uint8_t pollrate_selection = Get_Setting_Value(SETTINGS_POLL_RATE);
			return_val = Display_Settings_Page(pollrate_options, &pollrate_selection);
			break;

		case SETTINGS_GRAPH_TIME:
			char* graphtime_options[5] = {"Graph History", "1h", "4h", "12h"};
			uint8_t graphtime_selection = Get_Setting_Value(SETTINGS_GRAPH_TIME);
			return_val = Display_Settings_Page(graphtime_options, &graphtime_selection);
			break;

		case SETTINGS_HOME_SELECT:
			char* homeselect_options[5] = {"Home Select", "Home", "CO2", "VOC", "Light"};
			uint8_t homeselect_selection = Get_Setting_Value(SETTINGS_HOME_SELECT);
			return_val = Display_Settings_Page(homeselect_options, &homeselect_selection);
			break;

		case SETTINGS_UNIT_SELECT:
			char* unitselect_options[5] = {"Temp Units", "oF", "oC"};
			uint8_t unitselect_selection = Get_Setting_Value(SETTINGS_UNIT_SELECT);
			return_val = Display_Settings_Page(unitselect_options, &unitselect_selection);
			break;

		case SETTINGS_LIGHTDARK_SELECT:
			char* lightdark_options[5] = {"Dark Mode", "On", "Off"};
			uint8_t lightdark_selection = Get_Setting_Value(SETTINGS_LIGHTDARK_SELECT);
			return_val = Display_Settings_Page(lightdark_options, &lightdark_selection);
			break;



		default:
			return_val = Display_Off();
			break;
	}

	SSD1316_Update_Screen();

	return return_val;

}

enum gui_retval Check_Button_Navigation(enum button_state button1, enum button_state button2) {
	enum gui_retval return_val = GUI_SUCCESS;
	enum gui_state new_state = screen_state;
	if (button1 == BUTTON_PRESS) {
		switch (screen_state) {
			case OFF:
				new_state = ((home_select == 1 ? HOME : (home_select == 2 ? CO2_DISPLAY : (home_select == 3 ? VOC_DISPLAY : OPT_DISPLAY))));
				break;

			case HOME:
				new_state = (CALIBRATION_MODE);
				break;



			case CO2_DISPLAY:
				new_state = (CO2_GRAPH);
				break;

			case VOC_DISPLAY:
				new_state = (VOC_GRAPH);
				break;

			case NOX_DISPLAY:
				new_state = (NOX_GRAPH);
				break;

			case OPT_DISPLAY:
				new_state = (OPT_GRAPH);
				break;

			case TMP_DISPLAY:
				new_state = (TMP_GRAPH);
				break;

			case RH_DISPLAY:
				new_state = (RH_GRAPH);
				break;

			case BAT_DISPLAY:
				new_state = (BAT_GRAPH);
				break;



			case CO2_GRAPH:
				new_state = (CO2_DISPLAY);
				break;

			case VOC_GRAPH:
				new_state = (VOC_DISPLAY);
				break;

			case NOX_GRAPH:
				new_state = (NOX_DISPLAY);
				break;

			case OPT_GRAPH:
				new_state = (OPT_DISPLAY);
				break;

			case TMP_GRAPH:
				new_state = (TMP_DISPLAY);
				break;

			case RH_GRAPH:
				new_state = (RH_DISPLAY);
				break;

			case BAT_GRAPH:
				new_state = (BAT_DISPLAY);
				break;



			case CHARGE_MODE:
				new_state = (HOME);
				break;

			case CALIBRATION_MODE:
				new_state = (HOME);
				break;



			case SETTINGS_BASE:
				new_state = (SETTINGS_CONTRAST);
				break;

			case SETTINGS_CONTRAST:
				new_state = (SETTINGS_SCREEN_SLEEP);
				break;

			case SETTINGS_SCREEN_SLEEP:
				new_state = (SETTINGS_POLL_RATE);
				break;

			case SETTINGS_POLL_RATE:
				new_state = (SETTINGS_GRAPH_TIME);
				break;

			case SETTINGS_GRAPH_TIME:
				new_state = (SETTINGS_HOME_SELECT);
				break;

			case SETTINGS_HOME_SELECT:
				new_state = (SETTINGS_UNIT_SELECT);
				break;

			case SETTINGS_UNIT_SELECT:
				new_state = (SETTINGS_LIGHTDARK_SELECT);
				break;

			case SETTINGS_LIGHTDARK_SELECT:
				new_state = (SETTINGS_BASE);
				break;


			default:
				new_state = (HOME);
				break;

		}
	} else if (button2 == BUTTON_PRESS) {
		switch (screen_state) {
		case OFF:
			new_state = ((home_select == 1 ? HOME : (home_select == 2 ? CO2_DISPLAY : VOC_DISPLAY)));
			break;

		case HOME:
			new_state = (CO2_DISPLAY);
			break;



		case CO2_DISPLAY:
			new_state = (VOC_DISPLAY);
			break;

		case VOC_DISPLAY:
			new_state = (NOX_DISPLAY);
			break;

		case NOX_DISPLAY:
			new_state = (OPT_DISPLAY);
			break;

		case OPT_DISPLAY:
			new_state = (TMP_DISPLAY);
			break;

		case TMP_DISPLAY:
			new_state = (RH_DISPLAY);
			break;

		case RH_DISPLAY:
			new_state = (BAT_DISPLAY);
			break;

		case BAT_DISPLAY:
			new_state = (SETTINGS_BASE);
			break;



		case CO2_GRAPH:
			new_state = (VOC_GRAPH);
			break;

		case VOC_GRAPH:
			new_state = (NOX_GRAPH);
			break;

		case NOX_GRAPH:
			new_state = (OPT_GRAPH);
			break;

		case OPT_GRAPH:
			new_state = (TMP_GRAPH);
			break;

		case TMP_GRAPH:
			new_state = (RH_GRAPH);
			break;

		case RH_GRAPH:
			new_state = (BAT_GRAPH);
			break;

		case BAT_GRAPH:
			new_state = (CO2_GRAPH);
			break;



		case CHARGE_MODE:
			new_state = (HOME);
			break;

		case CALIBRATION_MODE:
			new_state = (HOME);
			break;


		case SETTINGS_BASE:
			new_state = (HOME);
			break;

		case SETTINGS_CONTRAST:
			new_state = (SETTINGS_CONTRAST);
			break;

		case SETTINGS_SCREEN_SLEEP:
			new_state = (SETTINGS_SCREEN_SLEEP);
			break;

		case SETTINGS_POLL_RATE:
			new_state = (SETTINGS_POLL_RATE);
			break;

		case SETTINGS_GRAPH_TIME:
			new_state = (SETTINGS_GRAPH_TIME);
			break;

		case SETTINGS_HOME_SELECT:
			new_state = (SETTINGS_HOME_SELECT);
			break;

		case SETTINGS_UNIT_SELECT:
			new_state = (SETTINGS_UNIT_SELECT);
			break;

		case SETTINGS_LIGHTDARK_SELECT:
			new_state = (SETTINGS_LIGHTDARK_SELECT);
			break;


		default:
			new_state = (HOME);
			break;

		}
	}

	if (new_state != screen_state) {
		return_val = Change_State(new_state);
	} else if (screen_state <= SETTINGS_LIGHTDARK_SELECT && screen_state >= SETTINGS_CONTRAST) {
		return_val = Change_State(new_state);
	}

	return return_val;
}


