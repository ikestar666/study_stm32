#include "i2c_ee.h"
#include "usart.h"


static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


static void I2C_Mode_Configu(void)
{
    I2C_InitTypeDef  I2C_InitStructure;

    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

    /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;

    I2C_InitStructure.I2C_OwnAddress1 = 0x0A;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;

    /* I2C的寻址模式 */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    /* 通信速率 */
    I2C_InitStructure.I2C_ClockSpeed = 400000;

    /* I2C 初始化 */
    I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);

    /* 使能 I2C */
    I2C_Cmd(EEPROM_I2Cx, ENABLE);
}

void I2C_GPIO_Configu()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // 配置 GPIO 时钟
    I2Cx_CLK_CMD_Func(I2Cx_CLK, ENABLE);
    I2Cx_GPIO_CLK_CMD_Func(I2Cx_GPIO_CLK, ENABLE);

    // 配置 SCL GPIO 工作模式
    GPIO_InitStruct.GPIO_Pin = I2Cx_SCL_GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2Cx_SCL_GPIO_Port, &GPIO_InitStruct);
    // 配置 SDA GPIO 工作模式
    GPIO_InitStruct.GPIO_Pin = I2Cx_SDA_GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2Cx_SDA_GPIO_Port, &GPIO_InitStruct);
}


void I2C_EEPROM_Configu()
{
    // 配置GPIO
    I2C_GPIO_Configu();
    I2C_Mode_Configu();

    printf("初始化&使能I2C电路\n");
}

// 主发送器 写一个字节到EEPROM中
uint32_t I2C_EEPROM_ByteWrite(uint8_t byte, uint8_t addr)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;

    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
    }
    // 发送起始信号
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    // 检测EV5事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(0);
    }
    // 发送通讯外设地址
    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Transmitter);
    // 检测EV6事件addrz
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(1);
    }
    // 发送数据写入地址
    I2C_SendData(EEPROM_I2Cx, addr);
    // 检测EV8事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(2);
    }
    // 发送数据
    I2C_SendData(EEPROM_I2Cx, byte);
    // 检测EV8_2 时间
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(3);
    }
    // 发送停止位
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
    return 1;
}


uint32_t I2C_EEPROM_PageWrite(uint8_t *pBuffer, uint8_t addr, uint8_t numOfByte)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;

    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
    }
    // 发送起始信号
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    // 检测EV5事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(20);
    }
    // 发送通讯外设地址
    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Transmitter);
    // 检测EV6事件addrz
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(21);
    }
    // 发送数据写入地址
    I2C_SendData(EEPROM_I2Cx, addr);
    // 检测EV8事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(22);
    }
    // 发送数据
    while (numOfByte--)
    {
        I2C_SendData(EEPROM_I2Cx, *pBuffer);
        pBuffer++;
        // 检测EV8_2 时间
        I2CTimeout = I2CT_FLAG_TIMEOUT;
        while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        {
            if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(23);
        }
    }
    // 发送停止位
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
    return 1;
}

// 连续发送数据
void I2C_EEPROM_BufferWrite(uint8_t *pBuffer, uint8_t addr, uint8_t numberToWrite)
{
    u8 needByteWriteBefore;
    u8 pageWriteSize;
    u8 needByteWriteAfter;
    needByteWriteBefore = I2C_EEPROM_PageSize - addr % I2C_EEPROM_PageSize;
    if (needByteWriteBefore == 8)
    {
        needByteWriteBefore = 0;
    }
    if (numberToWrite > needByteWriteBefore)
    {
        pageWriteSize = (numberToWrite - needByteWriteBefore) / I2C_EEPROM_PageSize;
        needByteWriteAfter = (numberToWrite - needByteWriteBefore) % I2C_EEPROM_PageSize;
    }
    else
    {
        pageWriteSize = 0;
        needByteWriteAfter = 0;
    }

    while (needByteWriteBefore--)
    {
        I2C_EEPROM_ByteWrite(*pBuffer, addr);
			I2C_EE_WaitEepromStandbyState();
        pBuffer ++;
        addr++;
    }
    while (pageWriteSize--)
    {
        I2C_EEPROM_PageWrite(pBuffer, addr, 8);
			I2C_EE_WaitEepromStandbyState();
        pBuffer += 8;
        addr += 8;
    }
    I2C_EEPROM_PageWrite(pBuffer, addr, needByteWriteAfter);

}

// 主接收器 根据地址 读取EEPROM中的数据
uint32_t I2C_EEPROM_SequentialRead(uint8_t *pdata, uint8_t addr, u16 numberToRead)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(10);
    }

    // Random Read 开头
    // 发送起始位
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);

    // 检测EV5事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(4);
    }

    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Transmitter);

    // 检测EV6事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(5);
    }
    I2C_Cmd(EEPROM_I2Cx, ENABLE);

    // 发送随机读取地址
    I2C_SendData(EEPROM_I2Cx, addr);

    // 检测EV8事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(6);
    }


    // 再次发起起始位
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);

    // 检测EV5事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(7);

    }

    // 发送设备读地址
    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Receiver);

    // 检测EV6事件
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(8);
    }

    // 循环接受数据
    while (numberToRead)
    {
        // 准备读取最后一个字节数据时，配置发送NA信号
        if (numberToRead == 1)
        {
//            printf("进来了");
            I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);
            I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
        }

        // 每次接受一个字节的数据都需要检测EV7事件
        I2CTimeout = I2CT_LONG_TIMEOUT;
        while (I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) == 0)
        {
            if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(9);
        }
        {

            *pdata = I2C_ReceiveData(EEPROM_I2Cx);
            pdata ++;
            numberToRead--;
        }
    }
    I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);

    return 1;

}

static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* Block communication and all processes */
    EEPROM_ERROR("I2C 等待超时!errorCode = %d", errorCode);

    return 0;
}


/**
  * @brief  Wait for EEPROM Standby state
  * @param  无
  * @retval 无
  */
void I2C_EE_WaitEepromStandbyState(void)
{
    vu16 SR1_Tmp = 0;

    do
    {
        /* Send START condition */
        I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
        /* Read I2C1 SR1 register */
        SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);
        /* Send EEPROM address for write */
        I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Transmitter);
    }
    while (!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));

    /* Clear AF flag */
    I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);
    /* STOP condition */
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
}

