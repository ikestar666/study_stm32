#ifndef __SPI_FLASH_H_
#define __SPI_FLASH_H_

#include "stm32f10x.h"
#include "stm32f10x_spi.h"



#define sFLASH_ID    0xEF4017
#define FLASH_PAGE_SIZE     256

// 空指令
#define DUMMY_BYTE          0xFF
// 设备指令
#define WRITE_ENABLE        0x06
#define WRITE_DISABLE       0x04
#define READ_STATUS_REG     0x05
#define WRITE_STATUS_REG    0x01
#define READ_DATA           0x03
#define FAST_READ_DATA      0x0B
#define FAST_READ_DUAL      0x3B
#define PAGE_PROGRAM        0x02
#define BLOCK_ERASE         0xDB
#define SECTOR_ERASE        0x20
#define CHIP_ERASE          0xC7
#define POWER_DOWN          0xB9
#define RELEASE_POWER_DOWN  0xAB
#define DEVICE_ID           0xAB
#define MANUFACT_DEVICE_ID  0x90
#define JEDEC_DEVICE_ID     0x9F



// SPI 设备时钟
#define SPI_CLK_CMD_Func    RCC_APB2PeriphClockCmd
#define SPI_CLK                         RCC_APB2Periph_SPI1
#define SPIx                     SPI1

// SPI 复用引脚
#define SPI_GPIO_CLK_CMD_Func    RCC_APB2PeriphClockCmd
// NSS
#define SPI_FLASH_GPIO_CS_CLK        RCC_APB2Periph_GPIOC
#define SPI_FLASH_GPIO_CS_PORT                      GPIOC
#define SPI_FLASH_GPIO_CS_Pin                       GPIO_Pin_0

// SCK
#define SPI_FLASH_GPIO_SCK_CLK        RCC_APB2Periph_GPIOA
#define SPI_FLASH_GPIO_SCK_PORT                     GPIOA
#define SPI_FLASH_GPIO_SCK_Pin                      GPIO_Pin_5

// MOSI
#define SPI_FLASH_GPIO_DIO_CLK        RCC_APB2Periph_GPIOA
#define SPI_FLASH_GPIO_DIO_PORT                     GPIOA
#define SPI_FLASH_GPIO_DIO_Pin                      GPIO_Pin_7

// MISO
#define SPI_FLASH_GPIO_DO_CLK        RCC_APB2Periph_GPIOA
#define SPI_FLASH_GPIO_DO_PORT                      GPIOA
#define SPI_FLASH_GPIO_DO_Pin                       GPIO_Pin_6


// FLASH 停止信号
#define SPI_FLASH_CS_HIGH()           GPIO_SetBits(SPI_FLASH_GPIO_CS_PORT, SPI_FLASH_GPIO_CS_Pin)
#define SPI_FLASH_CS_LOW()           GPIO_ResetBits(SPI_FLASH_GPIO_CS_PORT, SPI_FLASH_GPIO_CS_Pin)

// 等待超时时间
#define SPIT_FLAG_TIMEOUT      ((uint32_t) 0x1000)
#define SPIT_LONG_TIMEOUT       ((uint32_t) (SPIT_FLAG_TIMEOUT*10))


// 信息输出
#define FLASH_DEBUG_ON     0

#define FLASH_INFO(fmt, arg...)     printf("<<-FLASH-INFO->> "fmt"\n", ##arg);
#define FLASH_ERROR(fmt, arg...)     printf("<<-FLASH-ERROR->> "fmt"\n", ##arg);

#define FLASH_DEBUG(fmt, arg...)     do { \
                                         if(FLASH_DEBUG_ON)\
                                                                                        printf("<<-FLASH-DEBUG->> [%d]"fmt"\n", __LINE__, ##arg);\
                                      }while(0)


u8 SPI_WriteByte(u8 data);
u8 SPI_ReadByte(void);
void SPI_FLASH_EnableWrite(void);
void SPI_FLASH_WaitForWrite(void);
void SPI_FLASH_EraseSector(uint32_t addr);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_PageWrite(u8 *pData, u32 writeAddr, u16 numberToWrite);
void SPI_FLASH_BufferWrite(u8 *pData, u32 writeAddr, u16 numberToWrite);
void SPI_FLASH_BufferRead(u8 *pBuffer, u32 readAddr, u16 numberToRead);
void SPI_FLASH_Configu(void);
void SPI_FLASH_WakeUp(void);

#endif
