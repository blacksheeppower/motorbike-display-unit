/*
 * Lcd.c
 *
 *  Created on: Sep 16, 2025
 */

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
 *                                          INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 ==================================================================================================*/
#include "Lcd.h"
#include <stdio.h>
#include "Fm24clxx.h"
/*==================================================================================================
 SOURCE FILE VERSION INFORMATION
 ==================================================================================================*/

/*==================================================================================================
 *                                     FILE VERSION CHECKS
 ==================================================================================================*/

/*==================================================================================================
 *                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/
typedef enum {
    CLOCK_EDIT_NONE = 0,
    CLOCK_EDIT_HOUR,
    CLOCK_EDIT_MINUTE,
	CLOCK_HOUR_FORMAT,
	CLOCK_MODE_FORMAT,
} ClockEditState_e;

static ClockEditState_e ClockEditState = CLOCK_EDIT_NONE;
static u32 lastBlink = 0;
static bool blinkState = false;
static uint32_t LastActionTime = 0;
static bool skipAMPM = false;
/*==================================================================================================
 *                                          LOCAL MACROS
 ==================================================================================================*/

/*==================================================================================================
 *                                         LOCAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                         LOCAL VARIABLES
 ==================================================================================================*/

/*==================================================================================================
 *                                        GLOBAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                        GLOBAL VARIABLES
 ==================================================================================================*/
boolean IsExtPressButton = FALSE;
/*==================================================================================================
 *                                    LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/
uint8_t RTC_ConvertTo12hFormat(uint8_t hour_24)
{
    if (hour_24 == 0)
    {
        // 00:00 -> 12 AM
        Bike_SysData.Display.Setting.IsModeAM = 1U;
        return 12U;
    }
    else if (hour_24 == 12)
    {
        // 12:00 -> 12 PM
        Bike_SysData.Display.Setting.IsModeAM = 0U;
        return 12U;
    }
    else if (hour_24 > 12)
    {
        // 13:00..23:59 -> 1..11 PM
        Bike_SysData.Display.Setting.IsModeAM = 0U;
        return (hour_24 - 12U);
    }
    else
    {
        // 01:00..11:59 -> AM
        Bike_SysData.Display.Setting.IsModeAM = 1U;
        return hour_24;
    }
}

uint8_t RTC_ConvertTo24hFormat(uint8_t hour_12)
{
    if (Bike_SysData.Display.Setting.IsModeAM)
    {
        if (hour_12 == 12U)
        {
            // 12 AM -> 0h
            return 0U;
        }
        else
        {
            // 1..11 AM -> giữ nguyên
            return hour_12;
        }
    }
    else
    {
        if (hour_12 == 12U)
        {
            // 12 PM -> 12h
            return 12U;
        }
        else
        {
            // 1..11 PM -> +12h
            return (hour_12 + 12U);
        }
    }
}

/*==================================================================================================
 *                                         LOCAL VARIABLES
 ==================================================================================================*/
static float Metter_Odo_100Ms = 0;
static float Metter_Trip_1_100Ms = 0;
static float Metter_Trip_2_100Ms = 0;
static uint8_t BatteryPercent = 0;
/*==================================================================================================
 *                                         LOCAL FUNCTIONS
 ==================================================================================================*/

/*==================================================================================================
 *                                         GLOBAL FUNCTIONS
 ==================================================================================================*/

void Lcd_InitFirstTime(void) {
	static boolean bGetFirstStick = FALSE;
	static boolean bTimeDelayExpire = FALSE;
	static boolean bInitLcdDone = FALSE;
	static u32 Stick = 0;

	if (bGetFirstStick == FALSE) {
		Stick = HAL_GetTick();
		bGetFirstStick = TRUE;
	} else {
		if (bTimeDelayExpire == FALSE) {
			if (HAL_GetTick() - Stick > 350) {
				bTimeDelayExpire = TRUE;
			}
		} else {
			if (bTimeDelayExpire == TRUE) {
				if (bInitLcdDone == FALSE) {
					HAL_GPIO_WritePin(LED_LCD_GPIO_Port, LED_LCD_Pin, GPIO_PIN_SET);
					bInitLcdDone = TRUE;
				}
			}
		}
	}

}

void Lcd_UpdateUi(void) {
	//CanMsgRxType frame = {0};
	char m[20] = { 0 };
	static Bike_SysDataType Old_Bike_SysData = { 0 };

	if (HAL_GetTick() - LastActionTime > 5000)
	{
		if (ClockEditState == CLOCK_MODE_FORMAT)
		{
			if (skipAMPM)
			{
				ClockEditState = CLOCK_EDIT_HOUR;
			}
			else
			{
				ClockEditState = CLOCK_HOUR_FORMAT;
			}
			LastActionTime = HAL_GetTick();
		}
		else if (ClockEditState == CLOCK_HOUR_FORMAT)
		{
			ClockEditState = CLOCK_EDIT_HOUR;
			if (Bike_SysData.Display.Setting.IsMode24)
			{
				Bike_SysData.Display.Time.Raw.Hour = RTC_ConvertTo24hFormat(Bike_SysData.Display.Time.Raw.Hour);
			}
			else
			{
				Bike_SysData.Display.Time.Raw.Hour = RTC_ConvertTo12hFormat(Bike_SysData.Display.Time.Raw.Hour);
			}
			LastActionTime = HAL_GetTick();
		}
		else if (ClockEditState == CLOCK_EDIT_HOUR)
		{
			ClockEditState = CLOCK_EDIT_MINUTE;
			LastActionTime = HAL_GetTick();
		}
		else if (ClockEditState == CLOCK_EDIT_MINUTE)
		{
			ClockEditState = CLOCK_EDIT_NONE;
		    Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_TRIP_1;
		    lastBlink = HAL_GetTick();
		    blinkState = false;
		}
	}

	//update time
	if (Bike_SysData.Display.Time.Raw.Year != Old_Bike_SysData.Display.Time.Raw.Year || //year
		Bike_SysData.Display.Time.Raw.Month != Old_Bike_SysData.Display.Time.Raw.Month || //Month
		Bike_SysData.Display.Time.Raw.Date != Old_Bike_SysData.Display.Time.Raw.Date || //Day
		Bike_SysData.Display.Time.Raw.Hour != Old_Bike_SysData.Display.Time.Raw.Hour || //Hour
		Bike_SysData.Display.Time.Raw.Minute != Old_Bike_SysData.Display.Time.Raw.Minute || //Minute
		Bike_SysData.Display.Time.Raw.Second != Old_Bike_SysData.Display.Time.Raw.Second)
	{

		if (Bike_SysData.Display.Time.Raw.Hour != Old_Bike_SysData.Display.Time.Raw.Hour &&
				Old_Bike_SysData.Display.Time.Raw.Hour == 12)
		{
			Bike_SysData.Display.Setting.IsModeAM = !Bike_SysData.Display.Setting.IsModeAM;
		}

	    if (!Bike_SysData.Display.Setting.IsMode24)
	    {
	    	if (Bike_SysData.Display.Time.Raw.Hour > 12)
	    	{
	    		Bike_SysData.Display.Time.Raw.Hour -= 12;
	    	}
	    }

		for (uint8_t i = 0; i < 8; i++)
		{
			Old_Bike_SysData.Display.Time.bytes[i] = Bike_SysData.Display.Time.bytes[i];
		}



		//24h
	}
	if (ClockEditState == CLOCK_MODE_FORMAT && (HAL_GetTick() - lastBlink >= 500))
	{
		blinkState = !blinkState;

		if (blinkState)
		{
			if (Bike_SysData.Display.Setting.IsMode24 == true)
			{
				sprintf(m, "Mode 24");
			}
			else
			{
				sprintf(m, "Mode 12");
			}
		}
		else
		{
			sprintf(m, "  ");
		}
	}
	else if (ClockEditState == CLOCK_HOUR_FORMAT && (HAL_GetTick() - lastBlink >= 500))
	{
		blinkState = !blinkState;

		if (blinkState)
		{
			if (Bike_SysData.Display.Setting.IsModeAM == true)
			{
				sprintf(m, "AM");
			}
			else
			{
				sprintf(m, "PM");
			}
		}
		else
		{
			sprintf(m, "  ");
		}
	}
	else if (ClockEditState == CLOCK_EDIT_HOUR && (HAL_GetTick() - lastBlink >= 500))
	{
		blinkState = !blinkState;
		if (blinkState)
		{
			sprintf(m, "  :%02d", Bike_SysData.Display.Time.Raw.Minute);
		}
		else
		{
			sprintf(m, "%02d:%02d", Bike_SysData.Display.Time.Raw.Hour, Bike_SysData.Display.Time.Raw.Minute);
		}
	}
	else if (ClockEditState == CLOCK_EDIT_MINUTE && (HAL_GetTick() - lastBlink >= 500))
	{
		blinkState = !blinkState;
		if (blinkState)
		{
			sprintf(m, "%02d:  ", Bike_SysData.Display.Time.Raw.Hour);
		}
		else
		{
			sprintf(m, "%02d:%02d", Bike_SysData.Display.Time.Raw.Hour, Bike_SysData.Display.Time.Raw.Minute);
		}
	}
	else
	{
		if (Bike_SysData.Display.Setting.IsMode24)
		{
			sprintf(m, "%02d:%02d", Bike_SysData.Display.Time.Raw.Hour, Bike_SysData.Display.Time.Raw.Minute);
		}
		else
		{
			sprintf(m, "%02d:%02d %s", Bike_SysData.Display.Time.Raw.Hour, Bike_SysData.Display.Time.Raw.Minute, Bike_SysData.Display.Setting.IsModeAM ? "AM" : "PM");
		}
	}

	if (HAL_GetTick() - lastBlink >= 500)
	{
		lv_label_set_text(ui_CharingScreenlbTime1, m);
		lv_label_set_text(ui_lbTime, m);
		lastBlink = HAL_GetTick();
	}

	if (Bike_SysData.Display.Setting.IsUnitKm)
	{
		lv_label_set_text(ui_lbSpeedUnit, "KPH");
		lv_label_set_text(ui_lbOdoKm, "km");
		lv_label_set_text(ui_lbTripKm, "km");
	}
	else
	{
		lv_label_set_text(ui_lbSpeedUnit, "MPH");
		lv_label_set_text(ui_lbOdoKm, "mile");
		lv_label_set_text(ui_lbTripKm, "mile");
	}

	if (Old_Bike_SysData.VmuM.Switch.Raw.ChargeStatus != Bike_SysData.VmuM.Switch.Raw.ChargeStatus)
	{
		if (Bike_SysData.VmuM.Switch.Raw.ChargeStatus == VMU_CHARGING)
		{
			if (Bike_SysData.Display.ScreenNow != VMUD_SCREEN_CHARING)
			{
				lv_disp_load_scr(ui_ChargingScreen);
				lv_img_set_src(ui_CharingScreenImgChargeStatus1, &ui_img_charge_on_png);
				Bike_SysData.Display.ScreenNow = VMUD_SCREEN_CHARING;
				sprintf(m, "%d%%", BatteryPercent);
				{
					lv_label_set_text(ui_CharingScreenlbBatteryPercent1, m);
					lv_slider_set_value(ui_SldBattery1, BatteryPercent, LV_ANIM_OFF);
					lv_arc_set_value(ui_CharingScreenBatteryMetter1, BatteryPercent);

					if (BatteryPercent <= 20)
					{
						lv_obj_set_style_bg_color(ui_SldBattery1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
						lv_obj_set_style_arc_color(ui_CharingScreenBatteryMetter1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
					}
					else
					{
						lv_obj_set_style_bg_color(ui_SldBattery1, lv_color_hex(0x00FF4F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
						lv_obj_set_style_arc_color(ui_CharingScreenBatteryMetter1, lv_color_hex(0xF5F5F5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
					}
				}
			}
		}
		else
		{
			if (Bike_SysData.Display.ScreenNow == VMUD_SCREEN_CHARING)
			{
				lv_disp_load_scr(ui_HomeScreen);
				lv_img_set_src(ui_ImgChargeStatus, &ui_img_charge_off_png);
				Bike_SysData.Display.ScreenNow = VMUD_SCREEN_HOME;
			}
		}
		Old_Bike_SysData.VmuM.Switch.Raw.ChargeStatus = Bike_SysData.VmuM.Switch.Raw.ChargeStatus;
	}

	if (Bike_SysData.Display.ScreenNow != VMUD_SCREEN_CHARING)
	{
		float temp_odo = 0;
		float temp_trip1 = 0;
		float temp_trip2 = 0;
		float temp_range = 0;
		//odo with other task calc
		if (!Bike_SysData.Display.Setting.IsUnitKm)
		{
			temp_odo = Bike_SysData.Display.Metter.Odo * 0.621371;
		}
		else
		{
			temp_odo = Bike_SysData.Display.Metter.Odo;
		}
		sprintf(m, "%.1f", temp_odo / 1.0);
		lv_label_set_text(ui_lbOdoValue, m);


		if (Bike_SysData.Display.TripScreenNow != Old_Bike_SysData.Display.TripScreenNow)
		{
			Old_Bike_SysData.Display.TripScreenNow = Bike_SysData.Display.TripScreenNow;
			if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_1)
			{
				lv_label_set_text(ui_lbTrip, "trip 1");
			}
			else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_2)
			{
				lv_label_set_text(ui_lbTrip, "trip 2");
			}
			else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_REMAIN) {
				lv_label_set_text(ui_lbTrip, "range");
			}
		}

		if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_1)
		{
			if (!Bike_SysData.Display.Setting.IsUnitKm)
			{
				temp_trip1 = Bike_SysData.Display.Metter.Trip_1 * 0.621371;
			}
			else
			{
				temp_trip1 = Bike_SysData.Display.Metter.Trip_1;
			}
			sprintf(m, "%.1f", temp_trip1 / 1.0);
			lv_label_set_text(ui_lbTripValue, m);
		}
		else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_2)
		{
			if (!Bike_SysData.Display.Setting.IsUnitKm)
			{
				temp_trip2 = Bike_SysData.Display.Metter.Trip_2 * 0.621371;
			}
			else
			{
				temp_trip2 = Bike_SysData.Display.Metter.Trip_2;
			}
			sprintf(m, "%.1f", temp_trip2 / 1.0);
		}
		else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_REMAIN)
		{
			memcpy(&temp_range, &Bike_SysData.VmuM.RemainDistance, sizeof(float));
			if (!Bike_SysData.Display.Setting.IsUnitKm)
			{
				temp_range = temp_range * 0.621371;
			}
			Bike_SysData.Display.Metter.Remain = temp_range;

			sprintf(m, "%.1f", Bike_SysData.Display.Metter.Remain / 1.0);
			lv_label_set_text(ui_lbTripValue, m);
		}

		//speed
		if (Bike_SysData.Esc.Esc_1.Raw.Speed != Old_Bike_SysData.Esc.Esc_1.Raw.Speed)
		{
			Old_Bike_SysData.Esc.Esc_1.Raw.Speed = Bike_SysData.Esc.Esc_1.Raw.Speed;

			if (!Bike_SysData.Display.Setting.IsUnitKm)
            {
				Bike_SysData.Esc.Esc_1.Raw.Speed *= 0.621371;
            }

			sprintf(m, "%d", Bike_SysData.Esc.Esc_1.Raw.Speed);
			lv_label_set_text(ui_lbSpeedValue, m);
			lv_arc_set_value(ui_SpeedMetter, Bike_SysData.Esc.Esc_1.Raw.Speed);
		}

		//update warning
		if (Old_Bike_SysData.Esc.Esc_3.Raw.ErrorCode != Bike_SysData.Esc.Esc_3.Raw.ErrorCode ||    //
			Old_Bike_SysData.VmuM.Fault.Raw.FaultBms1 != Bike_SysData.VmuM.Fault.Raw.FaultBms1 ||    //
			Old_Bike_SysData.VmuM.Fault.Raw.FaultBms2 != Bike_SysData.VmuM.Fault.Raw.FaultBms2)
		{
			Old_Bike_SysData.Esc.Esc_3.Raw.ErrorCode = Bike_SysData.Esc.Esc_3.Raw.ErrorCode;
			Old_Bike_SysData.VmuM.Fault.Raw.FaultBms1 = Bike_SysData.VmuM.Fault.Raw.FaultBms1;
			Old_Bike_SysData.VmuM.Fault.Raw.FaultBms2 = Bike_SysData.VmuM.Fault.Raw.FaultBms2;
			if ((Old_Bike_SysData.Esc.Esc_3.Raw.ErrorCode != 0 || Old_Bike_SysData.VmuM.Fault.Raw.FaultBms1 != 0 || Old_Bike_SysData.VmuM.Fault.Raw.FaultBms1 != 0)
					&& (Bike_SysData.VmuM.Switch.Raw.AccStatus_1 == 1) && (Bike_SysData.VmuM.Switch.Raw.AccStatus_2 == 1))
			{
				lv_img_set_src(ui_imgWarning, &ui_img_warning_on_png);
			}
			else
			{
				lv_img_set_src(ui_imgWarning, &ui_img_warning_off_png);
			}

		}

		//turn LED
		if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus != Bike_SysData.VmuF.Light.Raw.TurnLightStatus)
		{
			Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus = Bike_SysData.VmuF.Light.Raw.TurnLightStatus;
			if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 0)
			{
				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_off_png);
				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_off_png);
			}
			else if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 1)
			{
				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_on_png);
				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_off_png);
			}
			else if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 2)
			{
				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_off_png);
				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_on_png);
			}
			else if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 3)
			{
				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_on_png);
				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_on_png);
			}
			lv_obj_invalidate(ui_imgXinNhanLeft);
			lv_obj_invalidate(ui_imgXinNhanRight);
		}

		//fog light
		if (Old_Bike_SysData.VmuF.Light.Raw.FogLightStatus != Bike_SysData.VmuF.Light.Raw.FogLightStatus)
		{
			Old_Bike_SysData.VmuF.Light.Raw.FogLightStatus = Bike_SysData.VmuF.Light.Raw.FogLightStatus;
		}

		//Head Light
		if (Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus != Bike_SysData.VmuF.Light.Raw.HeadLightStatus)
		{
			Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus = Bike_SysData.VmuF.Light.Raw.HeadLightStatus;

			if (Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 0)
			{
				lv_img_set_src(ui_imgCosPha, &ui_img_cos_off_png);
			}
			else if (Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 1)
			{
				lv_img_set_src(ui_imgCosPha, &ui_img_cos_on_png);
			}
			else if (Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 2 || Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 3) // 3 mean while low beam on, we turn on high beam so it will overlap low beam
			{
				lv_img_set_src(ui_imgCosPha, &ui_img_pha_on_png);
			}
		}

		//Gear D-N
		if (Old_Bike_SysData.Esc.Esc_2.Raw.ReadyToGo != Bike_SysData.Esc.Esc_2.Raw.ReadyToGo)
		{
			Old_Bike_SysData.Esc.Esc_2.Raw.ReadyToGo = Bike_SysData.Esc.Esc_2.Raw.ReadyToGo;
			if (Old_Bike_SysData.Esc.Esc_2.Raw.ReadyToGo == 0)
			{
				lv_img_set_src(ui_imgGear, &ui_img_n_png);
			}
			else
			{
				lv_img_set_src(ui_imgGear, &ui_img_d_png);
			}
		}

		//over temperature
		if (Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp != Bike_SysData.Esc.Esc_2.Raw.ControllerTemp ||    //
			Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp != Bike_SysData.Esc.Esc_2.Raw.MotorTemp ||
			Old_Bike_SysData.VmuM.Temperature.Raw.Temperature != Bike_SysData.VmuM.Temperature.Raw.Temperature)
		{
			Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp = Bike_SysData.Esc.Esc_2.Raw.ControllerTemp;
			Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp = Bike_SysData.Esc.Esc_2.Raw.MotorTemp;
			float temp_temperature = 0;
			memcpy(&temp_temperature, &Bike_SysData.VmuM.Temperature, sizeof(float));
			Old_Bike_SysData.VmuM.Temperature.Raw.Temperature = Bike_SysData.VmuM.Temperature.Raw.Temperature;

			if ((Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp - 273) > 60
					|| (Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp - 273) > 60
					|| temp_temperature > 60)
			{
				lv_img_set_src(ui_imgTemOver, &ui_img_over_tem_on_png);
			}
			else
			{
				lv_img_set_src(ui_imgTemOver, &ui_img_over_tem_off_png);
			}
		}
	}

	//update battery
	if (Old_Bike_SysData.VmuM.Batt_1.Raw.SocBms != Bike_SysData.VmuM.Batt_1.Raw.SocBms ||    //
		Old_Bike_SysData.VmuM.Batt_2.Raw.SocBms != Bike_SysData.VmuM.Batt_2.Raw.SocBms)
	{
		Old_Bike_SysData.VmuM.Batt_1.Raw.SocBms = Bike_SysData.VmuM.Batt_1.Raw.SocBms;
		Old_Bike_SysData.VmuM.Batt_2.Raw.SocBms = Bike_SysData.VmuM.Batt_2.Raw.SocBms;

		BatteryPercent = (Bike_SysData.VmuM.Batt_1.Raw.SocBms + Bike_SysData.VmuM.Batt_2.Raw.SocBms) / 2;
		sprintf(m, "%d%%", BatteryPercent);
		if (Old_Bike_SysData.VmuM.Switch.Raw.ChargeStatus == VMU_NOT_CHARGE) {
			lv_label_set_text(ui_lbBatteryPercent, m);
			lv_slider_set_value(ui_SldBattery, BatteryPercent, LV_ANIM_OFF);
			lv_arc_set_value(ui_BatteryMetter, BatteryPercent);

			if (BatteryPercent <= 20)
			{
				lv_obj_set_style_bg_color(ui_SldBattery, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
				lv_obj_set_style_arc_color(ui_BatteryMetter, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
			}
			else
			{
				lv_obj_set_style_bg_color(ui_SldBattery, lv_color_hex(0x00FF4F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
				lv_obj_set_style_arc_color(ui_BatteryMetter, lv_color_hex(0xF5F5F5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
			}
		}
		else
		{
			lv_label_set_text(ui_CharingScreenlbBatteryPercent1, m);
			lv_slider_set_value(ui_SldBattery1, BatteryPercent, LV_ANIM_OFF);
			lv_arc_set_value(ui_CharingScreenBatteryMetter1, BatteryPercent);

			if (BatteryPercent <= 20)
			{
				lv_obj_set_style_bg_color(ui_SldBattery1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
				lv_obj_set_style_arc_color(ui_CharingScreenBatteryMetter1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
			}
			else
			{
				lv_obj_set_style_bg_color(ui_SldBattery1, lv_color_hex(0x00FF4F), LV_PART_INDICATOR | LV_STATE_DEFAULT);
				lv_obj_set_style_arc_color(ui_CharingScreenBatteryMetter1, lv_color_hex(0xF5F5F5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
			}
		}
	}
}

Std_ReturnType Lcd_WriteMetter(Display_MetterType Metter) {
	return Fm24clxx_Write(0, 0, (u8*) &Metter, sizeof(Display_MetterType));
}

Std_ReturnType Lcd_ReadMetter(Display_MetterType *Metter) {
	return Fm24clxx_Read(0, 0, (u8*) Metter, sizeof(Display_MetterType));
}

Std_ReturnType Lcd_WriteSetting(Display_SettingType Setting) {
	uint16_t offset = sizeof(Display_MetterType);
	return Fm24clxx_Write(0, offset, (u8*) &Setting, sizeof(Display_SettingType));
}

Std_ReturnType Lcd_ReadSetting(Display_SettingType *Setting) {
	uint16_t offset = sizeof(Display_MetterType);
	return Fm24clxx_Read(0, offset, (u8*) Setting, sizeof(Display_SettingType));
}

void Lcd_LoadMetter(void) {
	Display_MetterType Metter = { 0 };
	Lcd_ReadMetter(&Metter);
	Bike_SysData.Display.Metter.Odo = Metter.Odo;
	Bike_SysData.Display.Metter.Trip_1 = Metter.Trip_1;
	Bike_SysData.Display.Metter.Trip_2 = Metter.Trip_2;
}

void Lcd_LoadSetting(void)
{
	Display_SettingType Setting = { false, false, false };
	Lcd_ReadSetting(&Setting);
	Bike_SysData.Display.Setting.IsMode24 = Setting.IsMode24;
	Bike_SysData.Display.Setting.IsModeAM = Setting.IsModeAM;
	Bike_SysData.Display.Setting.IsUnitKm = Setting.IsUnitKm;
}

void Lcd_CalcMetter(void) {
	float s = Bike_SysData.Esc.Esc_1.Raw.Speed / 3.6f * 0.03; // Lcd_CalcMetter is called each 30ms
															  // km/h / 3.6 = m/s, times 0.03s -> distance
	Metter_Odo_100Ms += s;
	Metter_Trip_1_100Ms += s;
	Metter_Trip_2_100Ms += s;

	if (Metter_Odo_100Ms > 100) {
		Bike_SysData.Display.Metter.Odo += Metter_Odo_100Ms / 1000.0;
		Metter_Odo_100Ms = 0;
	}

	if (Metter_Trip_1_100Ms > 100) {
		Bike_SysData.Display.Metter.Trip_1 += Metter_Trip_1_100Ms / 1000.0;
		Metter_Trip_1_100Ms = 0;
	}

	if (Metter_Trip_2_100Ms > 100) {
		Bike_SysData.Display.Metter.Trip_2 += Metter_Trip_2_100Ms / 1000.0;
		Metter_Trip_2_100Ms = 0;
	}
}

void Lcd_GetButton(void) {
	static u32 Stick = 0;
	static Button_eState OldBtnState = BUTTON_IDE;

    if (IsExtPressButton == TRUE) {
        if (LCD_BUTTON_READ_INPUT == STD_LOW) {
            if (OldBtnState == BUTTON_IDE) {
                OldBtnState = BUTTON_DEBOUND;
                Stick = HAL_GetTick();
            } else if (OldBtnState == BUTTON_DEBOUND) {
                if (HAL_GetTick() - Stick > 100) {
                    OldBtnState = BUTTON_PRESSED;
                    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
                    // quick press process
                    if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_REMAIN) {
                        if (ClockEditState == CLOCK_EDIT_NONE) {
                        	// do nothing
                        }
                        else if (ClockEditState == CLOCK_MODE_FORMAT)
                        {
                        	Bike_SysData.Display.Setting.IsMode24 = !Bike_SysData.Display.Setting.IsMode24;
                        	skipAMPM = Bike_SysData.Display.Setting.IsMode24 ? true : false;
                        	LastActionTime = HAL_GetTick();
                        }
                        else if (ClockEditState == CLOCK_HOUR_FORMAT)
                        {
                        	Bike_SysData.Display.Setting.IsModeAM = !Bike_SysData.Display.Setting.IsModeAM;
                        	RTC_SetTimeDate();
                        	LastActionTime = HAL_GetTick();
                        }
                        else if (ClockEditState == CLOCK_EDIT_HOUR)
                        {
                            // increase hour
                        	if (Bike_SysData.Display.Setting.IsMode24)
                        	{
                        		Bike_SysData.Display.Time.Raw.Hour = (Bike_SysData.Display.Time.Raw.Hour + 1) % 24;
                        	}
                        	else
                        	{
                                Bike_SysData.Display.Time.Raw.Hour++;
                                if (Bike_SysData.Display.Time.Raw.Hour > 12U)
                                {
                                    Bike_SysData.Display.Time.Raw.Hour = 1U; // avoid 0h in mode 12h
                                }
                        	}

                            RTC_SetTimeDate();
                            LastActionTime = HAL_GetTick();
                        } else if (ClockEditState == CLOCK_EDIT_MINUTE) {
                            // increase minute
                            Bike_SysData.Display.Time.Raw.Minute = (Bike_SysData.Display.Time.Raw.Minute + 1) % 60;
                            RTC_SetTimeDate();
                            LastActionTime = HAL_GetTick();
                        }
                    }
                }
            } else if (OldBtnState == BUTTON_PRESSED) {
                if (HAL_GetTick() - Stick > 1500) {
                    OldBtnState = BUTTON_HOLD;
                    if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_REMAIN) {
                    	if (ClockEditState == CLOCK_EDIT_NONE) {
							ClockEditState = CLOCK_MODE_FORMAT; // start editing clock
							lastBlink = HAL_GetTick();
							blinkState = true;
						}
                    	else if (ClockEditState == CLOCK_MODE_FORMAT)
                    	{
                    		// do nothing
                    	}
                    	else if (ClockEditState == CLOCK_HOUR_FORMAT)
                    	{
                    		// do nothing
                    	}
                    	else if (ClockEditState == CLOCK_EDIT_HOUR)
                    	{
                    		// do nothing
                        }
                    	else if (ClockEditState == CLOCK_EDIT_MINUTE)
                        {
                        	// do nothing
                        }
                        LastActionTime = HAL_GetTick();
                    } else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_1) {
                        Bike_SysData.Display.Metter.Trip_1 = 0.0;
                        Metter_Trip_1_100Ms = 0;
                    } else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_2) {
                        Bike_SysData.Display.Metter.Trip_2 = 0.0;
                        Metter_Trip_2_100Ms = 0;
                    }
                }
            }
        } else {
            if (OldBtnState == BUTTON_PRESSED) {
                if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_REMAIN) {
                    if (ClockEditState == CLOCK_EDIT_NONE) {
                        Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_TRIP_1;
                        Bike_SysData.Display.Setting.IsUnitKm = !Bike_SysData.Display.Setting.IsUnitKm;
                    }
                } else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_1) {
                    Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_TRIP_2;
                } else if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_TRIP_2) {
                    Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_REMAIN;
                }
            }
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
            OldBtnState = BUTTON_IDE;
            IsExtPressButton = FALSE;
        }
    }
}


#ifdef __cplusplus
}
#endif
