/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "lvgl.h"
#include <stdbool.h>
#include "lv_examples.h"
#include "ui.h"
#include "Vmu_Msg.h"
//#include "Queue_Can.h"
#include "Lcd.h"
#include "Fm24clxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

DMA2D_HandleTypeDef hdma2d;

DSI_HandleTypeDef hdsi;

I2C_HandleTypeDef hi2c1;

LTDC_HandleTypeDef hltdc;

RTC_HandleTypeDef hrtc;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */
extern bool IsTimeEditting;
static void VmuD_SendTime(void)
{
    //prepare frame
	uint32_t txMailbox = 0;
    CanMsgTxType message = {.Header.RTR = CAN_RTR_DATA, .Header.IDE = CAN_ID_STD, .Header.DLC = 8, .Header.ExtId = 0};

    message.Header.StdId = 0x0601;

    if (Bike_SysData.Display.Setting.IsMode24)
    {
    	Bike_SysData.Display.Time.Raw.AMPM = 0;
    }
    else
    {
    	if (Bike_SysData.Display.Setting.IsModeAM)
    	{
    		Bike_SysData.Display.Time.Raw.AMPM = 1;
    	}
    	else
    	{
    		Bike_SysData.Display.Time.Raw.AMPM = 2;
    	}
    }

    for (int i = 0; i < 8; i++)
    {
        message.Data[i] = Bike_SysData.Display.Time.bytes[i];
    }

    // VMUF is logging data and creating files so we dont want to send time data
    if (IsTimeEditting == false)
    {
		HAL_CAN_AbortTxRequest(&hcan1, txMailbox);
		HAL_CAN_AddTxMessage( &hcan1, &message.Header, message.Data, &txMailbox);
    }
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
static void MX_DSIHOST_DSI_Init(void);
static void MX_LTDC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_CAN1_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void RTC_GetTimeDate(void);
void RTC_SetTimeDate(void);
static FMC_SDRAM_CommandTypeDef Command;
/**
 * @brief  FMC SDRAM Mode definition register defines
 */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)/**
 * @}
 */

/** @defgroup STM32469I-Discovery_SDRAM_Exported_Constants STM32469I Discovery SDRAM Exported Constants
 * @{
 */
#define SDRAM_DEVICE_ADDR  ((uint32_t)0xD0000000)

/* SDRAM device size in Bytes */
#define SDRAM_DEVICE_SIZE  ((uint32_t)0x1000000)

#define SDRAM_MEMORY_WIDTH FMC_SDRAM_MEM_BUS_WIDTH_16
#define SDCLOCK_PERIOD     FMC_SDRAM_CLOCK_PERIOD_2

/* SDRAM refresh counter (90 MHz SD clock) */
#define REFRESH_COUNT       ((uint32_t)0x02AB)
#define  SDRAM_TIMEOUT      ((uint32_t)0xFFFF)

/**
 * @brief  SDRAM status structure definition
 */
#define   SDRAM_OK         ((uint8_t)0x00)
#define   SDRAM_ERROR      ((uint8_t)0x01)

/**
 * @brief  Programs the SDRAM device.
 * @param  RefreshCount: SDRAM refresh counter value
 */
void BSP_SDRAM_Initialization_sequence(uint32_t RefreshCount)
{
    __IO uint32_t tmpmrd = 0;

    /* Step 1: Configure a clock configuration enable command */
    Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand( &hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 2: Insert 100 us minimum delay */
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    HAL_Delay(1);

    /* Step 3: Configure a PALL (precharge all) command */
    Command.CommandMode = FMC_SDRAM_CMD_PALL;
    Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand( &hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 4: Configure an Auto Refresh command */
    Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber = 8;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand( &hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 5: Program the external memory mode register */
    tmpmrd = (uint32_t) SDRAM_MODEREG_BURST_LENGTH_1 |\
 SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |\
 SDRAM_MODEREG_CAS_LATENCY_3 |\
 SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
 SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    Command.AutoRefreshNumber = 1;
    Command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand( &hsdram1, &Command, SDRAM_TIMEOUT);

    /* Step 6: Set the refresh rate counter */
    /* Set the device refresh rate */
    HAL_SDRAM_ProgramRefreshRate( &hsdram1, RefreshCount);
}

/**
 * @brief  Reads an mount of data from the SDRAM memory in polling mode.
 * @param  uwStartAddress: Read start address
 * @param  pData: Pointer to data to be read
 * @param  uwDataSize: Size of read data from the memory
 * @retval SDRAM status : SDRAM_OK or SDRAM_ERROR.
 */
uint8_t BSP_SDRAM_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
    if (HAL_SDRAM_Read_32b( &hsdram1, (uint32_t*) uwStartAddress, pData, uwDataSize) != HAL_OK)
    {
        return SDRAM_ERROR;
    }
    else
    {
        return SDRAM_OK;
    }
}

/**
 * @brief  Writes an mount of data to the SDRAM memory in polling mode.
 * @param  uwStartAddress: Write start address
 * @param  pData: Pointer to data to be written
 * @param  uwDataSize: Size of written data from the memory
 * @retval SDRAM status : SDRAM_OK or SDRAM_ERROR.
 */
uint8_t BSP_SDRAM_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
    if (HAL_SDRAM_Write_32b( &hsdram1, (uint32_t*) uwStartAddress, pData, uwDataSize) != HAL_OK)
    {
        return SDRAM_ERROR;
    }
    else
    {
        return SDRAM_OK;
    }
}
uint8_t state_SDRAM = 0;
uint32_t buf_read[10] = {0}, buf_write[10] = {0};

/////////////////////////////////////////////////////////////////////LCD
LTDC_HandleTypeDef hltdc_eval;
static DSI_VidCfgTypeDef hdsivideo_handle;
DSI_HandleTypeDef hdsi_eval;
/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
typedef enum
{
    LCD_ORIENTATION_PORTRAIT = 0x00, /*!< Portrait orientation choice of LCD screen  */
    LCD_ORIENTATION_LANDSCAPE = 0x01, /*!< Landscape orientation choice of LCD screen */
    LCD_ORIENTATION_INVALID = 0x02 /*!< Invalid orientation choice of LCD screen   */
} LCD_OrientationTypeDef;
/**
 * @brief  LCD status structure definition
 */
#define   LCD_OK         0x00
#define   LCD_ERROR      0x01
#define   LCD_TIMEOUT    0x02
#define LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef
/**
 * @brief  LCD FB_StartAddress
 */
#define LCD_FB_START_ADDRESS       ((uint32_t)0xD0000000)

/** @brief Maximum number of LTDC layers
 */
#define LTDC_MAX_LAYER_NUMBER             ((uint32_t) 2)

/** @brief LTDC Background layer index
 */
#define LTDC_ACTIVE_LAYER_BACKGROUND      ((uint32_t) 0)

/** @brief LTDC Foreground layer index
 */
#define LTDC_ACTIVE_LAYER_FOREGROUND      ((uint32_t) 1)

/** @brief Number of LTDC layers
 */
#define LTDC_NB_OF_LAYERS                 ((uint32_t) 2)

/** @brief LTDC Default used layer index
 */
#define LTDC_DEFAULT_ACTIVE_LAYER         LTDC_ACTIVE_LAYER_FOREGROUND

/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define NT35510_ORIENTATION_PORTRAIT    ((uint32_t)0x00) /* Portrait orientation choice of LCD screen  */
#define NT35510_ORIENTATION_LANDSCAPE   ((uint32_t)0x01) /* Landscape orientation choice of LCD screen */

/**
 *  @brief  Possible values of
 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
 */
#define NT35510_FORMAT_RGB888    ((uint32_t)0x00) /* Pixel format chosen is RGB888 : 24 bpp */
#define NT35510_FORMAT_RBG565    ((uint32_t)0x02) /* Pixel format chosen is RGB565 : 16 bpp */

/**
 * @brief  nt35510_480x800 Size
 */

/* Width and Height in Portrait mode */
#define  NT35510_480X800_WIDTH             ((uint16_t)480)     /* LCD PIXEL WIDTH   */
#define  NT35510_480X800_HEIGHT            ((uint16_t)480)     /* LCD PIXEL HEIGHT  */

/* Width and Height in Landscape mode */
#define  NT35510_800X480_WIDTH             ((uint16_t)480)     /* LCD PIXEL WIDTH   */
#define  NT35510_800X480_HEIGHT            ((uint16_t)480)     /* LCD PIXEL HEIGHT  */

/**
 * @brief  NT35510_480X800 Timing parameters for Portrait orientation mode
 */
#define  NT35510_480X800_HSYNC             ((uint16_t)30)      /* Horizontal synchronization */
#define  NT35510_480X800_HBP               ((uint16_t)30)     /* Horizontal back porch      */
#define  NT35510_480X800_HFP               ((uint16_t)30)     /* Horizontal front porch     */

#define  NT35510_480X800_VSYNC             ((uint16_t)5)      /* Vertical synchronization   */
#define  NT35510_480X800_VBP               ((uint16_t)16)     /* Vertical back porch        */
#define  NT35510_480X800_VFP               ((uint16_t)6)     /* Vertical front porch       */

/**
 * @brief  NT35510_800X480 Timing parameters for Landscape orientation mode
 *         Same values as for Portrait mode in fact.
 */
#define  NT35510_800X480_HSYNC             NT35510_480X800_VSYNC  /* Horizontal synchronization */
#define  NT35510_800X480_HBP               NT35510_480X800_VBP    /* Horizontal back porch      */
#define  NT35510_800X480_HFP               NT35510_480X800_VFP    /* Horizontal front porch     */
#define  NT35510_800X480_VSYNC             NT35510_480X800_HSYNC  /* Vertical synchronization   */
#define  NT35510_800X480_VBP               NT35510_480X800_HBP    /* Vertical back porch        */
#define  NT35510_800X480_VFP               NT35510_480X800_HFP    /* Vertical front porch       */

/* List of NT35510 used commands                                  */
/* Detailed in NT35510 Data Sheet v0.80                           */
/* Version of 10/28/2011                                          */
/* Command, NumberOfArguments                                     */

#define  NT35510_CMD_NOP                   0x00  /* NOP */
#define  NT35510_CMD_SWRESET               0x01  /* SW reset */
#define  NT35510_CMD_RDDID                 0x04  /* Read display ID */
#define  NT35510_CMD_RDNUMED               0x05  /* Read number of errors on DSI */
#define  NT35510_CMD_RDDPM                 0x0A  /* Read display power mode */
#define  NT35510_CMD_RDDMADCTL             0x0B  /* Read display MADCTL */
#define  NT35510_CMD_RDDCOLMOD             0x0C  /* Read display pixel format */
#define  NT35510_CMD_RDDIM                 0x0D  /* Read display image mode */
#define  NT35510_CMD_RDDSM                 0x0E  /* Read display signal mode */
#define  NT35510_CMD_RDDSDR                0x0F  /* Read display self-diagnostics result */
#define  NT35510_CMD_SLPIN                 0x10  /* Sleep in */
#define  NT35510_CMD_SLPOUT                0x11  /* Sleep out */
#define  NT35510_CMD_PTLON                 0x12  /* Partial mode on  */
#define  NT35510_CMD_NORON                 0x13  /* Normal display mode on */
#define  NT35510_CMD_INVOFF                0x20  /* Display inversion off */
#define  NT35510_CMD_INVON                 0x21  /* Display inversion on */
#define  NT35510_CMD_ALLPOFF               0x22  /* All pixel off */
#define  NT35510_CMD_ALLPON                0x23  /* All pixel on */
#define  NT35510_CMD_GAMSET                0x26  /* Gamma set */
#define  NT35510_CMD_DISPOFF               0x28  /* Display off */
#define  NT35510_CMD_DISPON                0x29  /* Display on */
#define  NT35510_CMD_CASET                 0x2A  /* Column address set */
#define  NT35510_CMD_RASET                 0x2B  /* Row address set */
#define  NT35510_CMD_RAMWR                 0x2C  /* Memory write */
#define  NT35510_CMD_RAMRD                 0x2E  /* Memory read  */
#define  NT35510_CMD_PLTAR                 0x30  /* Partial area */
#define  NT35510_CMD_TOPC                  0x32  /* Turn On Peripheral Command */
#define  NT35510_CMD_TEOFF                 0x34  /* Tearing effect line off */
#define  NT35510_CMD_TEEON                 0x35  /* Tearing effect line on */
#define  NT35510_CMD_MADCTL                0x36  /* Memory data access control */
#define  NT35510_CMD_IDMOFF                0x38  /* Idle mode off */
#define  NT35510_CMD_IDMON                 0x39  /* Idle mode on */
#define  NT35510_CMD_COLMOD                0x3A  /* Interface pixel format */
#define  NT35510_CMD_RAMWRC                0x3C  /* Memory write continue */
#define  NT35510_CMD_RAMRDC                0x3E  /* Memory read continue */
#define  NT35510_CMD_STESL                 0x44  /* Set tearing effect scan line */
#define  NT35510_CMD_GSL                   0x45  /* Get scan line */

#define  NT35510_CMD_DSTBON                0x4F  /* Deep standby mode on */
#define  NT35510_CMD_WRPFD                 0x50  /* Write profile value for display */
#define  NT35510_CMD_WRDISBV               0x51  /* Write display brightness */
#define  NT35510_CMD_RDDISBV               0x52  /* Read display brightness */
#define  NT35510_CMD_WRCTRLD               0x53  /* Write CTRL display */
#define  NT35510_CMD_RDCTRLD               0x54  /* Read CTRL display value */
#define  NT35510_CMD_WRCABC                0x55  /* Write content adaptative brightness control */
#define  NT35510_CMD_RDCABC                0x56  /* Read content adaptive brightness control */
#define  NT35510_CMD_WRHYSTE               0x57  /* Write hysteresis */
#define  NT35510_CMD_WRGAMMSET             0x58  /* Write gamme setting */
#define  NT35510_CMD_RDFSVM                0x5A  /* Read FS value MSBs */
#define  NT35510_CMD_RDFSVL                0x5B  /* Read FS value LSBs */
#define  NT35510_CMD_RDMFFSVM              0x5C  /* Read median filter FS value MSBs */
#define  NT35510_CMD_RDMFFSVL              0x5D  /* Read median filter FS value LSBs */
#define  NT35510_CMD_WRCABCMB              0x5E  /* Write CABC minimum brightness */
#define  NT35510_CMD_RDCABCMB              0x5F  /* Read CABC minimum brightness */
#define  NT35510_CMD_WRLSCC                0x65  /* Write light sensor compensation coefficient value */
#define  NT35510_CMD_RDLSCCM               0x66  /* Read light sensor compensation coefficient value MSBs */
#define  NT35510_CMD_RDLSCCL               0x67  /* Read light sensor compensation coefficient value LSBs */
#define  NT35510_CMD_RDBWLB                0x70  /* Read black/white low bits */
#define  NT35510_CMD_RDBKX                 0x71  /* Read Bkx */
#define  NT35510_CMD_RDBKY                 0x72  /* Read Bky */
#define  NT35510_CMD_RDWX                  0x73  /* Read Wx */
#define  NT35510_CMD_RDWY                  0x74  /* Read Wy */
#define  NT35510_CMD_RDRGLB                0x75  /* Read red/green low bits */
#define  NT35510_CMD_RDRX                  0x76  /* Read Rx */
#define  NT35510_CMD_RDRY                  0x77  /* Read Ry */
#define  NT35510_CMD_RDGX                  0x78  /* Read Gx */
#define  NT35510_CMD_RDGY                  0x79  /* Read Gy */
#define  NT35510_CMD_RDBALB                0x7A  /* Read blue/acolor low bits */
#define  NT35510_CMD_RDBX                  0x7B  /* Read Bx */
#define  NT35510_CMD_RDBY                  0x7C  /* Read By */
#define  NT35510_CMD_RDAX                  0x7D  /* Read Ax */
#define  NT35510_CMD_RDAY                  0x7E  /* Read Ay */
#define  NT35510_CMD_RDDDBS                0xA1  /* Read DDB start */
#define  NT35510_CMD_RDDDBC                0xA8  /* Read DDB continue */
#define  NT35510_CMD_RDDCS                 0xAA  /* Read first checksum */
#define  NT35510_CMD_RDCCS                 0xAF  /* Read continue checksum */
#define  NT35510_CMD_RDID1                 0xDA  /* Read ID1 value */
#define  NT35510_CMD_RDID2                 0xDB  /* Read ID2 value */
#define  NT35510_CMD_RDID3                 0xDC  /* Read ID3 value */

/* Parameter TELOM : Tearing Effect Line Output Mode : possible values */
#define NT35510_TEEON_TELOM_VBLANKING_INFO_ONLY            0x00
#define NT35510_TEEON_TELOM_VBLANKING_AND_HBLANKING_INFO   0x01

/* Possible used values of MADCTR */
#define NT35510_MADCTR_MODE_PORTRAIT       0x00
#define NT35510_MADCTR_MODE_LANDSCAPE      0x60  /* MY = 0, MX = 1, MV = 1, ML = 0, RGB = 0 */

/* Possible values of COLMOD parameter corresponding to used pixel formats */
#define  NT35510_COLMOD_RGB565             0x55
#define  NT35510_COLMOD_RGB888             0x77

/** @brief LCD_Driver_TypeDef
 *  Return the supported LCD Components type.
 */
typedef enum
{
    LCD_CTRL_NT35510, LCD_CTRL_OTM8009A, LCD_CTRL_NONE
} LCD_Driver_TypeDef;
#define NT35510_ID                 0x80
/**
 *  @brief  Possible values of
 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
 */

#define   LCD_DSI_PIXEL_DATA_FMT_RBG888  DSI_RGB888 /*!< DSI packet pixel format chosen is RGB888 : 24 bpp */
#define   LCD_DSI_PIXEL_DATA_FMT_RBG565  DSI_RGB565 /*!< DSI packet pixel format chosen is RGB565 : 16 bpp */

/**
 * @brief  LCD Display DSI Virtual Channel ID
 */
#define LCD_Driver_ID ((uint32_t) 0)
#define LAYER0_ADDRESS               (LCD_FB_START_ADDRESS)
/**
 * @brief  NT35510_480X800 frequency divider
 */
#define NT35510_480X800_FREQUENCY_DIVIDER  2   /* LCD Frequency divider      */

LCD_Driver_TypeDef Lcd_Driver_Type;

uint32_t lcd_x_size = 0;
uint32_t lcd_y_size = 0;

/**
 * @brief  DCS or Generic short/long write command
 * @param  NbrParams: Number of parameters. It indicates the write command mode:
 *                 If inferior to 2, a long write command is performed else short.
 * @param  pParams: Pointer to parameter values table.
 * @retval HAL status
 */
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
    if (NbrParams == 0)
    {
        HAL_DSI_ShortWrite( &hdsi_eval, LCD_Driver_ID, DSI_DCS_SHORT_PKT_WRITE_P0, pParams[0], pParams[1]);
    }
    else if (NbrParams == 1)
    {
        HAL_DSI_ShortWrite( &hdsi_eval, LCD_Driver_ID, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
    }
    else
    {
        HAL_DSI_LongWrite( &hdsi_eval, LCD_Driver_ID, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[0], &pParams[1]);
    }
}

/**
 * @brief  Generic read command
 * @param  Reg Register to be read
 * @param  pData pointer to a buffer to store the payload of a read back operation.
 * @param  Size  Data size to be read (in byte).
 * @retval BSP status
 */
int32_t DSI_IO_ReadCmd(uint32_t Reg, uint8_t *pData, uint32_t Size)
{
    int32_t ret = LCD_ERROR;

    if (HAL_DSI_Read( &hdsi_eval, LCD_Driver_ID, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) == HAL_OK)
    {
        ret = LCD_OK;
    }

    return ret;
}

/* Exported functions ---------------------------------------------------------*/
/** @defgroup NT35510_Exported_Functions OTM8009A Exported Functions
 * @{
 */

/**
 * @brief  Initializes the LCD Frida display part by communication in DSI mode in Video Mode
 *         with IC Display Driver NT35510 (see IC Driver specification for more information).
 * @param  hdsi_eval : pointer on DSI configuration structure
 * @param  hdsivideo_handle : pointer on DSI video mode configuration structure
 * @retval Status
 */
uint8_t NT35510_Init(uint32_t ColorCoding, uint32_t orientation)
{
    HAL_Delay(120);

    /* ************************************************************************** */
    /* Proprietary Initialization                                                 */
    /* ************************************************************************** */
    const uint8_t nt35510_reg1[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x13};
    const uint8_t nt35510_reg2[] = {0xEF, 0x08};
    const uint8_t nt35510_reg3[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x10};
    const uint8_t nt35510_reg4[] = {0xC0, 0x3B, 0x00};
    const uint8_t nt35510_reg5[] = {0xC1, 0x10, 0x0C};
    const uint8_t nt35510_reg6[] = {0xC2, 0x07, 0x0A};
    const uint8_t nt35510_reg7[] = {0xC7, 0x00};
    const uint8_t nt35510_reg8[] = {0xCC, 0x10};
    const uint8_t nt35510_reg9[] = {0xB0, 0x05, 0x12, 0x98, 0x0E, 0x0F, 0x07, 0x07, 0x09, 0x09, 0x23, 0x05, 0x52, 0x0F, 0x67, 0x2C, 0x11};
    const uint8_t nt35510_reg10[] = {0xB1, 0x0B, 0x11, 0x97, 0x0C, 0x12, 0x06, 0x06, 0x08, 0x08, 0x22, 0x03, 0x51, 0x11, 0x66, 0x2B, 0x0F};
    const uint8_t nt35510_reg11[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x11};
    const uint8_t nt35510_reg12[] = {0xB0, 0x5D};
    const uint8_t nt35510_reg13[] = {0xB1, 0x55}; //VCOM SET
    const uint8_t nt35510_reg14[] = {0xB2, 0x81}; //VGH SET
    const uint8_t nt35510_reg15[] = {0xB3, 0x80};
    const uint8_t nt35510_reg16[] = {0xB5, 0x4E}; //VGL SET
    const uint8_t nt35510_reg17[] = {0xB7, 0x85};
    const uint8_t nt35510_reg18[] = {0xB8, 0x20};
    const uint8_t nt35510_reg19[] = {0xC1, 0x78};
    const uint8_t nt35510_reg20[] = {0xC2, 0x78};
    const uint8_t nt35510_reg21[] = {0xD0, 0x88};
    const uint8_t nt35510_reg22[] = {0xE0, 0x00, 0x00, 0x02};
    const uint8_t nt35510_reg23[] = {0xE1, 0x06, 0x30, 0x08, 0x30, 0x05, 0x30, 0x07, 0x30, 0x00, 0x33, 0x33};
    const uint8_t nt35510_reg24[] = {0xE2, 0x11, 0x11, 0x33, 0x33, 0xF4, 0x00, 0x00, 0x00, 0xF4, 0x00, 0x00, 0x00};
    const uint8_t nt35510_reg25[] = {0xE3, 0x00, 0x00, 0x11, 0x11};
    const uint8_t nt35510_reg26[] = {0xE4, 0x44, 0x44};
    const uint8_t nt35510_reg27[] = {0xE5, 0x0D, 0xF5, 0x30, 0xF0, 0x0F, 0xF7, 0x30, 0xF0, 0x09, 0xF1, 0x30, 0xF0, 0x0B, 0xF3, 0x30, 0xF0};
    const uint8_t nt35510_reg28[] = {0xE6, 0x00, 0x00, 0x11, 0x11};
    const uint8_t nt35510_reg29[] = {0xE7, 0x44, 0x44};
    const uint8_t nt35510_reg30[] = {0xE8, 0x0C, 0xF4, 0x30, 0xF0, 0x0E, 0xF6, 0x30, 0xF0, 0x08, 0xF0, 0x30, 0xF0, 0x0A, 0xF2, 0x30, 0xF0};
    const uint8_t nt35510_reg31[] = {0xE9, 0x36, 0x01};
    const uint8_t nt35510_reg32[] = {0xEB, 0x00, 0x01, 0xE4, 0xE4, 0x44, 0x88, 0x40};
    const uint8_t nt35510_reg33[] = {0xED, 0xFF, 0x10, 0xBF, 0x76, 0x54, 0x2A, 0xFC, 0xFF, 0xFF, 0xCF, 0xA2, 0x45, 0x67, 0xFB, 0x01, 0xFF};
    const uint8_t nt35510_reg34[] = {0xEF, 0x08, 0x08, 0x08, 0x45, 0x3F, 0x54};
    const uint8_t nt35510_reg35[] = {0xFF, 0x77, 0x01, 0x00, 0x00, 0x00};
    const uint8_t nt35510_reg36[] = {0x3A, 0x55};
    const uint8_t nt35510_reg37[] = {0x36, 0x10};
    const uint8_t nt35510_reg38[] = {0x35, 0x00};
    const uint8_t nt35510_reg39[] = {0x11, 0};
    const uint8_t nt35510_reg40[] = {0x29, 0};

    DSI_IO_WriteCmd(5, (uint8_t*) nt35510_reg1); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg2); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(5, (uint8_t*) nt35510_reg3); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(2, (uint8_t*) nt35510_reg4); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(2, (uint8_t*) nt35510_reg5); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(2, (uint8_t*) nt35510_reg6); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg7); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg8); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(16, (uint8_t*) nt35510_reg9); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(16, (uint8_t*) nt35510_reg10); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(5, (uint8_t*) nt35510_reg11); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg12); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg13); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg14); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg15); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg16); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg17); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg18); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg19); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg20); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg21); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(3, (uint8_t*) nt35510_reg22); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(11, (uint8_t*) nt35510_reg23); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(12, (uint8_t*) nt35510_reg24); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(4, (uint8_t*) nt35510_reg25); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(2, (uint8_t*) nt35510_reg26); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(16, (uint8_t*) nt35510_reg27); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(4, (uint8_t*) nt35510_reg28); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(2, (uint8_t*) nt35510_reg29); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(16, (uint8_t*) nt35510_reg30); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(2, (uint8_t*) nt35510_reg31); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(7, (uint8_t*) nt35510_reg32); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(16, (uint8_t*) nt35510_reg33); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(6, (uint8_t*) nt35510_reg34); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(5, (uint8_t*) nt35510_reg35); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg36); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg37); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(1, (uint8_t*) nt35510_reg38); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(0, (uint8_t*) nt35510_reg39); /* LV2:  Page 1 enable */
    DSI_IO_WriteCmd(0, (uint8_t*) nt35510_reg40); /* LV2:  Page 1 enable */

    return 0;
}
/**
 * @brief  Read the component ID.
 * @retval Component ID
 */
uint16_t NT35510_ReadID(void)
{
    uint8_t pData = 0;
    DSI_IO_ReadCmd(NT35510_CMD_RDID2, &pData, 1);
    return pData;
}
/**
 * @brief  Check if the component ID is correct.
 * @param  Lcd_type Driver Type Control NT35510 or OTM8009A
 */
static LCD_Driver_TypeDef LCD_ReadType(LCD_Driver_TypeDef Lcd_type)
{
    uint16_t read_id;
    /* Read the NT35510 ID */
    read_id = NT35510_ReadID();
    if (read_id == NT35510_ID)
    {
        Lcd_type = LCD_CTRL_NT35510;
    }
    else
    {

    }

    return Lcd_type;
}

/**
 * @brief  Initializes the DSI LCD.
 * The ititialization is done as below:
 *     - DSI PLL ititialization
 *     - DSI ititialization
 *     - LTDC ititialization
 *     - OTM8009A LCD Display IC Driver ititialization
 * @retval LCD state
 */
uint8_t BSP_LCD_InitEx(LCD_OrientationTypeDef orientation)
{
    DSI_PLLInitTypeDef dsiPllInit;
    DSI_PHY_TimerTypeDef PhyTimings;
    static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    uint32_t LcdClock = 25000; /*!< LcdClk = 27429 kHz */

    uint32_t laneByteClk_kHz = 0;
    uint32_t VSA; /*!< Vertical start active time in units of lines */
    uint32_t VBP; /*!< Vertical Back Porch time in units of lines */
    uint32_t VFP; /*!< Vertical Front Porch time in units of lines */
    uint32_t VACT; /*!< Vertical Active time in units of lines = imageSize Y in pixels to display */
    uint32_t HSA; /*!< Horizontal start active time in units of lcdClk */
    uint32_t HBP; /*!< Horizontal Back Porch time in units of lcdClk */
    uint32_t HFP; /*!< Horizontal Front Porch time in units of lcdClk */
    uint32_t HACT; /*!< Horizontal Active time in units of lcdClk = imageSize X in pixels to display */

    /* Toggle Hardware Reset of the DSI LCD using
     * its XRES signal (active low) */
//  BSP_LCD_Reset();
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(20);

    /* Call first MSP Initialize only in case of first initialization
     * This will set IP blocks LTDC, DSI and DMA2D
     * - out of reset
     * - clocked
     * - NVIC IRQ related to IP blocks enabled
     */
//  BSP_LCD_MspInit();
    /*************************DSI Initialization***********************************/

    /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
    hdsi_eval.Instance = DSI;

    HAL_DSI_DeInit( & (hdsi_eval));

#if !defined(USE_STM32469I_DISCO_REVA)
    dsiPllInit.PLLNDIV = 125;
    dsiPllInit.PLLIDF = DSI_PLL_IN_DIV4;
    dsiPllInit.PLLODF = DSI_PLL_OUT_DIV1;
#else
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
#endif
    laneByteClk_kHz = 62500; /* 500 MHz / 8 = 62.5 MHz = 62500 kHz */

    /* Set number of Lanes */
    hdsi_eval.Init.NumberOfLanes = DSI_TWO_DATA_LANES;

    /* TXEscapeCkdiv = f(LaneByteClk)/15.62 = 4 */
    hdsi_eval.Init.TXEscapeCkdiv = laneByteClk_kHz / 15620;

    HAL_DSI_Init( & (hdsi_eval), & (dsiPllInit));

    /* Start DSI */
    HAL_DSI_Start( &hdsi_eval);
    /* Enable the DSI BTW for read operations */
    HAL_DSI_ConfigFlowControl( &hdsi_eval, DSI_FLOW_CONTROL_BTA);
//  Lcd_Driver_Type = LCD_ReadType(Lcd_Driver_Type);

    uint8_t pData[8];
    pData[0] = 0;
    pData[1] = 0;
    pData[2] = 0;
    pData[3] = 0;
    pData[4] = 0;
    pData[5] = 0;
    pData[6] = 0;
    pData[7] = 0;
    pData[8] = 0;

    DSI_IO_ReadCmd(0x0A, pData, 1);

    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(20);
    HAL_DSI_Stop( &hdsi_eval);
#if 1
    /* Timing parameters for all Video modes
     * Set Timing parameters of LTDC depending on its chosen orientation
     */
    if (orientation == LCD_ORIENTATION_PORTRAIT)
    {
        lcd_x_size = NT35510_480X800_WIDTH; /* 480 */
        lcd_y_size = NT35510_480X800_HEIGHT; /* 800 */
    }
    else
    {
        /* lcd_orientation == LCD_ORIENTATION_LANDSCAPE */
        lcd_x_size = NT35510_800X480_WIDTH; /* 800 */
        lcd_y_size = NT35510_800X480_HEIGHT; /* 480 */
    }

    HACT = lcd_x_size;
    VACT = lcd_y_size;

    /* The following values are same for portrait and landscape orientations */
    if (1)
    {
        VSA = NT35510_480X800_VSYNC;
        VBP = NT35510_480X800_VBP;
        VFP = NT35510_480X800_VFP;

        HSA = NT35510_480X800_HSYNC;
        HBP = NT35510_480X800_HBP;
        HFP = NT35510_480X800_HFP;
    }

    hdsivideo_handle.VirtualChannelID = LCD_Driver_ID;
    hdsivideo_handle.ColorCoding = LCD_DSI_PIXEL_DATA_FMT_RBG888;
    hdsivideo_handle.VSPolarity = DSI_VSYNC_ACTIVE_HIGH;
    hdsivideo_handle.HSPolarity = DSI_HSYNC_ACTIVE_HIGH;
    hdsivideo_handle.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
    hdsivideo_handle.Mode = DSI_VID_MODE_BURST; /* Mode Video burst ie : one LgP per line */
    hdsivideo_handle.NullPacketSize = 0xFFF;
    hdsivideo_handle.NumberOfChunks = 0;
    hdsivideo_handle.PacketSize = HACT; /* Value depending on display orientation choice portrait/landscape */
    hdsivideo_handle.HorizontalSyncActive = (HSA * laneByteClk_kHz) / LcdClock;
    hdsivideo_handle.HorizontalBackPorch = (HBP * laneByteClk_kHz) / LcdClock;
    hdsivideo_handle.HorizontalLine = ( (HACT + HSA + HBP + HFP) * laneByteClk_kHz) / LcdClock; /* Value depending on display orientation choice portrait/landscape */
    hdsivideo_handle.VerticalSyncActive = VSA;
    hdsivideo_handle.VerticalBackPorch = VBP;
    hdsivideo_handle.VerticalFrontPorch = VFP;
    hdsivideo_handle.VerticalActive = VACT; /* Value depending on display orientation choice portrait/landscape */

    /* Enable or disable sending LP command while streaming is active in video mode */
    hdsivideo_handle.LPCommandEnable = DSI_LP_COMMAND_ENABLE; /* Enable sending commands in mode LP (Low Power) */

    /* Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions */
    /* Only useful when sending LP packets is allowed while streaming is active in video mode */
    hdsivideo_handle.LPLargestPacketSize = 16;

    /* Largest packet size possible to transmit in LP mode in HFP region during VACT period */
    /* Only useful when sending LP packets is allowed while streaming is active in video mode */
    hdsivideo_handle.LPVACTLargestPacketSize = 0;

    /* Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this region */
    /* while streaming is active in video mode                                                                         */
    hdsivideo_handle.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE; /* Allow sending LP commands during HFP period */
    hdsivideo_handle.LPHorizontalBackPorchEnable = DSI_LP_HBP_ENABLE; /* Allow sending LP commands during HBP period */
    hdsivideo_handle.LPVerticalActiveEnable = DSI_LP_VACT_ENABLE; /* Allow sending LP commands during VACT period */
    hdsivideo_handle.LPVerticalFrontPorchEnable = DSI_LP_VFP_ENABLE; /* Allow sending LP commands during VFP period */
    hdsivideo_handle.LPVerticalBackPorchEnable = DSI_LP_VBP_ENABLE; /* Allow sending LP commands during VBP period */
    hdsivideo_handle.LPVerticalSyncActiveEnable = DSI_LP_VSYNC_ENABLE; /* Allow sending LP commands during VSync = VSA period */

    /* Configure DSI Video mode timings with settings set above */
    HAL_DSI_ConfigVideoMode( & (hdsi_eval), & (hdsivideo_handle));

    /* Configure DSI PHY HS2LP and LP2HS timings */
    PhyTimings.ClockLaneHS2LPTime = 35;
    PhyTimings.ClockLaneLP2HSTime = 35;
    PhyTimings.DataLaneHS2LPTime = 35;
    PhyTimings.DataLaneLP2HSTime = 35;
    PhyTimings.DataLaneMaxReadTime = 0;
    PhyTimings.StopWaitTime = 10;
    HAL_DSI_ConfigPhyTimer( &hdsi_eval, &PhyTimings);

    /*************************End DSI Initialization*******************************/

    /************************LTDC Initialization***********************************/

    /* Timing Configuration */
    hltdc_eval.Init.HorizontalSync = (HSA - 1);
    hltdc_eval.Init.AccumulatedHBP = (HSA + HBP - 1);
    hltdc_eval.Init.AccumulatedActiveW = (lcd_x_size + HSA + HBP - 1);
    hltdc_eval.Init.TotalWidth = (lcd_x_size + HSA + HBP + HFP - 1);

    /* Initialize the LCD pixel width and pixel height */
    hltdc_eval.LayerCfg->ImageWidth = lcd_x_size;
    hltdc_eval.LayerCfg->ImageHeight = lcd_y_size;

    /* LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384 MHz / 7 = 54.857 MHz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 54.857 MHz / 2 = 27.429 MHz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct);

    /* Background value */
    hltdc_eval.Init.Backcolor.Blue = 0;
    hltdc_eval.Init.Backcolor.Green = 0;
    hltdc_eval.Init.Backcolor.Red = 0;
    hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc_eval.Instance = LTDC;

    /* Get LTDC Configuration from DSI Configuration */
    HAL_LTDCEx_StructInitFromVideoConfig( & (hltdc_eval), & (hdsivideo_handle));

    /* Initialize the LTDC */
    HAL_LTDC_Init( &hltdc_eval);

    /* Enable the DSI host and wrapper after the LTDC initialization
     To avoid any synchronization issue, the DSI shall be started after enabling the LTDC */
    HAL_DSI_Start( & (hdsi_eval));

#if !defined(DATA_IN_ExtSDRAM)
    /* Initialize the SDRAM */
//  BSP_SDRAM_Init();
#endif /* DATA_IN_ExtSDRAM */

    /* Initialize the font */
//  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    /************************End LTDC Initialization*******************************/

    /* Checking the ID to determine the type of component */
    if (1)
    {
        /***********************NT35510 Initialization********************************/
        /* Initialize the NT35510 LCD Display IC Driver (TechShine LCD IC Driver)
         * depending on configuration set in 'hdsivideo_handle'.
         */
        NT35510_Init(NT35510_FORMAT_RGB888, orientation);
        /***********************End NT35510 Initialization****************************/
    }
    else
    {
        return LCD_ERROR;
    }
#endif
    return LCD_OK;
}
/**
 * @brief  Gets the LCD X size.
 * @retval Used LCD X size
 */
uint32_t BSP_LCD_GetXSize(void)
{
    return (lcd_x_size);
}

/**
 * @brief  Gets the LCD Y size.
 * @retval Used LCD Y size
 */
uint32_t BSP_LCD_GetYSize(void)
{
    return (lcd_y_size);
}
/**
 * @brief  Initializes the LCD layers.
 * @param  LayerIndex: Layer foreground or background
 * @param  FB_Address: Layer frame buffer
 */
void BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
    LCD_LayerCfgTypeDef Layercfg;

    /* Layer Init */
    Layercfg.WindowX0 = 0;
    Layercfg.WindowX1 = BSP_LCD_GetXSize();
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = BSP_LCD_GetYSize();
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
    Layercfg.FBStartAdress = FB_Address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = BSP_LCD_GetXSize();
    Layercfg.ImageHeight = BSP_LCD_GetYSize();

    HAL_LTDC_ConfigLayer( &hltdc_eval, &Layercfg, LayerIndex);
}

/////////////////////////////LVGL port

/*********************
 *      DEFINES
 *********************/

#define MY_DISP_HOR_RES    480
#define MY_DISP_VER_RES    480

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
//    disp_init();
    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

//    /* Example for 1) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10]; /*A buffer for 10 rows*/
    lv_disp_draw_buf_init( &draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10); /*Initialize the display buffer*/
    /* Example for 2) */
//    static lv_disp_draw_buf_t draw_buf_dsc_2;
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10]; /*A buffer for 10 rows*/
//    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10]; /*An other buffer for 10 rows*/
//    lv_disp_draw_buf_init( &draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10); /*Initialize the display buffer*/
//    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
//                          MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/
    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init( &disp_drv); /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;
    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;
    /*Finally register the driver*/
    lv_disp_drv_register( &disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{

    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++)
        for(x = area->x1; x <= area->x2; x++)
        {
            *(__IO uint32_t*) (LAYER0_ADDRESS + (480 - y) * 480 * 4 + 4 * x) = color_p->full;
            color_p++;
        }

//    int32_t x1 = area->x1;
//    int32_t y1 = area->y1;
//    int32_t x2 = area->x2;
//    int32_t y2 = area->y2;
//
//    uint32_t width = x2 - x1 + 1;
//    uint32_t height = y2 - y1 + 1;
//
//    // Dia chi bat dau trong frame buffer
//    uint32_t *dst_buf = (uint32_t*) (LAYER0_ADDRESS + ( (480 - y1 - height) * 480 + x1) * 4);
//
//    // Copy boi vi DMA2D trong LVGL se tang toc ben trong
//    for(uint32_t row = 0; row < height; row++)
//    {
//        // Thay vì row, ghi vào hàng ngược lại
//        memcpy( &dst_buf[ (height - 1 - row) * 480], &color_p[row * width], width * 4);
//    }
//    uint32_t destination = (uint32_t)LAYER0_ADDRESS + (area->y1 * 480 + area->x1) * 4;
//      uint32_t source      = (uint32_t)color_p;
//
//    /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
//    hdma2d.Init.Mode         = DMA2D_M2M;
//    hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
//    hdma2d.Init.OutputOffset = 480 - (area->x2 - area->x1+1);
//
//    /*##-2- DMA2D Callbacks Configuration ######################################*/
//    hdma2d.XferCpltCallback  = NULL;
//
//    /*##-3- Foreground Configuration ###########################################*/
//    hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
//    hdma2d.LayerCfg[1].InputAlpha = 0xFF;
//    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
//    hdma2d.LayerCfg[1].InputOffset = 0;
//
//    hdma2d.Instance          = DMA2D;
//
//    /* DMA2D Initialization */
//    if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
//
//    if (HAL_DMA2D_Start(&hdma2d, source, destination, area->x2 - area->x1+1, area->y2 - area->y1+1) == HAL_OK)
//          {
//            /* Polling For DMA transfer */
//            HAL_DMA2D_PollForTransfer(&hdma2d, 1000);
//          }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}
/** Status of the application. */
typedef enum
{
    BOOT_IDLE = 0, BOOT_START, BOOT_RUNNING,

} STATE_APP;
STATE_APP state_bootloader;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void RTC_SetTimeDate(void)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    sTime.Hours = Bike_SysData.Display.Time.Raw.Hour;
    sTime.Minutes = Bike_SysData.Display.Time.Raw.Minute;
    sTime.Seconds = Bike_SysData.Display.Time.Raw.Second ;
    //sTime.TimeFormat = RTC_HOURFORMAT12_AM;
    sTime.TimeFormat = Bike_SysData.Display.Setting.IsModeAM ? RTC_HOURFORMAT12_AM : RTC_HOURFORMAT12_PM;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime( &hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) Error_Handler();

    //sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = Bike_SysData.Display.Time.Raw.Month;
    sDate.Date = Bike_SysData.Display.Time.Raw.Date;
    sDate.Year = Bike_SysData.Display.Time.Raw.Year; // năm 2025

    if (HAL_RTC_SetDate( &hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) Error_Handler();
}

void RTC_GetTimeDate(void)
{
    RTC_TimeTypeDef gTime;
    RTC_DateTypeDef gDate;

    HAL_RTC_GetTime( &hrtc, &gTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate( &hrtc, &gDate, RTC_FORMAT_BIN);

    Bike_SysData.Display.Time.Raw.Hour = gTime.Hours;
    Bike_SysData.Display.Time.Raw.Minute = gTime.Minutes;
    Bike_SysData.Display.Time.Raw.Second = gTime.Seconds;

    Bike_SysData.Display.Time.Raw.Date = gDate.Date;
    Bike_SysData.Display.Time.Raw.Month = gDate.Month;
    Bike_SysData.Display.Time.Raw.Year = gDate.Year;

//
//    printf("Time: %02d:%02d:%02d\r\n", gTime.Hours, gTime.Minutes, gTime.Seconds);
//    printf("Date: %02d-%02d-%02d\r\n", gDate.Date, gDate.Month, 2000 + gDate.Year);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
#if 0
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();
  MX_DSIHOST_DSI_Init();
  MX_LTDC_Init();
  MX_DMA2D_Init();
  MX_CAN1_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
#else
    MX_GPIO_Init();
    MX_FMC_Init();
    MX_RTC_Init();
    MX_I2C1_Init();
    BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE);
    BSP_SDRAM_Initialization_sequence(REFRESH_COUNT);
    BSP_LCD_LayerDefaultInit(0, LAYER0_ADDRESS);
    MX_CAN1_Init();
#endif

    lv_init();
    lv_port_disp_init();
    ui_init();

    CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0;
    sFilterConfig.FilterIdLow = 0;
    sFilterConfig.FilterMaskIdHigh = 0;
    sFilterConfig.FilterMaskIdLow = 0;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 0;
    HAL_CAN_ConfigFilter( &hcan1, &sFilterConfig);

    HAL_CAN_Start( &hcan1);  // Bắt đầu CAN
    HAL_CAN_ActivateNotification( &hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // Bật ngắt nhận
    CanQueue_Init( &CanRxQueue);
    Lcd_LoadMetter();
    Lcd_LoadSetting();
    Bike_SysData.Display.Metter.Remain = 0.0;
    Bike_SysData.Esc.Esc_1.Raw.Speed = 0;
    Bike_SysData.Display.IsCharing = VMU_NOT_CHARGE;
    Bike_SysData.Display.TripScreenNow = VMUD_TRIP_SCREEN_TRIP_1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    volatile u32 stick = HAL_GetTick();
    volatile u32 stickLvgl = 0;
    volatile u32 stickUpdateUI = 0;
    volatile u32 stickCalcMetter = 0;
    while(1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        Lcd_InitFirstTime();
        Lcd_GetButton();
        if (HAL_GetTick() - stickLvgl > 5)
        {
            lv_task_handler();
            stickLvgl = HAL_GetTick();
        }

        if (HAL_GetTick() - stickUpdateUI >= 30)
        {
            Lcd_UpdateUi();
            stickUpdateUI = HAL_GetTick();
        }

        if (HAL_GetTick() - stickCalcMetter >= 30)
        {
            Lcd_CalcMetter();
            stickCalcMetter = HAL_GetTick();
        }

        if (HAL_GetTick() - stick > 1000)
        {
            RTC_GetTimeDate();
            Lcd_WriteMetter(Bike_SysData.Display.Metter);
            Lcd_WriteSetting(Bike_SysData.Display.Setting);
            VmuD_SendTime();
            stick = HAL_GetTick();
        }
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */
  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_10TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */
  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */
  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */
  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief DSIHOST Initialization Function
  * @param None
  * @retval None
  */
static void MX_DSIHOST_DSI_Init(void)
{

  /* USER CODE BEGIN DSIHOST_Init 0 */
  /* USER CODE END DSIHOST_Init 0 */

  DSI_PLLInitTypeDef PLLInit = {0};
  DSI_HOST_TimeoutTypeDef HostTimeouts = {0};
  DSI_PHY_TimerTypeDef PhyTimings = {0};
  DSI_LPCmdTypeDef LPCmd = {0};
  DSI_CmdCfgTypeDef CmdCfg = {0};

  /* USER CODE BEGIN DSIHOST_Init 1 */
  /* USER CODE END DSIHOST_Init 1 */
  hdsi.Instance = DSI;
  hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
  hdsi.Init.TXEscapeCkdiv = 4;
  hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  PLLInit.PLLNDIV = 125;
  PLLInit.PLLIDF = DSI_PLL_IN_DIV4;
  PLLInit.PLLODF = DSI_PLL_OUT_DIV1;
  if (HAL_DSI_Init(&hdsi, &PLLInit) != HAL_OK)
  {
    Error_Handler();
  }
  HostTimeouts.TimeoutCkdiv = 1;
  HostTimeouts.HighSpeedTransmissionTimeout = 0;
  HostTimeouts.LowPowerReceptionTimeout = 0;
  HostTimeouts.HighSpeedReadTimeout = 0;
  HostTimeouts.LowPowerReadTimeout = 0;
  HostTimeouts.HighSpeedWriteTimeout = 0;
  HostTimeouts.HighSpeedWritePrespMode = DSI_HS_PM_DISABLE;
  HostTimeouts.LowPowerWriteTimeout = 0;
  HostTimeouts.BTATimeout = 0;
  if (HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK)
  {
    Error_Handler();
  }
  PhyTimings.ClockLaneHS2LPTime = 28;
  PhyTimings.ClockLaneLP2HSTime = 33;
  PhyTimings.DataLaneHS2LPTime = 15;
  PhyTimings.DataLaneLP2HSTime = 25;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 16;
  if (HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetLowPowerRXFilter(&hdsi, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigErrorMonitor(&hdsi, HAL_DSI_ERROR_NONE) != HAL_OK)
  {
    Error_Handler();
  }
  LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite = DSI_LP_DLW_ENABLE;
  LPCmd.LPMaxReadPacket = DSI_LP_MRDP_ENABLE;
  LPCmd.AcknowledgeRequest = DSI_ACKNOWLEDGE_DISABLE;
  if (HAL_DSI_ConfigCommand(&hdsi, &LPCmd) != HAL_OK)
  {
    Error_Handler();
  }
  CmdCfg.VirtualChannelID = 0;
  CmdCfg.ColorCoding = DSI_RGB888;
  CmdCfg.CommandSize = 200;
  CmdCfg.TearingEffectSource = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.HSPolarity = DSI_HSYNC_ACTIVE_LOW;
  CmdCfg.VSPolarity = DSI_VSYNC_ACTIVE_LOW;
  CmdCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.VSyncPol = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh = DSI_AR_ENABLE;
  CmdCfg.TEAcknowledgeRequest = DSI_TE_ACKNOWLEDGE_ENABLE;
  if (HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetGenericVCID(&hdsi, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DSIHOST_Init 2 */
  /* USER CODE END DSIHOST_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */
  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */
  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 0;
  hltdc.Init.VerticalSync = 13;
  hltdc.Init.AccumulatedHBP = 1;
  hltdc.Init.AccumulatedVBP = 14;
  hltdc.Init.AccumulatedActiveW = 201;
  hltdc.Init.AccumulatedActiveH = 494;
  hltdc.Init.TotalWidth = 202;
  hltdc.Init.TotalHeigh = 496;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 0;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 0;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 0;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 0;
  pLayerCfg.ImageHeight = 0;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 0;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 0;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg1.FBStartAdress = 0;
  pLayerCfg1.ImageWidth = 0;
  pLayerCfg1.ImageHeight = 0;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */
  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */
  /* Enable LSE and wait ready */
  __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET) {}
  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
//  sTime.Hours = 0x0;
//  sTime.Minutes = 0x0;
//  sTime.Seconds = 0x0;
//  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
//  sDate.Month = RTC_MONTH_OCTOBER;
//  sDate.Date = 0x11;
//  sDate.Year = 0x0;
//
//  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */
  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */
  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RESET_Pin|LED_LCD_Pin|SPI3_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_GREEN_Pin|LED_RED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_RESET_Pin LED_LCD_Pin SPI3_CS_Pin */
  GPIO_InitStruct.Pin = LCD_RESET_Pin|LED_LCD_Pin|SPI3_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_OTG_HS_FS;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_GREEN_Pin LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CanMsgRxType frame = {0};
    //
    /* Get the CAN message */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &frame.Header, frame.Data) == HAL_OK)
    {
        if (hcan->Instance == hcan1.Instance)
        {
            //CanQueue_Push( &CanRxQueue, &frame);
            VmuMsg_UpdateSysTemData( &frame);
        }
    }
}
bool IsOdoReset = FALSE;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_12)
	{
		// Normally odo is only reseted if key is off <=> no supply power so it only resets when we power VMUD board
		if ((Bike_SysData.VmuM.Switch.Raw.AccStatus_1 == 0) && (Bike_SysData.VmuM.Switch.Raw.AccStatus_2 == 0))
		{
			IsOdoReset = TRUE;
		}
		else
		{
			IsOdoReset = FALSE;
		}
		if(IsExtPressButton == FALSE)
		IsExtPressButton = TRUE;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
