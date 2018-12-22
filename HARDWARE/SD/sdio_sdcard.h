/**********************************************************
* @ File name -> sdio_sdcard.h
* @ Version   -> V1.0
* @ Date      -> 02-09-2014
* @ Brief     -> SDCard的SDIO驱动函数

 V1.
* @ Revise    -> 
**********************************************************/

#ifndef _sdio_sdcard_h_
#define _sdio_sdcard_h_

/**********************************************************
                     外部函数头文件                        
**********************************************************/

#include "stm32f10x_sdio.h"
#include "stm32f10x_dma.h"

#include "sys.h"		//用到中断分组
#include "string.h"		//读写disk用到内存操作相关函数

/* 通讯调节速度分频因子 */
/* HCLK = 72MHz, SDIOCLK = 72MHz */
/* 计算公式：SDIO_CK = SDIO_CLK / (CLKDIV + 2） */

#define SDIO_INIT_CLK_DIV                  ((uint8_t)0xb2)			//400KHz----> 0xb2(178)
#define SDIO_TRANSFER_CLK_DIV              ((uint8_t)0x02)
//24MHz ----> 0x01
//18MHz ----> 0x02
//12MHz ----> 0x04

/**********************************************************
                      SDIO工作模式定义
                 要修改工作模式请修改这里
**********************************************************/

#define SD_DMA_MODE                                ((uint32_t)0x00000000)
//#define SD_POLLING_MODE                            ((uint32_t)0x00000002)

/**********************************************************
                       相关宏定义
**********************************************************/

//SDIO Static flags, TimeOut, FIFO Address
#define NULL							0
#define SDIO_STATIC_FLAGS               ((uint32_t)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((uint32_t)0x00010000)
#define SDIO_DATATIMEOUT                ((uint32_t)0x0001FFFF)

//Mask for errors Card Status R1 (OCR Register)
#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)

//Masks for R6 Response
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define SD_VOLTAGE_WINDOW_MMC           ((uint32_t)0x80FF8000)
#define SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define SD_ALLZERO                      ((uint32_t)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define SD_CARD_LOCKED                  ((uint32_t)0x02000000)

#define SD_DATATIMEOUT                  ((uint32_t)0xFFFFFFFF)
#define SD_0TO7BITS                     ((uint32_t)0x000000FF)
#define SD_8TO15BITS                    ((uint32_t)0x0000FF00)
#define SD_16TO23BITS                   ((uint32_t)0x00FF0000)
#define SD_24TO31BITS                   ((uint32_t)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define SD_HALFFIFO                     ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES                ((uint32_t)0x00000020)

//Command Class Supported
#define SD_CCCC_LOCK_UNLOCK             ((uint32_t)0x00000080)
#define SD_CCCC_WRITE_PROT              ((uint32_t)0x00000040)
#define SD_CCCC_ERASE                   ((uint32_t)0x00000020)

//Following commands are SD Card Specific commands.
//SDIO_APP_CMD should be sent before sending these commands.
#define SDIO_SEND_IF_COND               ((uint32_t)0x00000008)

/**********************************************************
            SD卡初始化和读写卡是的状态值定义
**********************************************************/

typedef enum
{
	/* 初始化过程当中错误信息定义 */
	  
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),

	/* 初始化待机状态错误信息定义 */
	 
	SD_INTERNAL_ERROR, 
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING, 
	SD_REQUEST_NOT_APPLICABLE, 
	SD_INVALID_PARAMETER,  
	SD_UNSUPPORTED_FEATURE,  
	SD_UNSUPPORTED_HW,  
	SD_ERROR,  
	SD_OK = 0 
}SD_Error;

/**********************************************************
                    传输状态信息定义
**********************************************************/

//DMA2传输方向
#define DMA2_ReadDATA						0	//DMA读取数据
#define DMA2_WriteDATA						1	//DMA写入数据

//SDIO传输方向
#define SDIO_To_Card						0	//SDIO主机写数据到卡
#define Card_To_SDIO						1	//SDIO主机从卡读取数据

//SD命令响应长短
#define SD_Response_No                    	0	//无响应
#define SD_Response_Short                 	1	//短响应
#define SD_Response_Long                  	3	//长响应

typedef enum
{
	SD_TRANSFER_OK  = 0,	/* 传输成功 */
	SD_TRANSFER_BUSY = 1,	/* 正忙着呢 */
	SD_TRANSFER_ERROR		/* 传输出错 */
}SDTransferState;

typedef enum
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;

/**********************************************************
                    卡信息寄存器结构
**********************************************************/

//CSD寄存器结构定义
typedef struct
{
	__IO uint8_t  CSDStruct;            /*!< CSD structure */
	__IO uint8_t  SysSpecVersion;       /*!< System specification version */
	__IO uint8_t  Reserved1;            /*!< Reserved */
	__IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
	__IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	__IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	__IO uint16_t CardComdClasses;      /*!< Card command classes */
	__IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
	__IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
	__IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
	__IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
	__IO uint8_t  DSRImpl;              /*!< DSR implemented */
	__IO uint8_t  Reserved2;            /*!< Reserved */
	__IO uint32_t DeviceSize;           /*!< Device Size */
	__IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	__IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	__IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	__IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	__IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
	__IO uint8_t  EraseGrSize;          /*!< Erase group size */
	__IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
	__IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
	__IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
	__IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
	__IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
	__IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
	__IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	__IO uint8_t  Reserved3;            /*!< Reserded */
	__IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
	__IO uint8_t  FileFormatGrouop;     /*!< File format group */
	__IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
	__IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
	__IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
	__IO uint8_t  FileFormat;           /*!< File Format */
	__IO uint8_t  ECC;                  /*!< ECC code */
	__IO uint8_t  CSD_CRC;              /*!< CSD CRC */
	__IO uint8_t  Reserved4;            /*!< always 1*/
}SD_CSD;

//CID寄存器结构定义
typedef struct
{
	__IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
	__IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
	__IO uint32_t ProdName1;            /*!< Product Name part1 */
	__IO uint8_t  ProdName2;            /*!< Product Name part2*/
	__IO uint8_t  ProdRev;              /*!< Product Revision */
	__IO uint32_t ProdSN;               /*!< Product Serial Number */
	__IO uint8_t  Reserved1;            /*!< Reserved1 */
	__IO uint16_t ManufactDate;         /*!< Manufacturing Date */
	__IO uint8_t  CID_CRC;              /*!< CID CRC */
	__IO uint8_t  Reserved2;            /*!< always 1 */
}SD_CID;

//卡状态寄存器结构定义
typedef struct
{
	__IO uint8_t DAT_BUS_WIDTH;
	__IO uint8_t SECURED_MODE;
	__IO uint16_t SD_CARD_TYPE;
	__IO uint32_t SIZE_OF_PROTECTED_AREA;
	__IO uint8_t SPEED_CLASS;
	__IO uint8_t PERFORMANCE_MOVE;
	__IO uint8_t AU_SIZE;
	__IO uint16_t ERASE_SIZE;
	__IO uint8_t ERASE_TIMEOUT;
	__IO uint8_t ERASE_OFFSET;
}SD_CardStatus;

//卡信息汇总
typedef struct
{
	SD_CSD SD_csd;
	SD_CID SD_cid;
	long long CardCapacity;	/*!< Card Capacity */
	uint32_t CardBlockSize;	/*!< Card Block Size */
	uint16_t RCA;
	uint8_t CardType;
}SD_CardInfo;

extern SD_CardInfo SDCardInfo;

/**********************************************************
                    SD卡命令字节定义
                   对应十进制的 0 ~ 64
**********************************************************/

#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_NO_CMD                              ((uint8_t)64)

//APP命令，先发送CMD55再发送以下命令

#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53) /*!< For SD I/O Card only */

//以下命令仅支持SD Card

#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /*!< For SD Card only */

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT                                 ((uint8_t)0x01)
#define SD_NOT_PRESENT                             ((uint8_t)0x00)

/**********************************************************
                     支持卡的类型定义
**********************************************************/

#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

/**********************************************************
                     外部功能函数
**********************************************************/

void SD_DeInit(void);
SD_Error SD_Init(void);
SDTransferState SD_GetStatus(void);
SDCardState SD_GetState(void);
SD_Error SD_PowerON(void);
SD_Error SD_PowerOFF(void);
SD_Error SD_InitializeCards(void);
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
SD_Error SD_GetCardStatus(SD_CardStatus *cardstatus);
SD_Error SD_EnableWideBusOperation(uint32_t WideMode);
SD_Error SD_SelectDeselect(uint32_t addr);
SD_Error SD_ReadBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize);
SD_Error SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
SD_Error SD_WriteBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize);
SD_Error SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
SDTransferState SD_GetTransferState(void);
SD_Error SD_StopTransfer(void);
SD_Error SD_Erase(uint32_t startaddr, uint32_t endaddr);
SD_Error SD_SendStatus(uint32_t *pcardstatus);
SD_Error SD_SendSDStatus(uint32_t *psdstatus);
SD_Error SD_ProcessIRQSrc(void);
SD_Error SD_WaitReadOperation(void);
SD_Error SD_WaitWriteOperation(void);

SD_Error SD_ReadDisk(uint8_t *readbuff, uint32_t sector, uint16_t cnt);
SD_Error SD_WriteDisk(uint8_t *writebuff, uint32_t sector, uint16_t cnt);


#endif

