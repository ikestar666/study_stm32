#include "i2c_ee.h"
#include "usart.h"


static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


static void I2C_Mode_Configu(void)
{
    I2C_InitTypeDef  I2C_InitStructure;

    /* I2C ���� */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

    /* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;

    I2C_InitStructure.I2C_OwnAddress1 = 0x0A;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;

    /* I2C��Ѱַģʽ */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    /* ͨ������ */
    I2C_InitStructure.I2C_ClockSpeed = 400000;

    /* I2C ��ʼ�� */
    I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);

    /* ʹ�� I2C */
    I2C_Cmd(EEPROM_I2Cx, ENABLE);
}

void I2C_GPIO_Configu()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // ���� GPIO ʱ��
    I2Cx_CLK_CMD_Func(I2Cx_CLK, ENABLE);
    I2Cx_GPIO_CLK_CMD_Func(I2Cx_GPIO_CLK, ENABLE);

    // ���� SCL GPIO ����ģʽ
    GPIO_InitStruct.GPIO_Pin = I2Cx_SCL_GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2Cx_SCL_GPIO_Port, &GPIO_InitStruct);
    // ���� SDA GPIO ����ģʽ
    GPIO_InitStruct.GPIO_Pin = I2Cx_SDA_GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2Cx_SDA_GPIO_Port, &GPIO_InitStruct);
}


void I2C_EEPROM_Configu()
{
    // ����GPIO
    I2C_GPIO_Configu();
    I2C_Mode_Configu();

    printf("��ʼ��&ʹ��I2C��·\n");
}

// �������� дһ���ֽڵ�EEPROM��
uint32_t I2C_EEPROM_ByteWrite(uint8_t byte, uint8_t addr)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;

    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
    }
    // ������ʼ�ź�
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    // ���EV5�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(0);
    }
    // ����ͨѶ�����ַ
    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Transmitter);
    // ���EV6�¼�addrz
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(1);
    }
    // ��������д���ַ
    I2C_SendData(EEPROM_I2Cx, addr);
    // ���EV8�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(2);
    }
    // ��������
    I2C_SendData(EEPROM_I2Cx, byte);
    // ���EV8_2 ʱ��
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(3);
    }
    // ����ֹͣλ
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
    // ������ʼ�ź�
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    // ���EV5�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(20);
    }
    // ����ͨѶ�����ַ
    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Transmitter);
    // ���EV6�¼�addrz
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(21);
    }
    // ��������д���ַ
    I2C_SendData(EEPROM_I2Cx, addr);
    // ���EV8�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(22);
    }
    // ��������
    while (numOfByte--)
    {
        I2C_SendData(EEPROM_I2Cx, *pBuffer);
        pBuffer++;
        // ���EV8_2 ʱ��
        I2CTimeout = I2CT_FLAG_TIMEOUT;
        while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        {
            if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(23);
        }
    }
    // ����ֹͣλ
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
    return 1;
}

// ������������
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

// �������� ���ݵ�ַ ��ȡEEPROM�е�����
uint32_t I2C_EEPROM_SequentialRead(uint8_t *pdata, uint8_t addr, u16 numberToRead)
{
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(10);
    }

    // Random Read ��ͷ
    // ������ʼλ
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);

    // ���EV5�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(4);
    }

    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Transmitter);

    // ���EV6�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(5);
    }
    I2C_Cmd(EEPROM_I2Cx, ENABLE);

    // ���������ȡ��ַ
    I2C_SendData(EEPROM_I2Cx, addr);

    // ���EV8�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(6);
    }


    // �ٴη�����ʼλ
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);

    // ���EV5�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(7);

    }

    // �����豸����ַ
    I2C_Send7bitAddress(EEPROM_I2Cx, I2C_EEPROM_Addr, I2C_Direction_Receiver);

    // ���EV6�¼�
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if (I2CTimeout-- == 0) return I2C_TIMEOUT_UserCallback(8);
    }

    // ѭ����������
    while (numberToRead)
    {
        // ׼����ȡ���һ���ֽ�����ʱ�����÷���NA�ź�
        if (numberToRead == 1)
        {
//            printf("������");
            I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);
            I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
        }

        // ÿ�ν���һ���ֽڵ����ݶ���Ҫ���EV7�¼�
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
    EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d", errorCode);

    return 0;
}


/**
  * @brief  Wait for EEPROM Standby state
  * @param  ��
  * @retval ��
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

