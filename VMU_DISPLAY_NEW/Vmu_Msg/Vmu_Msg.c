/*
 * Vmu_Msg.c
 *
 *  Created on: Aug 15, 2025
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
#include "Vmu_Msg.h"
#include <string.h>
/*==================================================================================================
 SOURCE FILE VERSION INFORMATION
 ==================================================================================================*/

/*==================================================================================================
 *                                     FILE VERSION CHECKS
 ==================================================================================================*/

/*==================================================================================================
 *                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/

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
Bike_SysDataType Bike_SysData = {0};
/*==================================================================================================
 *                                    LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/

/*==================================================================================================
 *                                         LOCAL VARIABLES
 ==================================================================================================*/

/*==================================================================================================
 *                                         LOCAL FUNCTIONS
 ==================================================================================================*/

/*==================================================================================================
 *                                         GLOBAL FUNCTIONS
 ==================================================================================================*/

boolean VmuMsg_GetData_Esc_Msg_1(uint8_t *Buf, VmuMgs_Esc_Msg_1_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }

    memcpy(Data, Buf, sizeof(VmuMgs_Esc_Msg_1_Type));
    return TRUE;
}

boolean VmuMsg_GetData_Esc_Msg_2(uint8_t *Buf, VmuMgs_Esc_Msg_2_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_Esc_Msg_2_Type));
    return TRUE;
}

boolean VmuMsg_GetData_Esc_Msg_3(uint8_t *Buf, VmuMgs_Esc_Msg_3_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_Esc_Msg_3_Type));
    return TRUE;
}

boolean VmuMsg_GetData_Light_Msg(uint8_t *Buf, VmuMgs_VmuF_Light_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuF_Light_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_BatteryInfo_Msg(uint8_t *Buf, VmuMgs_VmuM_BatteryInfo_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuM_BatteryInfo_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_BatteryFault_Msg(uint8_t *Buf, VmuMgs_VmuM_BatteryFault_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuM_BatteryFault_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_BatterySwitchStatus_Msg(uint8_t *Buf, VmuMgs_VmuM_BatterySwitchStatus_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuM_BatterySwitchStatus_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_BatterySoh_Msg(uint8_t *Buf, VmuMgs_VmuM_BatterySoh_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuM_BatterySoh_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_RemainDistance_Msg(uint8_t *Buf, VmuMgs_VmuM_RemainDistance_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuM_RemainDistance_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_VmuM_Temperature_Msg(uint8_t *Buf, VmuMgs_VmuM_Temperature_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuM_Temperature_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_BatteryStatus_Msg(uint8_t *Buf, VmuMgs_Bms_BatteryStatus_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_Bms_BatteryStatus_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_CellTemperature_Msg(uint8_t *Buf, VmuMgs_Bms_CellTemperature_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_Bms_CellTemperature_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_ErrorInfo_Msg(uint8_t *Buf, VmuMgs_Bms_ErrorInfo_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_Bms_ErrorInfo_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_SwitchStatus_Msg(uint8_t *Buf, VmuMgs_Bms_SwitchStatus_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_Bms_SwitchStatus_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_ControlInfo_Msg(uint8_t *Buf, VmuMgs_Bms_ControlInfo_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_Bms_ControlInfo_Msg_Type));
    return TRUE;
}

boolean VmuMsg_GetData_Time_Msg(uint8_t *Buf, VmuMgs_VmuD_Time_Msg_Type *Data)
{
    if (Buf == NULL_PTR || Data == NULL_PTR)
    {
        return FALSE;
    }
    memcpy(Data, Buf, sizeof(VmuMgs_VmuD_Time_Msg_Type));
    return TRUE;
}

void VmuMsg_UpdateSysTemData(CanMsgRxType *CanMsg)
{
    if (CanMsg->Header.IDE == CAN_ID_STD)
    {
        switch(CanMsg->Header.StdId)
        {
            case CAN_ID_ESC_MGS_1:
            {
                VmuMsg_GetData_Esc_Msg_1(CanMsg->Data, &Bike_SysData.Esc.Esc_1);
                break;
            }
            case CAN_ID_ESC_MGS_2:
            {
                VmuMsg_GetData_Esc_Msg_2(CanMsg->Data, &Bike_SysData.Esc.Esc_2);
                break;
            }
            case CAN_ID_ESC_MGS_3:
            {
                VmuMsg_GetData_Esc_Msg_3(CanMsg->Data, &Bike_SysData.Esc.Esc_3);
                break;
            }

            case CAN_ID_VMUF_LIGHT_MSG:
            {
                VmuMsg_GetData_Light_Msg(CanMsg->Data, &Bike_SysData.VmuF.Light);
                break;
            }

            case CAN_ID_VMUM_BATTERY_INFO_MSG_1:
            {
                VmuMsg_GetData_BatteryInfo_Msg(CanMsg->Data, &Bike_SysData.VmuM.Batt_1);
                break;
            }

            case CAN_ID_VMUM_BATTERY_INFO_MSG_2:
            {
                VmuMsg_GetData_BatteryInfo_Msg(CanMsg->Data, &Bike_SysData.VmuM.Batt_2);
                break;
            }

            case CAN_ID_VMUM_BATTERY_FAULT_MSG:
            {
                VmuMsg_GetData_BatteryFault_Msg(CanMsg->Data, &Bike_SysData.VmuM.Fault);
                break;
            }

            case CAN_ID_VMUM_BATTERY_SWITCH_MSG:
            {
                VmuMsg_GetData_BatterySwitchStatus_Msg(CanMsg->Data, &Bike_SysData.VmuM.Switch);
#ifdef VMUF
                if ((Bike_SysData.VmuM.Switch.Raw.ChargeStatus == VMU_NOT_CHARGE) && (Bike_SysData.VmuM.Switch.Raw.AccStatus_1 == 1) && (Bike_SysData.VmuM.Switch.Raw.AccStatus_2 == 1))
                {
                    VMUF_OUTPUT_ON;
                }
                else
                {
                    VUMF_OUTPUT_OFF;
                }
#endif
                break;
            }

            case CAN_ID_VMUM_BATTERY_SOH_MSG:
            {
                VmuMsg_GetData_BatterySoh_Msg(CanMsg->Data, &Bike_SysData.VmuM.Soh);
                break;
            }

            case CAN_ID_VMUM_REMAIN_DISTANCE_MSG:
            {
                VmuMsg_GetData_RemainDistance_Msg(CanMsg->Data, &Bike_SysData.VmuM.RemainDistance);
                break;
            }

            case CAN_ID_VMUM_TEMPERATURE_MSG:
            {
                VmuMsg_GetData_VmuM_Temperature_Msg(CanMsg->Data, &Bike_SysData.VmuM.Temperature);
                break;
            }

            case CAN_ID_BMS_BATTERY_STATUS_MSG_1:
            {
                VmuMsg_GetData_BatteryStatus_Msg(CanMsg->Data, &Bike_SysData.Bms.Bms_1.Batt);
                break;
            }

            case CAN_ID_BMS_BATTERY_CELL_TEMP_MSG_1:
            {
                VmuMsg_GetData_CellTemperature_Msg(CanMsg->Data, &Bike_SysData.Bms.Bms_1.Cell);
                break;
            }

            case CAN_ID_BMS_BATTERY_STATUS_MSG_2:
            {
                VmuMsg_GetData_BatteryStatus_Msg(CanMsg->Data, &Bike_SysData.Bms.Bms_2.Batt);
                break;
            }

            case CAN_ID_BMS_BATTERY_CELL_TEMP_MSG_2:
            {
                VmuMsg_GetData_CellTemperature_Msg(CanMsg->Data, &Bike_SysData.Bms.Bms_2.Cell);
                break;
            }

            case CAN_ID_VMUD_TIME_MSG:
            {
                VmuMsg_GetData_Time_Msg(CanMsg->Data, &Bike_SysData.Display.Time);
                break;
            }

            default:
                break;
        }
    }
}

#ifdef __cplusplus
}
#endif
