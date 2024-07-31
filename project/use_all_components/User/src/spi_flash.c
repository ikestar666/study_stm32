#include "spi_flash.h"
#include "usart.h"

static __IO uint32_t  SPITimeout = SPIT_FLAG_TIMEOUT;
static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


// ��ʼ����ʹ��SPI����
void SPI_FLASH_Configu()
{
    GPIO_InitTypeDef SPI_GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    // ʹ��ʱ��
    SPI_CLK_CMD_Func(SPI_CLK, ENABLE);
    SPI_GPIO_CLK_CMD_Func(SPI_FLASH_GPIO_CS_CLK
                          | SPI_FLASH_GPIO_DIO_CLK | SPI_FLASH_GPIO_DO_CLK | SPI_FLASH_GPIO_SCK_CLK, ENABLE);


    SPI_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    SPI_GPIO_InitStructure.GPIO_Pin = SPI_FLASH_GPIO_CS_Pin;
    SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_FLASH_GPIO_CS_PORT, &SPI_GPIO_InitStructure);

    SPI_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    SPI_GPIO_InitStructure.GPIO_Pin = SPI_FLASH_GPIO_SCK_Pin;
    SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_FLASH_GPIO_SCK_PORT, &SPI_GPIO_InitStructure);

    SPI_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    SPI_GPIO_InitStructure.GPIO_Pin = SPI_FLASH_GPIO_DIO_Pin;
    SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_FLASH_GPIO_DIO_PORT, &SPI_GPIO_InitStructure);

    SPI_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    SPI_GPIO_InitStructure.GPIO_Pin = SPI_FLASH_GPIO_DO_Pin;
    SPI_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_FLASH_GPIO_DO_PORT, &SPI_GPIO_InitStructure);

    // ������Ƭѡ
    SPI_FLASH_CS_HIGH();

    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;

    SPI_Init(SPIx, &SPI_InitStructure);
    SPI_Cmd(SPIx, ENABLE);

}

void SPI_FLASH_BufferRead(u8 *pBuffer, u32 readAddr, u16 numberToRead)
{
    // Ƭѡ�ź�
    SPI_FLASH_CS_LOW();
    // ����read ָ��
    SPI_WriteByte(READ_DATA);
    SPI_WriteByte((readAddr & 0xFF0000) >> 16);
    SPI_WriteByte((readAddr & 0xFF00) >> 8);
    SPI_WriteByte(readAddr & 0xFF);

    while (numberToRead--)
    {
        *pBuffer = SPI_ReadByte();
        pBuffer ++;
    }
    SPI_FLASH_CS_HIGH();
}

// ������д������
void SPI_FLASH_BufferWrite(u8 *pData, u32 writeAddr, u16 numberToWrite)
{

    u16 numberOfSingle, numberOfPage = 0;

    numberOfSingle = numberToWrite % FLASH_PAGE_SIZE;
    numberOfPage = (numberToWrite - numberOfSingle) / FLASH_PAGE_SIZE;

    SPI_FLASH_PageWrite(pData, writeAddr, numberOfSingle);

    pData += numberOfSingle;
    writeAddr += numberOfSingle;

    while (numberOfPage--)
    {
        SPI_FLASH_PageWrite(pData, writeAddr, FLASH_PAGE_SIZE);
        pData += FLASH_PAGE_SIZE;
        writeAddr += FLASH_PAGE_SIZE;
    }
    // ������Ƭѡ
    SPI_FLASH_CS_HIGH();
}

// ҳд�빦�ܣ���Ҫ��Ҫ����256���ֽ�,ҳд���ַ��ǿ��Ҫ����룬����һ������������ַ����
void SPI_FLASH_PageWrite(u8 *pData, u32 writeAddr, u16 numberToWrite)
{
    SPI_FLASH_EnableWrite();
    // Ƭѡ�ź�
    SPI_FLASH_CS_LOW();
    // ����page write ָ��
    SPI_WriteByte(PAGE_PROGRAM);
    SPI_WriteByte((writeAddr & 0xFF0000) >> 16);
    SPI_WriteByte((writeAddr & 0xFF00) >> 8);
    SPI_WriteByte(writeAddr & 0xFF);

    while (numberToWrite--)
    {
        SPI_WriteByte(*pData);
        pData ++;
        //SPI_FLASH_WaitForWrite();
    }

    // ������Ƭѡ
    SPI_FLASH_CS_HIGH();
    SPI_FLASH_WaitForWrite();

}


// Flash��д������ʱҲ����Ҫһ��ʱ�䣬��Ҫͨ����ȡFLASH�е�״̬�Ĵ����ж��ܷ����д��
void SPI_FLASH_WaitForWrite(void)
{

    u8 status = 0;
    // Ƭѡ�ź�
    SPI_FLASH_CS_LOW();

    // ���Ͳ�ѯ״ָ̬��
    SPI_WriteByte(READ_STATUS_REG);
    do
    {
        status = SPI_ReadByte();
    }
    while ((status & 0x01) == SET);

    // ������Ƭѡ
    SPI_FLASH_CS_HIGH();

}

// ʹ��д
void SPI_FLASH_EnableWrite()
{
    // Ƭѡ�ź�
    SPI_FLASH_CS_LOW();

    // ���Ͳ���ָ��
    SPI_WriteByte(WRITE_ENABLE);

    // ������Ƭѡ
    SPI_FLASH_CS_HIGH();
}

// ��ȡ�豸id
u32 SPI_FLASH_ReadDeviceID(void)
{
    u32 temp, temp1, temp2, temp3 = 0;

    // Ƭѡ�ź�
    SPI_FLASH_CS_LOW();

    // ���Ͳ���ָ��
    SPI_WriteByte(JEDEC_DEVICE_ID);

    // ���ζ�ȡ�����ֽڣ�
    temp1 = SPI_ReadByte();
    temp2 = SPI_ReadByte();
    temp3 = SPI_ReadByte();

    // ������Ƭѡ
    SPI_FLASH_CS_HIGH();

    temp = (temp1 << 16) | (temp2 << 8) | temp3;
    return temp;

}

// ����һ������, ָ���߿�����ֻ�ܶ�д��һ�����������������������˴���
void SPI_FLASH_EraseSector(uint32_t addr)
{
    // ע�����֮ǰҪʹ��д
    SPI_FLASH_EnableWrite();
    SPI_FLASH_WaitForWrite();
    // Ƭѡ�ź�
    SPI_FLASH_CS_LOW();

    // ���Ͳ���ָ��
    SPI_WriteByte(SECTOR_ERASE);

    SPI_WriteByte((addr & 0xFF0000) >> 16);

    SPI_WriteByte((addr & 0xFF00) >> 8);

    SPI_WriteByte(addr & 0xFF);

    // ������Ƭѡ
    SPI_FLASH_CS_HIGH();

    // �ȴ��������
    SPI_FLASH_WaitForWrite();


}

// дһ���ֽڣ� ���ｫ��ַҲ��Ϊһ�����ݵ�Ԫ����˲�������Ϊ��������
// ��Ϊʱȫ˫���ģ���˷������ݵ�ʱ��Ҳ���Խ�������
u8 SPI_WriteByte(u8 data)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;
    // �ȴ����ͻ�����Ϊ��
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
    {
        if (SPITimeout-- == 0) return SPI_TIMEOUT_UserCallback(0);
    }

    // ��������
    SPI_I2S_SendData(SPIx, data);

    SPITimeout = SPIT_FLAG_TIMEOUT;
    // �ȴ����ܻ������ǿ�
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if (SPITimeout-- == 0) return SPI_TIMEOUT_UserCallback(1);
    }

    // ��������
    return SPI_I2S_ReceiveData(SPIx);

}

// ��ȡһ���ֽ�, ��ȡ�ֽ�ʱҲ��Ҫͨ������һ���ֽ�������ʱ�ӣ����͵������޹�
u8 SPI_ReadByte(void)
{
    return SPI_WriteByte(DUMMY_BYTE);
}



// ������־

static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    FLASH_ERROR("SPI �ȴ���ʱ! errorCode=%d", errorCode);
    return 0;
}