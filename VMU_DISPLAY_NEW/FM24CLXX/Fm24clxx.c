/*
 * Fm24clxx.c
 *
 *  Created on: Sep 18, 2025
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
#include "Fm24clxx.h"
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
static Fm24Clxx_ConfigType FRAM_Devices[FM24CLXX_MAX_SUPPORT] = {{.I2cAddress = 0xA0}, };
/*==================================================================================================
 *                                    LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/

/*==================================================================================================
 *                                         LOCAL VARIABLES
 ==================================================================================================*/

/*==================================================================================================
 *                                         LOCAL FUNCTIONS
 ==================================================================================================*/

/**
 * @brief
 * @param 
 * @return
 * @details
 */

/*==================================================================================================
 *                                         GLOBAL FUNCTIONS
 ==================================================================================================*/
void Fm24clxx_Init(void)
{

}

Std_ReturnType Fm24clxx_Write(u8 CtrlIdx, u16 Address, const u8 *Buff, u16 Length)
{
    if (CtrlIdx > FM24CLXX_MAX_SUPPORT) return FALSE;
    if (Address > FM24CLXX_MAX_ADDR) return FALSE;
    if ( (Address + Length) > FM24CLXX_MAX_ADDR) return FALSE;

    uint8_t buf[Length + 2];
    buf[0] = (uint8_t) (Address >> 8);
    buf[1] = (uint8_t) (Address & 0xFF);
    memcpy( &buf[2], Buff, Length);
    return HAL_I2C_Master_Transmit( &hi2c1, FRAM_Devices[CtrlIdx].I2cAddress, buf, Length + 2, HAL_MAX_DELAY);
}

Std_ReturnType Fm24clxx_Read(u8 CtrlIdx, u16 Address, u8 *Buff, u16 Length)
{
    if (CtrlIdx > FM24CLXX_MAX_SUPPORT) return FALSE;
    if (Address > FM24CLXX_MAX_ADDR) return FALSE;
    if ( (Address + Length) > FM24CLXX_MAX_ADDR) return FALSE;

    uint8_t addr[2];
    addr[0] = (uint8_t) (Address >> 8);
    addr[1] = (uint8_t) (Address & 0xFF);

    if (HAL_I2C_Master_Transmit( &hi2c1, FRAM_Devices[CtrlIdx].I2cAddress, addr, 2, HAL_MAX_DELAY) == HAL_OK)
    {
        return HAL_I2C_Master_Receive( &hi2c1, FRAM_Devices[CtrlIdx].I2cAddress, Buff, Length, HAL_MAX_DELAY);
    }
    else return FALSE;
}

Std_ReturnType Fm24clxx_Esrase(u8 CtrlIdx, u16 Address, u16 Length)
{
    if (CtrlIdx > FM24CLXX_MAX_SUPPORT) return FALSE;
    if (Address > FM24CLXX_MAX_ADDR) return FALSE;
    if ( (Address + Length) > FM24CLXX_MAX_ADDR) return FALSE;

    uint8_t buf[Length + 2];
    buf[0] = (uint8_t) (Address >> 8);
    buf[1] = (uint8_t) (Address & 0xFF);
    for(u16 i = 0; i < Length; i++)
    {
        buf[i] = 0xFF;
    }
    return HAL_I2C_Master_Transmit( &hi2c1, FRAM_Devices[CtrlIdx].I2cAddress, buf, Length + 2, HAL_MAX_DELAY);
}

#ifdef __cplusplus
}
#endif
