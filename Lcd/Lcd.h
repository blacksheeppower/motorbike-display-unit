/*
 * Lcd.h
 *
 *  Created on: Sep 16, 2025
 */

#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#if defined(__cplusplus)
extern "C"{
#endif

/*==================================================================================================
 *                                          INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 ==================================================================================================*/
#include "ui.h"
#include "Common.h"
#include "Vmu_Msg.h"
/*==================================================================================================
 *                               SOURCE FILE VERSION INFORMATION
 ==================================================================================================*/

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 ==================================================================================================*/

/*==================================================================================================
 *                                            CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                       DEFINES AND MACROS
 ==================================================================================================*/
#define LCD_BUTTON_READ_INPUT HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)
/*==================================================================================================
 *                                              ENUMS
 ==================================================================================================*/

/*==================================================================================================
 *                                  STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
typedef enum
{
    BUTTON_IDE, BUTTON_DEBOUND, BUTTON_PRESSED, BUTTON_HOLD
} Button_eState;

typedef enum
{
    METTER_TRIP_1, METTER_TRIP_2, METTER_REMAIN
} TripScreen_eState;
/*==================================================================================================
 *                                  GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
extern boolean IsExtPressButton;
/*==================================================================================================
 *                                       FUNCTION PROTOTYPES
 ==================================================================================================*/
void Lcd_InitFirstTime(void);
void Lcd_UpdateUi(void);
Std_ReturnType Lcd_WriteMetter(Display_MetterType Metter);
Std_ReturnType Lcd_ReadMetter(Display_MetterType *Metter);
void Lcd_LoadMetter(void);
void Lcd_CalcMetter(void);
void Lcd_GetButton(void);
Std_ReturnType Lcd_WriteSetting(Display_SettingType Setting);
Std_ReturnType Lcd_ReadSetting(Display_SettingType *Setting);
void Lcd_LoadSetting(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* LCD_LCD_H_ */
