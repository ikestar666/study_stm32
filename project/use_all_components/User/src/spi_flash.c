#include "spi_flash.h"
#include "usart.h"

static __IO uint32_t  SPITimeout = SPIT_FLAG_TIMEOUT;
static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


// 初始化并使能SPI外设
void SPI_FLASH_Configu()
{
    GPIO_InitTypeDef SPI_GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    // 使能时钟
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

    // 不进行片选
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
    // 片选信号
    SPI_FLASH_CS_LOW();
    // 发送read 指令
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

// 不定量写入数据
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
    // 不进行片选
    SPI_FLASH_CS_HIGH();
}

// 页写入功能，主要不要超过256个字节,页写入地址不强制要求对齐，但是一般会和扇区基地址对齐
void SPI_FLASH_PageWrite(u8 *pData, u32 writeAddr, u16 numberToWrite)
{
    SPI_FLASH_EnableWrite();
    // 片选信号
    SPI_FLASH_CS_LOW();
    // 发送page write 指令
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

    // 不进行片选
    SPI_FLASH_CS_HIGH();
    SPI_FLASH_WaitForWrite();

}


// Flash在写入数据时也是需要一段时间，需要通过读取FLASH中的状态寄存器判断能否继续写入
void SPI_FLASH_WaitForWrite(void)
{

    u8 status = 0;
    // 片选信号
    SPI_FLASH_CS_LOW();

    // 发送查询状态指令
    SPI_WriteByte(READ_STATUS_REG);
    do
    {
        status = SPI_ReadByte();
    }
    while ((status & 0x01) == SET);

    // 不进行片选
    SPI_FLASH_CS_HIGH();

}

// 使能写
void SPI_FLASH_EnableWrite()
{
    // 片选信号
    SPI_FLASH_CS_LOW();

    // 发送擦除指令
    SPI_WriteByte(WRITE_ENABLE);

    // 不进行片选
    SPI_FLASH_CS_HIGH();
}

// 获取设备id
u32 SPI_FLASH_ReadDeviceID(void)
{
    u32 temp, temp1, temp2, temp3 = 0;

    // 片选信号
    SPI_FLASH_CS_LOW();

    // 发送擦除指令
    SPI_WriteByte(JEDEC_DEVICE_ID);

    // 依次读取三个字节；
    temp1 = SPI_ReadByte();
    temp2 = SPI_ReadByte();
    temp3 = SPI_ReadByte();

    // 不进行片选
    SPI_FLASH_CS_HIGH();

    temp = (temp1 << 16) | (temp2 << 8) | temp3;
    return temp;

}

// 擦除一个扇区, 指南者开发板只能读写第一个扇区，其他扇区都保留了代码
void SPI_FLASH_EraseSector(uint32_t addr)
{
    // 注意擦除之前要使能写
    SPI_FLASH_EnableWrite();
    SPI_FLASH_WaitForWrite();
    // 片选信号
    SPI_FLASH_CS_LOW();

    // 发送擦除指令
    SPI_WriteByte(SECTOR_ERASE);

    SPI_WriteByte((addr & 0xFF0000) >> 16);

    SPI_WriteByte((addr & 0xFF00) >> 8);

    SPI_WriteByte(addr & 0xFF);

    // 不进行片选
    SPI_FLASH_CS_HIGH();

    // 等待擦除完毕
    SPI_FLASH_WaitForWrite();


}

// 写一个字节， 这里将地址也作为一个数据单元，因此不单独作为参数传入
// 因为时全双工的，因此发送数据的时候也可以接收数据
u8 SPI_WriteByte(u8 data)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;
    // 等待发送缓冲区为空
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
    {
        if (SPITimeout-- == 0) return SPI_TIMEOUT_UserCallback(0);
    }

    // 发送数据
    SPI_I2S_SendData(SPIx, data);

    SPITimeout = SPIT_FLAG_TIMEOUT;
    // 等待接受缓冲区非空
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if (SPITimeout-- == 0) return SPI_TIMEOUT_UserCallback(1);
    }

    // 接收数据
    return SPI_I2S_ReceiveData(SPIx);

}

// 读取一个字节, 读取字节时也需要通过发送一个字节来激发时钟，发送的内容无关
u8 SPI_ReadByte(void)
{
    return SPI_WriteByte(DUMMY_BYTE);
}



// 错误日志

static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    FLASH_ERROR("SPI 等待超时! errorCode=%d", errorCode);
    return 0;
}