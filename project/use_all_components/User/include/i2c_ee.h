#ifndef __IIC_EE_H
#define __IIC_EE_H


#include "stm32f10x.h"

// I2C 配置相关的宏定义（包括外设、时钟、引脚、配置等）

#define     EEPROM_I2Cx                                       I2C1

#define     I2Cx_CLK_CMD_Func                                 RCC_APB1PeriphClockCmd
#define     I2Cx_CLK                                          RCC_APB1Periph_I2C1
#define     I2Cx_GPIO_CLK_CMD_Func                            RCC_APB2PeriphClockCmd
#define     I2Cx_GPIO_CLK                                     RCC_APB2Periph_GPIOB

#define     I2Cx_SCL_GPIO_Port                                GPIOB
#define     I2Cx_SCL_GPIO_Pin                                 GPIO_Pin_6
#define     I2Cx_SDA_GPIO_Port                                GPIOB
#define     I2Cx_SDA_GPIO_Pin                                 GPIO_Pin_7

#define     I2Cx_ClockSpeed                                   400000
#define     I2Cx_OwnAddress1                                                                    0x0A

// 通信外设地址
#define     I2C_EEPROM_Addr                                                                     0xA0

// EEPROM 的page大小
#define     I2C_EEPROM_PageSize                                                             8

// 等待时间
/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*信息输出*/
#define EEPROM_DEBUG_ON         0

#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


void I2C_EEPROM_Configu(void);
uint32_t I2C_EEPROM_ByteWrite(uint8_t byte, uint8_t addr);
uint32_t I2C_EEPROM_PageWrite(uint8_t *pBuffer, uint8_t addr, uint8_t numOfByte);
void I2C_EEPROM_BufferWrite(uint8_t *pBuffer, uint8_t addr, uint8_t numberToWrite);
uint32_t I2C_EEPROM_SequentialRead(uint8_t *pdata, uint8_t addr, u16 numberToRead);
void I2C_EE_WaitEepromStandbyState(void);
uint32_t I2C_EE_BufferRead(u8 *pbuffer, u8 addr, u16 numtoread);
#endif /* __IIC_EE_H */
