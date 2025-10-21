/*
 * Vmu_Msg.h
 *
 *  Created on: Aug 15, 2025
 */

#ifndef VMU_MSG_VMU_MSG_H_
#define VMU_MSG_VMU_MSG_H_

#if defined(__cplusplus)
extern "C"{
#endif

/*==================================================================================================
 *                                          INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 ==================================================================================================*/
#include "Main.h"
#include "Common.h"
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
#ifdef VMUF
#define VMUF_OUTPUT_ON  HAL_GPIO_WritePin(OUTPUT_CONTROL_GPIO_Port, OUTPUT_CONTROL_Pin, GPIO_PIN_SET)
#define VUMF_OUTPUT_OFF  HAL_GPIO_WritePin(OUTPUT_CONTROL_GPIO_Port, OUTPUT_CONTROL_Pin, GPIO_PIN_RESET)
#endif
/*==================================================================================================
 *                                              ENUMS
 ==================================================================================================*/
typedef enum
{
    //ESC
    CAN_ID_ESC_MGS_1 = 0x0101,
    CAN_ID_ESC_MGS_2 = 0x0102,
    CAN_ID_ESC_MGS_3 = 0x0103,

    //VMU_F
    CAN_ID_VMUF_LIGHT_MSG = 0x0081,

    //VMU_M
    CAN_ID_VMUM_BATTERY_INFO_MSG_1 = 0x0501,
    CAN_ID_VMUM_BATTERY_INFO_MSG_2 = 0x0502,
    CAN_ID_VMUM_BATTERY_FAULT_MSG = 0x0503,
    CAN_ID_VMUM_BATTERY_SWITCH_MSG = 0x0504,
    CAN_ID_VMUM_BATTERY_SOH_MSG = 0x0505,
    CAN_ID_VMUM_REMAIN_DISTANCE_MSG = 0x0506,
    CAN_ID_VMUM_TEMPERATURE_MSG = 0x0507,
    //CAN_ID_EXT_VMUM_BATTERY_CONTROL_MSG_BMS_1 = 0x18F0F428,
    //CAN_ID_EXT_VMUM_BATTERY_CONTROL_MSG_BMS_2 = 0x18F0F429,

    //BMS_1
    CAN_ID_BMS_BATTERY_STATUS_MSG_1 = 0x02F4,
    CAN_ID_BMS_BATTERY_CELL_TEMP_MSG_1 = 0x05F4,
    //CAN_ID_EXT_BMS_ERROR_INFO_MSG_1 = 0x18F328F4,
    //CAN_ID_EXT_BMS_SWITCH_STATUS_MSG_1 = 0x18F528F4,
    //BMS_2
    CAN_ID_BMS_BATTERY_STATUS_MSG_2 = 0x02F5,
    CAN_ID_BMS_BATTERY_CELL_TEMP_MSG_2 = 0x05F5,
    //CAN_ID_EXT_BMS_ERROR_INFO_MSG_2 = 0x18F328F5,
    //CAN_ID_EXT_BMS_SWITCH_STATUS_MSG_2 = 0x18F528F5,

    //CAN_ID_EXT_BMS_CONTROL_INFO_MSG = 0x18F0F428,

    //VMUD
    CAN_ID_VMUD_TIME_MSG = 0x0601,
} VmuMgs_eCanId_Type;

typedef enum
{
    VMUD_SCREEN_SPLASH, //
    VMUD_SCREEN_HOME,    //
    VMUD_SCREEN_SETTING,    //
    VMUD_SCREEN_CHARING,
} VmuMgs_eVmuDScreen_Type;

typedef enum
{
    VMUD_TRIP_SCREEN_REMAIN, //
    VMUD_TRIP_SCREEN_TRIP_1,    //
    VMUD_TRIP_SCREEN_TRIP_2,    //
} VmuMgs_eVmuDTripScreen_Type;

typedef enum
{
    VMU_NOT_CHARGE = 0,    //
    VMU_CHARGING = 1,
} VmuMgs_eCharge_Type;
/*==================================================================================================
 *                                  STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
typedef struct
{
    CAN_TxHeaderTypeDef Header;
    uint8_t Data[8];
    uint8_t CanIf; // 1 ~ CAN1, 2 ~ CAN2 for Display, 3 for both CAN1 & CAN2
} CanMsgTxType;

typedef struct
{
	CAN_RxHeaderTypeDef Header;
    uint8_t Data[8];
} CanMsgRxType;

/*
 ESC Message 1
 •   Message ID: 0x0101
 •   Broadcast Period: 100 ms
 •   Sender: ESC
 •   Receiver: VMU-F, VMU-M, Display Unit
 Field Name          Start bit   Length  Unit    Resolution  Offset  Description
 Speed               0           16      kph         1           0
 Motor current d     16          16      A           0.1         0   Current Id in Amperes
 Motor current q     32          16      A           0.1         0   Current Iq in Amperes
 Motor voltage       48          16      V           0.1         0   Voltage = sqrtf(Vd^2 + Vq^2)
 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t Speed :16;
        uint64_t MotoCurrentD :16;
        uint64_t MotoCurrentQ :16;
        uint64_t MotoVoltage :16;
    } Raw;
} VmuMgs_Esc_Msg_1_Type;

/*
 ESC Message 2
 •   Message ID: 0x0102
 •   Broadcast Period: 100 ms
 •   Sender: ESC
 •   Receiver: VMU-F, VMU-M, Display Unit

 Field Name      Start bit   Length  Unit    Resolution  Offset  Description
 Break status    0           8       -           -           -   Break signal for light control. 0  = OFF, 1 = ON
 Ready to go     8           8       -           -           -   After pull both front brake and rear brake, the vehicle is ready to go.
 0 = Not ready (N)
 1 = Read (D)
 Motor throttle  16          8       %           1           0   Throttle command value scaled.  Raw adc = Appx 700 – 3400 count
 Controller temp 32          16      °C          0.1         0   Temperature of the controller in °C (limited to 1 decimal place).
 Motor temp      48          16      °C          0.1         0   Temperature of the motor in °C (limited to 1 decimal place).
 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t BreakStatus :8;
        uint64_t ReadyToGo :8;
        uint64_t MotoThrottle :8;
        uint64_t :8;
        uint64_t ControllerTemp :16;
        uint64_t MotorTemp :16;
    } Raw;
} VmuMgs_Esc_Msg_2_Type;

/*
 ESC Message 3
 •   Message ID: 0x0103
 •   Broadcast Period: 100 ms
 •   Sender: ESC
 •   Receiver: VMU-F, VMU-M, Display Unit

 Field Name  Start bit   Length  Unit    Resolution  Offset  Description
 Error code  0           32      -       -           -       Bit field defined in ESC
 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t ErrorCode :32;
        uint64_t :32;
    } Raw;
} VmuMgs_Esc_Msg_3_Type;

/*
 VMU-F light Message
 •   Message ID: 0x0481
 •   Broadcast Period: none
 •   Sender: VMU-F
 •   Receiver: VMU-M, Display unit, ESC
 Field Name          Start bit   Length  Unit    Resolution  Offset  Description
 Turn light status   0           2         -     -           -       00 = OFF
 ------------------------------------------------------------------- 01 = left light ON
 ------------------------------------------------------------------- 10 = right light ON

 --Brake light status
 (ESC sends brake status)    8   2               00 = OFF
 ------------------------------------------------------------------- 01 = ON
 Head light status   16  2                                           00 = OFF
 ------------------------------------------------------------------- 01 = low beam ON
 ------------------------------------------------------------------- 10 = high beam ON
 Fog light status    24  2                                           00 = OFF
 ------------------------------------------------------------------- 01 = ON
 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t TurnLightStatus :2;
        uint64_t :6;
        uint64_t :8;
        uint64_t HeadLightStatus :2;
        uint64_t :6;
        uint64_t FogLightStatus :2;
        uint64_t :6;
        uint64_t :32;
    } Raw;
} VmuMgs_VmuF_Light_Msg_Type;

/*
 VMU-M Battery 1 Info Message
 •   Message ID: 0x0501
 •   Broadcast Period: 100 ms
 •   Sender: VMU-M
 •   Receiver: VMU-F, Display Unit

 Field Name                  Start bit   Length  Unit    Resolution  Offset  Description
 Voltage bms1                0           16      V       0.1             0   Battery voltage in Volts.
 Current bms1                16          16      A       0.1         -400    Current in Amperes
 SOC bms1                    32          8       %       1           0       State of Charge
 Max cell temperature bms 1  40          8       °C      1           -50     Maximum cell temperature
 Min cell temperature bms 1  48          8       °C      1           -50     Minimum cell temperature
 Avrg cell temp bms1         56          8       °C      1   -       50      Average cell temperature
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t VoltageBms :16;
        uint64_t CurrentBms :16;
        uint64_t SocBms :8;
        uint64_t MaxCellTempertureBms :8;
        uint64_t MinCellTempertureBms :8;
        uint64_t AvrgCellTempertureBms :8;
    } Raw;
} VmuMgs_VmuM_BatteryInfo_Msg_Type;

/*VMU-M Battery 1&2 fault Message
 •   Message ID: 0x0503
 •   Broadcast Period: 100 ms
 •   Sender: VMU-M
 •   Receiver: VMU-F, Display Unit

 Field Name  Start bit   Length  Unit    Resolution  Offset  Description
 Fault bms1  0           17      -           -           -   See BMS 1 BMSERR_INFO Message
 Fault bms2  32          17      -           -           -   See BMS 2 BMSERR_INFO Message
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t FaultBms1 :17;
        uint64_t :15;
        uint64_t FaultBms2 :17;
        uint64_t :15;
    } Raw;
} VmuMgs_VmuM_BatteryFault_Msg_Type;

/*
 VMU-M Battery 1&2 switch status Message
 •   Message ID: 0x0504
 •   Broadcast Period: 500 ms
 •   Sender: VMU-M
 •   Receiver: VMU-F, Display Unit

 Field Name          Start bit   Length  Unit    Resolution  Offset  Description
 Switch status bms1  0           5       -           -           -   See message 0x18F528F4
 Switch status bms2  8           5       -           -           -   See message 0x18F528F5
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        //bms_1
        uint64_t ChgMosSta_1 :1;
        uint64_t DChgMosSta_1 :1;
        uint64_t BalanSta_1 :1;
        uint64_t HeatSta_1 :1;
        uint64_t ChgDevPlugSta_1 :1;
        uint64_t AccStatus_1 :1;
        uint64_t :2;

        //bms_2
        uint64_t ChgMosSta_2 :1;
        uint64_t DChgMosSta_2 :1;
        uint64_t BalanSta_2 :1;
        uint64_t HeatSta_2 :1;
        uint64_t ChgDevPlugSta_2 :1;
        uint64_t AccStatus_2 :1;
        uint64_t :2;

        //Charing
        uint64_t ChargeStatus :1;
        uint64_t :47;
    } Raw;
} VmuMgs_VmuM_BatterySwitchStatus_Msg_Type;

/*
 VMU-M Battery 1&2 control information Message
 •   Message ID: 0x18F0F428 (BMS1) 0x18F0F429 (BMS2)
 •   Broadcast Period:
 •   Sender: VMU-M
 •   Receiver: BMS1&2

 Field Name          Start bit   Length  Unit    Resolution  Offset  Description
 Control info bms1   0           32      -       -           -   See message 0x18F0F428
 Control info bms2   32          32      -       -           -
 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t ControlInfoBms1 :32;
        uint64_t ControlInfoBms2 :32;
    } Raw;
} VmuMgs_VmuM_BatteryControlInfo_Msg_Type;

/*
 VMU-M Battery SOH Message
 •   Message ID: 0x0505
 •   Broadcast Period: 500 ms
 •   Sender: VMU-M
 •   Receiver: VMU-F, Display Unit, ESC

 Field Name  Start bit   Length  Unit    Resolution  Offset  Description
 SOH bms1    0              8       -       -           -       State of Health
 SOH bms2    8              8       -       -           -       State of Health
 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t Bms1 :8;
        uint64_t Bms2 :8;
        uint64_t :48;
    } Raw;
} VmuMgs_VmuM_BatterySoh_Msg_Type;

/*
 VMU-M remaining distance information Message
 •   Message ID: 0x506
 •   Broadcast Period: 30s
 •   Sender: VMU-M
 •   Receiver: Display unit
 Field Name      Start bit   Length  Unit    Resolution  Offset  Description
 Remain distance 0           32      km      0           0       Instant value calculated from BMS & ESC parameters
 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
        float RemainDistance;
        uint64_t :32;
    } Raw;
} VmuMgs_VmuM_RemainDistance_Msg_Type;

/*
 VMU-M temperature Message
 •   Message ID: 0x507
 •   Broadcast Period: 500ms
 •   Sender: VMU-M
 •   Receiver: Display unit
 Field Name  Start bit   Length  Unit    Resolution  Offset  Description
 VMU_temp    0           32      °C      0           0       Internal temperature float value

 */
typedef union
{
    uint8_t bytes[8];
    struct
    {
    	float Temperature;
        uint64_t :32;
    } Raw;
} VmuMgs_VmuM_Temperature_Msg_Type;

/*
 BMS 1 Status Battery Message
 •   Message ID: 0x02F4
 •   Cycle: 20 ms
 •   Sender: BMS
 •   Receiver: VMU-M

 Field Name      Start bit   Length  Unit    Resolution  Offset  Description
 Voltage bms1    0           16      V       0.1         0       Battery voltage in Volts.
 Current bms1    16          16      A       0.1         -400    Current in Amperes
 SOC bms1        32          8       %       1           0       State of Charge
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t VoltageBms :16;
        uint64_t CurrentBms :16;
        uint64_t SocBms :8;
        uint64_t :24;
    } Raw;
} VmuMgs_Bms_BatteryStatus_Msg_Type;

/*
 BMS 1 Cell Temp Message
 •   Message ID: 0x05F4
 •   Cycle: 500 ms
 •   Sender: BMS
 •   Receiver: VMU-M

 Field Name                  Start bit   Length  Unit    Resolution  Offset  Description
 Max cell temperature bms 1  0           8       °C          1       -50     Maximum cell temperature
 Max Cell No                 8           8       -           1       1       Highest temperature position
 Min cell temperature bms 1  16          8       °C          1       -50     Minimum cell temperature
 Min Cell No                 24          8       -           1       1       Lowest temperature position
 Avrg cell temp bms1         32          8       °C          1       -50     Average cell temperature
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t MaxCellTemperatureBms :8;
        uint64_t MaxCellNo :8;
        uint64_t MinCellTemperatureBms :8;
        uint64_t MinCellNo :8;
        uint64_t AvrgCellTemperatureBms :8;
        uint64_t :24;
    } Raw;
} VmuMgs_Bms_CellTemperature_Msg_Type;

/*
 BMS 1 BMSERR_INFO Message
 •   Message ID: 0x18F328F4
 •   Cycle: 100 ms
 •   Sender: BMS
 •   Receiver: VMU-M

 Field Name                              Start bit   Length  Unit    Resolution  Offset  Description
 Excessive line resistance               0   1   -   -   -   0: Normal 1: Fault
 MOS overtemperature                     1   1   -   -   -
 Monomer quantity does not match         2   1   -   -   -
 Abnormal current sensor                 3   1   -   -   -
 Monomer overpressure                    4   1   -   -   -
 Battery total pressure overvoltage      5   1   -   -   -
 Charging overcurrent                    6   1   -   -   -
 Charging short circuit                  7   1   -   -   -
 Charging temperature too high           8   1   -   -   -
 Charging temperature too low            9   1   -   -   -
 BMS internal communication exception    10  1   -   -   -
 Monomer undervoltage                    11  1   -   -   -
 Battery total voltage undervoltage      12  1   -   -   -
 Discharge overcurrent                   13  1   -   -   -
 Discharge short circuit protection      14  1   -   -   -
 Discharge temperature too high          15  1   -   -   -
 MOS fault charging                      16  1   -   -   -
 MOS fault discharging                   17  1   -   -   -
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t ExcessiveLineResistance :1;
        uint64_t MosOverTemperature :1;
        uint64_t MonomerQuantityNotMatch :1;
        uint64_t AbnormalCurrentSensor :1;
        uint64_t MonomerOverPressure :1;
        uint64_t BatteryTotalPressureOvervoltage :1;
        uint64_t ChargingOverCurrent :1;
        uint64_t ChargingShortCircuit :1;
        uint64_t ChargingTemperatureTooHigh :1;
        uint64_t ChargingTemperatureTooLow :1;
        uint64_t BmsInternalCommunicationException :1;
        uint64_t MonomerUnderVoltage :1;
        uint64_t BatteryTotalVoltageUndervoltage :1;
        uint64_t DischargeOverCurrent :1;
        uint64_t DischargeShortCircuitProtection :1;
        uint64_t DischargeTemperatureTooHigh :1;
        uint64_t MosFaultCharging :1;
        uint64_t MosFaultDischarging :1;
        uint64_t :46;
    } Raw;
} VmuMgs_Bms_ErrorInfo_Msg_Type;

/*
 BMS 1 Switch status Message
 •   Message ID: 0x18F528F4
 •   Cycle: 500 ms
 •   Sender: BMS
 •   Receiver: VMU-M
 Field Name      Start bit   Length  Unit    Resolution  Offset  Description
 ChgMosSta       0           1       -       -           -       Charging MOS
 --------------------------------------------------------------- 0: Open  1: Close

 DChgMosSta      1           1       -       -           -       Discharging MOS
 --------------------------------------------------------------- 0: Open  1: Close

 BalanSta        2           1       -       -           -       Equilibrium status
 --------------------------------------------------------------- 0: Unbalanced     1: Blalanced

 HeatSta         3           1       -       -           -       Heat MOS status
 --------------------------------------------------------------- 0: Open  1: Close

 ChgDevPlugSta   4           1       -       -           -      Charger status
 --------------------------------------------------------------- 0: Open  1: Close

 ACC status      5           1       -       -           -       ACC status
 --------------------------------------------------------------- 0: Off     1: On
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t ChgMosSta :1;
        uint64_t DchgMosSta :1;
        uint64_t BalanSta :1;
        uint64_t HeatSta :1;
        uint64_t ChgDevPlugSta :1;
        uint64_t AccStatus :1;
        uint64_t :58;
    } Raw;
} VmuMgs_Bms_SwitchStatus_Msg_Type;

/*
 BMS Control information Message
 •   Message ID: 0x18F0F428
 •   Cycle:
 •   Sender: VMU-M
 •   Receiver: BMS

 Field Name  Start bit   Length  Unit    Resolution  Offset  Description
 MaskCode    0           8       -           -           -
 ------------------------------------------------------------ 0: Control forbidden
 ------------------------------------------------------------ 1: Control allowed
 ------------------------------------------------------------ Bit 0: charge control
 ------------------------------------------------------------ Bit 1: discharge control
 ------------------------------------------------------------ Bit 2: balance control

 ChgSw       8           8       -           -           -   Charging switch
 ------------------------------------------------------------ 0: Off
 ------------------------------------------------------------ 1: On

 DchgSw      16          8       -           -           -   Discharging switch
 ------------------------------------------------------------ 0: Off
 ------------------------------------------------------------ 1: On

 BalanSw     24          8       -           -           -   Equalizing switch
 ------------------------------------------------------------ 0: Off
 ------------------------------------------------------------ 1: On
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t MaskCode :8;
        uint64_t ChgSw :8;
        uint64_t DchgSw :8;
        uint64_t BalanSw :8;
        uint64_t :32;
    } Raw;
} VmuMgs_Bms_ControlInfo_Msg_Type;

/*
 Display unit time setting Message
 •   Message ID: 0x0601
 •   Broadcast Period: 1000 ms
 •   Sender: Display unit
 •   Receiver: VMU-F, VMU-M, ESC


 Field Name      Start bit   Length  Unit    Resolution  Offset  Description
 Year setting    0           7   -   1       2000
 Month setting   8           4   -   1       0
 Day setting     16          5   -   1       0
 Hour setting    24          5   -   1       0
 Minute setting  32          6   -   1       0
 */

typedef union
{
    uint8_t bytes[8];
    struct
    {
        uint64_t Year :7;
        uint64_t :1;
        uint64_t Month :4;
        uint64_t :4;
        uint64_t Date :5;
        uint64_t :3;
        uint64_t Hour :5;
        uint64_t :3;
        uint64_t Minute :6;
        uint64_t :2;
        uint64_t Second :6;
        uint64_t :2;
        uint64_t AMPM :16;
    } Raw;
} VmuMgs_VmuD_Time_Msg_Type;

typedef struct
{
    VmuMgs_Esc_Msg_1_Type Esc_1;
    VmuMgs_Esc_Msg_2_Type Esc_2;
    VmuMgs_Esc_Msg_3_Type Esc_3;
} Esc_Msg_Type;

typedef struct
{
    VmuMgs_VmuM_BatteryInfo_Msg_Type Batt_1;
    VmuMgs_VmuM_BatteryInfo_Msg_Type Batt_2;
    VmuMgs_VmuM_BatteryFault_Msg_Type Fault;
    VmuMgs_VmuM_BatterySwitchStatus_Msg_Type Switch;
    VmuMgs_VmuM_Temperature_Msg_Type Temperature;
    VmuMgs_VmuM_BatterySoh_Msg_Type Soh;
    VmuMgs_VmuM_RemainDistance_Msg_Type RemainDistance;
//VmuMgs_VmuM_BatteryControlInfo_Msg_Type Control;
} VmuM_Msg_Type;

typedef struct
{
    VmuMgs_Bms_BatteryStatus_Msg_Type Batt;
    VmuMgs_Bms_CellTemperature_Msg_Type Cell;
    VmuMgs_Bms_ErrorInfo_Msg_Type Error;
    VmuMgs_Bms_SwitchStatus_Msg_Type Switch;
    VmuMgs_Bms_ControlInfo_Msg_Type Control;
} Bms_Part_Msg_Type;

typedef struct
{
    Bms_Part_Msg_Type Bms_1;
    Bms_Part_Msg_Type Bms_2;
    VmuMgs_Bms_ControlInfo_Msg_Type Control;
} Bms_Msg_Type;

typedef struct
{
    float Odo;
    float Trip_1;
    float Trip_2;
    float Remain;
} Display_MetterType;

typedef struct
{
	boolean IsModeAM;
	boolean IsMode24;
	boolean IsUnitKm;
} Display_SettingType;

typedef struct
{
	Display_SettingType Setting;
    boolean IsCharing;
    VmuMgs_VmuD_Time_Msg_Type Time;
    VmuMgs_eVmuDScreen_Type ScreenNow;
    VmuMgs_eVmuDTripScreen_Type TripScreenNow;
    Display_MetterType Metter;
} Display_Msg_Type;

typedef struct
{
    VmuMgs_VmuF_Light_Msg_Type Light;
    float Temperature;
} VmuF_Msg_Type;

typedef struct
{
    boolean IsTime12H;
    boolean IsMile;
    boolean IsDegree;
} Bike_Setting_Type;

typedef struct
{
    Bms_Msg_Type Bms;
    Esc_Msg_Type Esc;
    VmuM_Msg_Type VmuM;
    VmuF_Msg_Type VmuF;
    Display_Msg_Type Display;
    Bike_Setting_Type Setting;
} Bike_SysDataType;
/*==================================================================================================
 *                                  GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
extern Bike_SysDataType Bike_SysData;
/*==================================================================================================
 *                                       FUNCTION PROTOTYPES
 ==================================================================================================*/
boolean VmuMsg_GetData_Esc_Msg_1(uint8_t *Buf, VmuMgs_Esc_Msg_1_Type *Data);
boolean VmuMsg_GetData_Esc_Msg_2(uint8_t *Buf, VmuMgs_Esc_Msg_2_Type *Data);
boolean VmuMsg_GetData_Esc_Msg_3(uint8_t *Buf, VmuMgs_Esc_Msg_3_Type *Data);
boolean VmuMsg_GetData_Light_Msg(uint8_t *Buf, VmuMgs_VmuF_Light_Msg_Type *Data);
boolean VmuMsg_GetData_BatteryInfo_Msg(uint8_t *Buf, VmuMgs_VmuM_BatteryInfo_Msg_Type *Data);
boolean VmuMsg_GetData_BatteryFault_Msg(uint8_t *Buf, VmuMgs_VmuM_BatteryFault_Msg_Type *Data);
boolean VmuMsg_GetData_BatterySwitchStatus_Msg(uint8_t *Buf, VmuMgs_VmuM_BatterySwitchStatus_Msg_Type *Data);
boolean VmuMsg_GetData_BatteryControlInfo_Msg(uint8_t *Buf, VmuMgs_VmuM_BatteryControlInfo_Msg_Type *Data);
boolean VmuMsg_GetData_BatteryStatus_Msg(uint8_t *Buf, VmuMgs_Bms_BatteryStatus_Msg_Type *Data);
boolean VmuMsg_GetData_CellTemperature_Msg(uint8_t *Buf, VmuMgs_Bms_CellTemperature_Msg_Type *Data);
boolean VmuMsg_GetData_ErrorInfo_Msg(uint8_t *Buf, VmuMgs_Bms_ErrorInfo_Msg_Type *Data);
boolean VmuMsg_GetData_SwitchStatus_Msg(uint8_t *Buf, VmuMgs_Bms_SwitchStatus_Msg_Type *Data);
boolean VmuMsg_GetData_ControlInfo_Msg(uint8_t *Buf, VmuMgs_Bms_ControlInfo_Msg_Type *Data);
boolean VmuMsg_GetData_Time_Msg(uint8_t *Buf, VmuMgs_VmuD_Time_Msg_Type *Data);
boolean VmuMsg_GetData_BatterySoh_Msg(uint8_t *Buf, VmuMgs_VmuM_BatterySoh_Msg_Type *Data);
boolean VmuMsg_GetData_RemainDistance_Msg(uint8_t *Buf, VmuMgs_VmuM_RemainDistance_Msg_Type *Data);
boolean VmuMsg_GetData_VmuM_Temperature_Msg(uint8_t *Buf, VmuMgs_VmuM_Temperature_Msg_Type *Data);
void VmuMsg_UpdateSysTemData(CanMsgRxType *CanMsg);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* VMU_MSG_VMU_MSG_H_ */
