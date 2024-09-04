#include "exti.h"
#include "key.h"
#include "led.h"
#include "nvic.h"
#include "stm32f10x.h" // 相当于51单片机中的  #include <reg51.h>
#include "usart.h"
#include "utils.h"
#include "i2c_ee.h"
#include "spi_flash.h"
#include "ff.h"
#include <stdio.h>




// 定义全局变量
volatile int KeyPressed = 0;

FATFS fs;                                                   /* FatFs文件系统对象 */
MKFS_PARM mkfs_parm;
FIL fnew;                                                   /* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;                                /* 文件成功读写数量 */
BYTE ReadBuffer[1024] = {0};      /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
    "欢迎使用野火STM32开发板 今天是个好日子，新建文件系统测试文件\r\n";
BYTE work[4096] = {0};


void TestLED(void)
{
    uint32_t time = 200;
    OpenRedLED();
    Delay_ms(time);

    OpenGreenLED();
    Delay_ms(time);

    OpenBlueLED();
    Delay_ms(time);

    OpenCyanLED();
    Delay_ms(time);

    OpenMagentaLED();
    Delay_ms(time);

    OpenYellowLED();
    Delay_ms(time);

    OpenWhiteLED();
    Delay_ms(time);
    CloseAllLED();
    Delay_ms(time);
    OpenWhiteLED();
    Delay_ms(time);
    CloseAllLED();
    Delay_ms(time);
    OpenWhiteLED();
    Delay_ms(time);
    CloseAllLED();
}

void ShowMusicLed(uint8_t *music, uint16_t music_len)
{
    int i;
    for (i = 0; i < music_len; i++)
    {
        if (music[i] == 0xFF)
        {
            break;
        }
        if (music[i] == 1)
        {
            OpenRedLED();
        }
        else if (music[i] == 2)
        {
            OpenGreenLED();
        }
        else if (music[i] == 3)
        {
            OpenBlueLED();
        }
        else if (music[i] == 4)
        {
            OpenMagentaLED();
        }
        else if (music[i] == 5)
        {
            OpenYellowLED();
        }
        else if (music[i] == 6)
        {
            OpenCyanLED();
        }
        else if (music[i] == 7)
        {
            OpenWhiteLED();
        }
        else
        {
            CloseAllLED();
        }
        Delay_ms(100);
    }
    CloseAllLED();
}

// 通过接受窗口输入实现
int PassCodeCheck()
{
    u8 PassCode[6] = {0};
    u16 inputPassCode[6] = {0};
    int i = 0;
    I2C_EEPROM_SequentialRead(PassCode, 2, 6);

    while (1)
    {
        i = 0;
        for (i = 0; i < 6; i++)
        {
            inputPassCode[i] = USART_ReceiveByte(USARTx);
        }
        printf("您输入的密码为");
        for (i = 0; i < 6; i++)
        {
            printf("%d ", inputPassCode[i]);
        }
        i = 0;
        while (inputPassCode[i] == PassCode[i] && i < 6)
        {
            i++;
        }

        if (i == 6)
        {
            printf("密码正确\n");
            Blink(LED_GREEN_PORT, LED_GREEN_PIN, 3, 200);
            break;
        }
        else
        {
            printf("密码错误，请重试\n");
            Blink(LED_RED_PORT, LED_RED_PIN, 3, 200);
            printf("开机密码为");
            for (i = 0; i < 6; i++)
            {
                printf("%d ", PassCode[i]);
            }
        }
    }
    return 1;
}


int main1(void)
{
    uint16_t res = 0;
    uint8_t music[256] = {0};

    // NVIC中断配置
    MyNVIC_Config();

    // EXTI 中断配置
    KEY_EXTI_Init();
    KEY1_EXTI_Cmd(ENABLE);
    KEY2_EXTI_Cmd(ENABLE);


    // 初始化外设
    MyUSART_Init();
    USART_Cmd(USARTx, ENABLE);
    USART_SentString(USARTx, "USART初始化成功\n");

    LEDInit();
    USART_SentString(USARTx, "LED初始化成功\n");

    KeyInit();
    USART_SentString(USARTx, "KEY初始化成功\n");

    USART_SentString(USARTx, "开始测试LED功能\n");
    TestLED();
    USART_SentString(USARTx, "LED功能正常\n");

    I2C_EEPROM_Configu();
    USART_SentString(USARTx, "I2C初始化成功\n");
    // I2C_EEPROM_BufferWrite(PassCode, 2, 6);

    SPI_FLASH_Configu();
    USART_SentString(USARTx, "SPI初始化成功\n");
    SPI_FLASH_BufferRead(music, 0x00, 1024);


    USART_SentString(USARTx, "请输出开机密码：\n");

    // USART 中断配置(暂不)
    //USART_TI_Cmd();
    res = PassCodeCheck();

    if (res == 1)
    {
        USART_SentString(USARTx, "开机密码正确，已开机\n");

        USART_SentString(USARTx, "请欣赏灯光秀\n");
        ShowMusicLed(music, 1024);
        CloseAllLED();
        USART_SentString(USARTx, "灯光秀展示完毕\n");
    }

    USART_SentString(USARTx, "将在绿灯闪烁三次后关机\n");
    Blink(LED_GREEN_PORT, LED_GREEN_PIN, 3, 500);

    return 0;
}



int testEEPROM()
{

    uint8_t writeBuff[10] = {7, 6, 5, 4, 3, 2, 1, 0};
    uint8_t readBuff[10] = {0};
    uint8_t writeAllBuff[128] = {0};
    u8 mm[] = {1, 2, 3, 3, 2, 1};
    int i;
    int j;

    for (j = 0; j < 128; j++)
    {
        writeAllBuff[j] = j * 2;
    }

    // 初始化外设
    MyUSART_Init();

    I2C_EEPROM_Configu();

//    // 测试读写单个字节
    I2C_EEPROM_ByteWrite(125, 0);
    I2C_EE_WaitEepromStandbyState();

    // 测试读写一页
    I2C_EEPROM_PageWrite(writeBuff, 0, 8);
    I2C_EE_WaitEepromStandbyState();

    // 测试随机写任意字符
    I2C_EEPROM_BufferWrite(writeAllBuff, 0, 128);

    I2C_EEPROM_SequentialRead(readBuff, 0, 128);
    for (i = 0; i < 128; i++)
    {
        if (readBuff[i] == writeAllBuff[i])
        {
            printf("%d ", readBuff[i]);
        }
        else
        {
            printf("数据没对上");
            break;
        }
    }
    printf("数据都对上了");

    I2C_EEPROM_BufferWrite(mm, 2, 6);

    while (1)
    {}
}

int testFLASH()
{


    uint8_t music[] =
    {
        // 前奏
        6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1, 6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1,

        // 主歌
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // 副歌
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1,

        // 过渡
        5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1, 5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1,

        // 主歌
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // 副歌
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1
    };
    const u8 bufferSize = countOfArr(music);
    u8 pData[bufferSize] = {0};
    u32 jedecId;
    u16 i = 0;


    // 初始化外设
    MyUSART_Init();
    SPI_FLASH_Configu();
    jedecId = SPI_FLASH_ReadDeviceID();
    printf("jedecId=%x", jedecId);
    // 现在开始读前两个page区的内容
    SPI_FLASH_BufferRead(pData, 0x00, bufferSize);
    printf("第一个页的内容\n");
    for (i = 0; i < bufferSize; i++)
    {
        printf("%x ", pData[i]);
    }
    // 擦除第一个扇区
    SPI_FLASH_EraseSector(0x00);

    // 现在开始读前两个page区的内容
    SPI_FLASH_BufferRead(pData, 0x00, bufferSize);
    printf("擦除后第一个页的内容\n");
    for (i = 0; i < bufferSize; i++)
    {
        printf("%x ", pData[i]);
    }
//
    SPI_FLASH_BufferWrite(music, 0x00, bufferSize);

    SPI_FLASH_BufferRead(pData, 0x00, bufferSize);


    for (i = 0; i < bufferSize; i++)
    {
        if (music[i] == pData[i])
        {
            printf("%x ", pData[i]);
        }
        else
        {
            printf("数据没对上");
            break;
        }
    }
    if (i ==  bufferSize)
    {
        printf("数据都对上了");
    }

    while (1)
    {}

}

void testFatFs()
{
    int i = 0;
    /* 初始化LED */
    LEDInit();
    OpenBlueLED();

    /* 初始化调试串口，一般为串口1 */
    MyUSART_Init();
    printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
    printf("\r\n 使用指南者底板时 左上角排针位置 不要将PC0盖有跳帽 防止影响PC0做SPIFLASH片选脚 \r\n");

    //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
    //初始化函数调用流程如下
    //f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
    res_flash = f_mount(&fs, "1:", 1);

//    /*----------------------- 格式化测试 -----------------*/
//    /* 如果没有文件系统就格式化创建创建文件系统 */
    if (res_flash == FR_NO_FILESYSTEM)
    {
        printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
        /* 格式化 */
        res_flash = f_mkfs("1:", 0, work, sizeof work);

        if (res_flash == FR_OK)
        {
            printf("》FLASH已成功格式化文件系统。\r\n");
            SPI_FLASH_BufferRead(ReadBuffer, 512 << 12, 1024);
            printf("第一个扇区=\r\n");
            for (i = 0; i < 1024; i++)
            {
                printf("%x", ReadBuffer[i]);
            }
        }
        else
        {
            OpenRedLED();
            printf("《《格式化失败。》》 （%d)\r\n ", res_flash);
            while (1);
        }
    }
    else if (res_flash != FR_OK)
    {
        printf("》外部Flash挂载文件系统失败 （%d）\r\n", res_flash);
        while (1);
    }
    else
    {
        printf("》文件系统挂载成功，可以进行读写测试\r\n");

    }


//    /*----------------------- 文件系统测试：写测试 -------------------*/
//    /* 打开文件，每次都以新建的形式打开，属性为可写 */
//    printf("\r\n****** 即将进行文件写入测试... ******\r\n");
//    res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    if (res_flash == FR_OK)
//    {
//        printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
//        /* 将指定存储区内容写入到文件内 */
//        res_flash = f_write(&fnew, WriteBuffer, sizeof(WriteBuffer), &fnum);
//        if (res_flash == FR_OK)
//        {
//            printf("》文件写入成功，写入字节数据：%d\n", fnum);
//            printf("》向文件写入的数据为：\r\n%s\r\n", WriteBuffer);
//        }
//        else
//        {
//            printf("！！文件写入失败：(%d)\n", res_flash);
//        }
//        /* 不再读写，关闭文件 */
//        f_close(&fnew);
//    }
//    else
//    {
//        OpenRedLED();
//        printf("！！打开/创建文件失败。:((%d))\r\n", res_flash);
//    }

    /*------------------- 文件系统测试：读测试 --------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ);
    if (res_flash == FR_OK)
    {
        OpenGreenLED();
        printf("》打开文件成功。\r\n");
        res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (res_flash == FR_OK)
        {
            printf("》文件读取成功,读到字节数据：%d\r\n", fnum);
            printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            printf("！！文件读取失败：(%d)\n", res_flash);
        }
    }
    else
    {
        OpenRedLED();
        printf("！！打开文件失败。\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL, "1:", 1);

    /* 操作完成，停机 */
    while (1)
    {
    }
}


int main()
{
    //testFLASH();

    //testEEPROM();

    testFatFs();
}
