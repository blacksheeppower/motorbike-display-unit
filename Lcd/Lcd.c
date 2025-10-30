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
#include <stdlib.h>
#include <math.h>
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
	CLOCK_EDIT_DAY,
	CLOCK_EDIT_MONTH,
	CLOCK_EDIT_YEAR,
	CLOCK_HOUR_FORMAT,
	CLOCK_MODE_FORMAT,
} ClockEditState_e;

static ClockEditState_e ClockEditState = CLOCK_EDIT_NONE;
static u32 lastBlink = 0;
static bool blinkState = false;
static uint32_t LastActionTime = 0;
static bool skipAMPM = false;
static uint8_t icon_on = 0;
static bool icon_on_done = false;
static bool head_light_first_on = false;
//static bool charge_first_on = false;
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
bool IsTimeEditting = false;
extern bool IsOdoReset;
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
static void Lcd_clockStateMachine(void)
{
	if (HAL_GetTick() - LastActionTime > 5000)
	{
		switch (ClockEditState)
		{
			case CLOCK_MODE_FORMAT:
				if (skipAMPM)
				{
					ClockEditState = CLOCK_EDIT_HOUR;
				}
				else
				{
					ClockEditState = CLOCK_HOUR_FORMAT;
				}
				LastActionTime = HAL_GetTick();
				break;

			case CLOCK_HOUR_FORMAT:
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
				break;

			case CLOCK_EDIT_HOUR:
				ClockEditState = CLOCK_EDIT_MINUTE;
				LastActionTime = HAL_GetTick();
				break;

			case CLOCK_EDIT_MINUTE:
				ClockEditState = CLOCK_EDIT_NONE;
				Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_TRIP_1;
				lastBlink = HAL_GetTick();
				blinkState = false;
				IsTimeEditting = false;
				break;

			case CLOCK_EDIT_DAY:
				ClockEditState = CLOCK_EDIT_MONTH;
				LastActionTime = HAL_GetTick();
				break;

			case CLOCK_EDIT_MONTH:
				ClockEditState = CLOCK_EDIT_YEAR;
				LastActionTime = HAL_GetTick();
				break;

			case CLOCK_EDIT_YEAR:
				ClockEditState = CLOCK_EDIT_NONE;
				lastBlink = HAL_GetTick();
				blinkState = false;
				IsTimeEditting = false;
				break;

			default: break;
		}
	}
}

// Helper function to handle clock editing on quick press
static void HandleClockEditQuickPress(void) {
    if (Bike_SysData.Display.TripScreenNow != VMUD_TRIP_SCREEN_REMAIN) return;

    switch (ClockEditState)
    {
        case CLOCK_EDIT_NONE:
            // Do nothing
            break;
        case CLOCK_MODE_FORMAT:
            Bike_SysData.Display.Setting.IsMode24 = !Bike_SysData.Display.Setting.IsMode24;
            skipAMPM = Bike_SysData.Display.Setting.IsMode24 ? true : false;
            break;
        case CLOCK_HOUR_FORMAT:
            Bike_SysData.Display.Setting.IsModeAM = !Bike_SysData.Display.Setting.IsModeAM;
            RTC_SetTimeDate();
            break;
        case CLOCK_EDIT_HOUR:
            if (Bike_SysData.Display.Setting.IsMode24)
            {
                Bike_SysData.Display.Time.Raw.Hour = (Bike_SysData.Display.Time.Raw.Hour + 1) % 24;
            }
            else
            {
                Bike_SysData.Display.Time.Raw.Hour++;
                if (Bike_SysData.Display.Time.Raw.Hour > 12U)
                {
                    Bike_SysData.Display.Time.Raw.Hour = 1U;
                }
            }
            RTC_SetTimeDate();
            break;
        case CLOCK_EDIT_MINUTE:
            Bike_SysData.Display.Time.Raw.Minute = (Bike_SysData.Display.Time.Raw.Minute + 1) % 60;
            RTC_SetTimeDate();
            break;
        case CLOCK_EDIT_DAY: {
            uint8_t max_days;
            if (Bike_SysData.Display.Time.Raw.Month == 2) {
                max_days = 29; // February (leap year not handled)
            }
            else if (Bike_SysData.Display.Time.Raw.Month % 2 == 1 && Bike_SysData.Display.Time.Raw.Month < 8)
            {
                max_days = 31;
            }
            else if (Bike_SysData.Display.Time.Raw.Month % 2 == 1 && Bike_SysData.Display.Time.Raw.Month >= 8)
            {
                max_days = 30;
            }
            else if (Bike_SysData.Display.Time.Raw.Month % 2 == 0 && Bike_SysData.Display.Time.Raw.Month < 8)
            {
                max_days = 30;
            }
            else
            {
                max_days = 31;
            }
            Bike_SysData.Display.Time.Raw.Date = (Bike_SysData.Display.Time.Raw.Date % max_days) + 1;
            RTC_SetTimeDate();
            break;
        }
        case CLOCK_EDIT_MONTH:
            Bike_SysData.Display.Time.Raw.Month = (Bike_SysData.Display.Time.Raw.Month % 12) + 1;
            RTC_SetTimeDate();
            break;
        case CLOCK_EDIT_YEAR:
            Bike_SysData.Display.Time.Raw.Year = (Bike_SysData.Display.Time.Raw.Year + 1) % 100;
            RTC_SetTimeDate();
            break;
    }
    LastActionTime = HAL_GetTick();
}

// Helper function to handle long press actions
static void HandleLongPress(void) {
    if (IsOdoReset == TRUE) {
        if (Bike_SysData.Display.TripScreenNow == VMUD_TRIP_SCREEN_REMAIN)
        {
            Bike_SysData.Display.Metter.Odo = 0;
            Lcd_WriteMetter(Bike_SysData.Display.Metter);
        }
        else
        {
            if (ClockEditState == CLOCK_EDIT_NONE)
            {
                IsTimeEditting = true;
                ClockEditState = CLOCK_EDIT_DAY;
                lastBlink = HAL_GetTick();
                blinkState = true;
            }
            LastActionTime = HAL_GetTick();
        }
    }
    else
    {
    	switch (Bike_SysData.Display.TripScreenNow)
    	{
			case VMUD_TRIP_SCREEN_REMAIN:
				if (ClockEditState == CLOCK_EDIT_NONE)
				{
					ClockEditState = CLOCK_MODE_FORMAT;
					lastBlink = HAL_GetTick();
					blinkState = true;
					IsTimeEditting = true;
				} // No state transitions for other clock edit states
				LastActionTime = HAL_GetTick();
				break;
			case VMUD_TRIP_SCREEN_TRIP_1:
				Bike_SysData.Display.Metter.Trip_1 = 0.0;
				Metter_Trip_1_100Ms = 0;
				break;
			case VMUD_TRIP_SCREEN_TRIP_2:
				Bike_SysData.Display.Metter.Trip_2 = 0.0;
				Metter_Trip_2_100Ms = 0;
				break;
			default: break;
    	}
    }
}

// Helper function to handle button release
static void HandleButtonRelease(void) {
	switch (Bike_SysData.Display.TripScreenNow)
	{
		case VMUD_TRIP_SCREEN_REMAIN:
			if (ClockEditState == CLOCK_EDIT_NONE)
			{
				Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_TRIP_1;
				Bike_SysData.Display.Setting.IsUnitKm = !Bike_SysData.Display.Setting.IsUnitKm;
			}
			break;
		case VMUD_TRIP_SCREEN_TRIP_1:
			Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_TRIP_2;
			break;
		case VMUD_TRIP_SCREEN_TRIP_2:
			Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_REMAIN;
			break;
		default: break;
	}
}
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
	Lcd_clockStateMachine();

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
	}

	if (HAL_GetTick() - lastBlink >= 500)
	{
	    lastBlink = HAL_GetTick();
	    blinkState = !blinkState;

	    switch (ClockEditState)
	    {
	        case CLOCK_MODE_FORMAT:
	            if (blinkState)
	                sprintf(m, "Mode %s", Bike_SysData.Display.Setting.IsMode24 ? "24" : "12");
	            else
	                sprintf(m, "  ");
	            break;

	        case CLOCK_HOUR_FORMAT:
	            if (blinkState)
	                sprintf(m, "%s", Bike_SysData.Display.Setting.IsModeAM ? "AM" : "PM");
	            else
	                sprintf(m, "  ");
	            break;

	        case CLOCK_EDIT_HOUR:
	            if (blinkState)
	                sprintf(m, "  :%02d", Bike_SysData.Display.Time.Raw.Minute);
	            else
	                sprintf(m, "%02d:%02d", Bike_SysData.Display.Time.Raw.Hour, Bike_SysData.Display.Time.Raw.Minute);
	            break;

	        case CLOCK_EDIT_MINUTE:
	            if (blinkState)
	                sprintf(m, "%02d:  ", Bike_SysData.Display.Time.Raw.Hour);
	            else
	                sprintf(m, "%02d:%02d", Bike_SysData.Display.Time.Raw.Hour, Bike_SysData.Display.Time.Raw.Minute);
	            break;

	        case CLOCK_EDIT_DAY:
	            if (blinkState)
	                sprintf(m, "  - %02d - %02d", Bike_SysData.Display.Time.Raw.Month, Bike_SysData.Display.Time.Raw.Year);
	            else
	                sprintf(m, "%02d - %02d - %02d",
	                        Bike_SysData.Display.Time.Raw.Date,
	                        Bike_SysData.Display.Time.Raw.Month,
	                        Bike_SysData.Display.Time.Raw.Year);
	            break;

	        case CLOCK_EDIT_MONTH:
	            if (blinkState)
	                sprintf(m, "%02d -   - %02d", Bike_SysData.Display.Time.Raw.Date, Bike_SysData.Display.Time.Raw.Year);
	            else
	                sprintf(m, "%02d - %02d - %02d",
	                        Bike_SysData.Display.Time.Raw.Date,
	                        Bike_SysData.Display.Time.Raw.Month,
	                        Bike_SysData.Display.Time.Raw.Year);
	            break;

	        case CLOCK_EDIT_YEAR:
	            if (blinkState)
	                sprintf(m, "%02d - %02d -  ",
	                        Bike_SysData.Display.Time.Raw.Date,
	                        Bike_SysData.Display.Time.Raw.Month);
	            else
	                sprintf(m, "%02d - %02d - %02d",
	                        Bike_SysData.Display.Time.Raw.Date,
	                        Bike_SysData.Display.Time.Raw.Month,
	                        Bike_SysData.Display.Time.Raw.Year);
	            break;

	        default:
	            // Normal clock display
	            if (Bike_SysData.Display.Setting.IsMode24)
	                sprintf(m, "%02d:%02d", Bike_SysData.Display.Time.Raw.Hour, Bike_SysData.Display.Time.Raw.Minute);
	            else
	                sprintf(m, "%02d:%02d %s",
	                        Bike_SysData.Display.Time.Raw.Hour,
	                        Bike_SysData.Display.Time.Raw.Minute,
	                        Bike_SysData.Display.Setting.IsModeAM ? "AM" : "PM");
	            break;
	    }

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
				lv_obj_clear_flag(ui_ImgChargeStatus, LV_OBJ_FLAG_HIDDEN);
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
				lv_obj_add_flag(ui_ImgChargeStatus, LV_OBJ_FLAG_HIDDEN);
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
		uint16_t  temp_speed = 0;
		//odo with other task calc
		if (!Bike_SysData.Display.Setting.IsUnitKm)
		{
			temp_odo = Bike_SysData.Display.Metter.Odo * 0.621371;
		}
		else
		{
			temp_odo = Bike_SysData.Display.Metter.Odo;
		}


		if (fabs(temp_odo - Old_Bike_SysData.Display.Metter.Odo) >= 0.1)
		{
			sprintf(m, "%.1f", temp_odo / 1.0);
			lv_label_set_text(ui_lbOdoValue, m);
		}

		Old_Bike_SysData.Display.Metter.Odo = temp_odo;
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

			if (fabs(temp_trip1 - Old_Bike_SysData.Display.Metter.Trip_1) >= 0.1)
			{
				sprintf(m, "%.1f", temp_trip1 / 1.0);
				lv_label_set_text(ui_lbTripValue, m);
			}

			Old_Bike_SysData.Display.Metter.Trip_1 = temp_trip1;

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
			if (fabs(temp_trip1 - Old_Bike_SysData.Display.Metter.Trip_2) >= 0.1)
			{
				sprintf(m, "%.1f", temp_trip2 / 1.0);
				lv_label_set_text(ui_lbTripValue, m);
			}

			Old_Bike_SysData.Display.Metter.Trip_2 = temp_trip2;
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
		if (abs(Bike_SysData.Esc.Esc_1.Raw.Speed - Old_Bike_SysData.Esc.Esc_1.Raw.Speed) >= 1)
		{
			Old_Bike_SysData.Esc.Esc_1.Raw.Speed = Bike_SysData.Esc.Esc_1.Raw.Speed;
			temp_speed =  Bike_SysData.Esc.Esc_1.Raw.Speed;

            if (!Bike_SysData.Display.Setting.IsUnitKm)
            {
            	temp_speed = (uint16_t)(temp_speed * 0.621371f);
            }

			sprintf(m, "%u", temp_speed);
			lv_label_set_text(ui_lbSpeedValue, m);
			lv_arc_set_value(ui_SpeedMetter, temp_speed);
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
				lv_obj_clear_flag(ui_imgWarning, LV_OBJ_FLAG_HIDDEN);
			}
			else
			{
				lv_obj_add_flag(ui_imgWarning, LV_OBJ_FLAG_HIDDEN);
			}

		}
//		if ((Bike_SysData.Esc.Esc_3.Raw.ErrorCode != 0 || Bike_SysData.VmuM.Fault.Raw.FaultBms1 != 0 || Bike_SysData.VmuM.Fault.Raw.FaultBms1 != 0)
//				&& (Bike_SysData.VmuM.Switch.Raw.AccStatus_1 == 1) && (Bike_SysData.VmuM.Switch.Raw.AccStatus_2 == 1))
//		{
//			lv_obj_clear_flag(ui_imgWarning, LV_OBJ_FLAG_HIDDEN);
//		}
//		else
//		{
//			lv_obj_add_flag(ui_imgWarning, LV_OBJ_FLAG_HIDDEN);
//		}

		//turn LED
//		if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus != Bike_SysData.VmuF.Light.Raw.TurnLightStatus)
//		{
//			Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus = Bike_SysData.VmuF.Light.Raw.TurnLightStatus;
//			if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 0)
//			{
//				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_off_png);
//				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_off_png);
//			}
//			else if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 1)
//			{
//				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_on_png);
//				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_off_png);
//			}
//			else if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 2)
//			{
//				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_off_png);
//				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_on_png);
//			}
//			else if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus == 3)
//			{
//				lv_img_set_src(ui_imgXinNhanLeft, &ui_img_left_on_png);
//				lv_img_set_src(ui_imgXinNhanRight, &ui_img_right_on_png);
//			}
//			lv_obj_invalidate(ui_imgXinNhanLeft);
//			lv_obj_invalidate(ui_imgXinNhanRight);
//		}

		if (Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus != Bike_SysData.VmuF.Light.Raw.TurnLightStatus)
		{
			Old_Bike_SysData.VmuF.Light.Raw.TurnLightStatus = Bike_SysData.VmuF.Light.Raw.TurnLightStatus;
			switch (Bike_SysData.VmuF.Light.Raw.TurnLightStatus)
			{
				case 0:
					lv_obj_add_flag(ui_imgXinNhanRight, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(ui_imgXinNhanLeft, LV_OBJ_FLAG_HIDDEN);
					break;

				case 1: // Xi-nhan trái
					lv_obj_clear_flag(ui_imgXinNhanLeft, LV_OBJ_FLAG_HIDDEN);
					break;

				case 2: // Xi-nhan phải
					lv_obj_clear_flag(ui_imgXinNhanRight, LV_OBJ_FLAG_HIDDEN);
					break;

				case 3: // Cả hai
					lv_obj_clear_flag(ui_imgXinNhanLeft, LV_OBJ_FLAG_HIDDEN);
					lv_obj_clear_flag(ui_imgXinNhanRight, LV_OBJ_FLAG_HIDDEN);
					break;

				default:
					break;
			}
		}

		//fog light
		if (Old_Bike_SysData.VmuF.Light.Raw.FogLightStatus != Bike_SysData.VmuF.Light.Raw.FogLightStatus)
		{
			Old_Bike_SysData.VmuF.Light.Raw.FogLightStatus = Bike_SysData.VmuF.Light.Raw.FogLightStatus;
		}

		//Head Light
		if (Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus != Bike_SysData.VmuF.Light.Raw.HeadLightStatus) {
		    if (Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 0) {
		    	//lv_img_set_src(ui_imgCosPha, &ui_img_cos_off_png);
		    	if(icon_on >= 30)
		        lv_obj_add_flag(ui_imgCosPha, LV_OBJ_FLAG_HIDDEN);
		    } else if (Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 1) {
		        lv_img_set_src(ui_imgCosPha, &ui_img_cos_on_png);
		        lv_obj_clear_flag(ui_imgCosPha, LV_OBJ_FLAG_HIDDEN);
		    } else if (Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 2 || Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 3) {
		        lv_img_set_src(ui_imgCosPha, &ui_img_pha_on_png);
		        lv_obj_clear_flag(ui_imgCosPha, LV_OBJ_FLAG_HIDDEN);
		    }
		    //head_light_first_on = true;
		    Old_Bike_SysData.VmuF.Light.Raw.HeadLightStatus = Bike_SysData.VmuF.Light.Raw.HeadLightStatus;
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
//		if (Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp != Bike_SysData.Esc.Esc_2.Raw.ControllerTemp ||    //
//			Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp != Bike_SysData.Esc.Esc_2.Raw.MotorTemp ||
//			Old_Bike_SysData.VmuM.Temperature.Raw.Temperature != Bike_SysData.VmuM.Temperature.Raw.Temperature)
//		{
//			Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp = Bike_SysData.Esc.Esc_2.Raw.ControllerTemp;
//			Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp = Bike_SysData.Esc.Esc_2.Raw.MotorTemp;
//			float temp_temperature = 0;
//			memcpy(&temp_temperature, &Bike_SysData.VmuM.Temperature, sizeof(float));
//			Old_Bike_SysData.VmuM.Temperature.Raw.Temperature = Bike_SysData.VmuM.Temperature.Raw.Temperature;
//
//			if ((Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp - 273) > 60
//					|| (Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp - 273) > 60
//					|| temp_temperature > 60)
//			{
//				lv_img_set_src(ui_imgTemOver, &ui_img_over_tem_on_png);
//			}
//			else
//			{
//				lv_img_set_src(ui_imgTemOver, &ui_img_over_tem_off_png);
//			}
//		}

		if (Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp != Bike_SysData.Esc.Esc_2.Raw.ControllerTemp ||    //
			Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp != Bike_SysData.Esc.Esc_2.Raw.MotorTemp ||
			Old_Bike_SysData.VmuM.Temperature.Raw.Temperature != Bike_SysData.VmuM.Temperature.Raw.Temperature)
		{
			Old_Bike_SysData.Esc.Esc_2.Raw.ControllerTemp = Bike_SysData.Esc.Esc_2.Raw.ControllerTemp;
			Old_Bike_SysData.Esc.Esc_2.Raw.MotorTemp = Bike_SysData.Esc.Esc_2.Raw.MotorTemp;
			Old_Bike_SysData.VmuM.Temperature.Raw.Temperature = Bike_SysData.VmuM.Temperature.Raw.Temperature;
			float temp_temperature = 0;
			memcpy(&temp_temperature, &Bike_SysData.VmuM.Temperature, sizeof(float));
			if ((Bike_SysData.Esc.Esc_2.Raw.ControllerTemp - 273) > 60
					|| (Bike_SysData.Esc.Esc_2.Raw.MotorTemp - 273) > 60
					|| temp_temperature > 60)
			{
				lv_obj_clear_flag(ui_imgTemOver, LV_OBJ_FLAG_HIDDEN);
			}
			else
			{
				lv_obj_add_flag(ui_imgTemOver, LV_OBJ_FLAG_HIDDEN);

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

	if (icon_on < 30)
	{
		icon_on++;
	}
	else if (icon_on_done == false)
	{
		lv_obj_add_flag(ui_imgWarning, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(ui_imgTemOver, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(ui_imgXinNhanRight, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(ui_imgXinNhanLeft, LV_OBJ_FLAG_HIDDEN);
		if (Bike_SysData.VmuF.Light.Raw.HeadLightStatus == 0)
		{
			lv_obj_add_flag(ui_imgCosPha, LV_OBJ_FLAG_HIDDEN);
		}
		if (Bike_SysData.VmuM.Switch.Raw.ChargeStatus != VMU_CHARGING)
		{
			lv_obj_add_flag(ui_ImgChargeStatus, LV_OBJ_FLAG_HIDDEN);
		}
		icon_on_done = true;
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
    static uint32_t Stick = 0;
    static Button_eState OldBtnState = BUTTON_IDE;

    if (IsExtPressButton != TRUE) return;

    if (LCD_BUTTON_READ_INPUT == STD_LOW) {
        switch (OldBtnState) {
            case BUTTON_IDE:
                OldBtnState = BUTTON_DEBOUND;
                Stick = HAL_GetTick();
                break;
            case BUTTON_DEBOUND:
                if (HAL_GetTick() - Stick > 100) {
                    OldBtnState = BUTTON_PRESSED;
                    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
                    HandleClockEditQuickPress();
                }
                break;
            case BUTTON_PRESSED:
                if (HAL_GetTick() - Stick > 1500) {
                    OldBtnState = BUTTON_HOLD;
                    HandleLongPress();
                }
                break;
            case BUTTON_HOLD:
                // Do nothing, wait for release
                break;
        }
    } else {
        if (OldBtnState == BUTTON_PRESSED) {
            HandleButtonRelease();
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
            OldBtnState = BUTTON_IDE;
            IsExtPressButton = FALSE;
        } else if (OldBtnState == BUTTON_HOLD) {
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
            OldBtnState = BUTTON_IDE;
            IsExtPressButton = FALSE;
        }
    }
}


#ifdef __cplusplus
}
#endif
