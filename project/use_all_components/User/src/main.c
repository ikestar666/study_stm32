#include "exti.h"
#include "key.h"
#include "led.h"
#include "nvic.h"
#include "stm32f10x.h" // �൱��51��Ƭ���е�  #include <reg51.h>
#include "usart.h"
#include "utils.h"
#include "i2c_ee.h"
#include "spi_flash.h"
#include "ff.h"
#include <stdio.h>




// ����ȫ�ֱ���
volatile int KeyPressed = 0;

FATFS fs;                                                   /* FatFs�ļ�ϵͳ���� */
MKFS_PARM mkfs_parm;
FIL fnew;                                                   /* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;                                /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024] = {0};      /* �������� */
BYTE WriteBuffer[] =              /* д������*/
    "��ӭʹ��Ұ��STM32������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";
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

// ͨ�����ܴ�������ʵ��
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
        printf("�����������Ϊ");
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
            printf("������ȷ\n");
            Blink(LED_GREEN_PORT, LED_GREEN_PIN, 3, 200);
            break;
        }
        else
        {
            printf("�������������\n");
            Blink(LED_RED_PORT, LED_RED_PIN, 3, 200);
            printf("��������Ϊ");
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

    // NVIC�ж�����
    MyNVIC_Config();

    // EXTI �ж�����
    KEY_EXTI_Init();
    KEY1_EXTI_Cmd(ENABLE);
    KEY2_EXTI_Cmd(ENABLE);


    // ��ʼ������
    MyUSART_Init();
    USART_Cmd(USARTx, ENABLE);
    USART_SentString(USARTx, "USART��ʼ���ɹ�\n");

    LEDInit();
    USART_SentString(USARTx, "LED��ʼ���ɹ�\n");

    KeyInit();
    USART_SentString(USARTx, "KEY��ʼ���ɹ�\n");

    USART_SentString(USARTx, "��ʼ����LED����\n");
    TestLED();
    USART_SentString(USARTx, "LED��������\n");

    I2C_EEPROM_Configu();
    USART_SentString(USARTx, "I2C��ʼ���ɹ�\n");
    // I2C_EEPROM_BufferWrite(PassCode, 2, 6);

    SPI_FLASH_Configu();
    USART_SentString(USARTx, "SPI��ʼ���ɹ�\n");
    SPI_FLASH_BufferRead(music, 0x00, 1024);


    USART_SentString(USARTx, "������������룺\n");

    // USART �ж�����(�ݲ�)
    //USART_TI_Cmd();
    res = PassCodeCheck();

    if (res == 1)
    {
        USART_SentString(USARTx, "����������ȷ���ѿ���\n");

        USART_SentString(USARTx, "�����͵ƹ���\n");
        ShowMusicLed(music, 1024);
        CloseAllLED();
        USART_SentString(USARTx, "�ƹ���չʾ���\n");
    }

    USART_SentString(USARTx, "�����̵���˸���κ�ػ�\n");
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

    // ��ʼ������
    MyUSART_Init();

    I2C_EEPROM_Configu();

//    // ���Զ�д�����ֽ�
    I2C_EEPROM_ByteWrite(125, 0);
    I2C_EE_WaitEepromStandbyState();

    // ���Զ�дһҳ
    I2C_EEPROM_PageWrite(writeBuff, 0, 8);
    I2C_EE_WaitEepromStandbyState();

    // �������д�����ַ�
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
            printf("����û����");
            break;
        }
    }
    printf("���ݶ�������");

    I2C_EEPROM_BufferWrite(mm, 2, 6);

    while (1)
    {}
}

int testFLASH()
{


    uint8_t music[] =
    {
        // ǰ��
        6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1, 6, 6, 6, 6, 5, 3, 3, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1,

        // ����
        5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1, 5, 5, 6, 5, 3, 2, 1, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1, 6, 5, 6, 1, 3,
        3, 2, 3, 2, 1, 6, 5, 6, 1, 3, 3, 2, 3, 2, 1,

        // ����
        6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6,
        1, 3, 2, 1, 2, 3, 2, 1, 6, 5, 6, 1, 3, 2, 1, 2, 3, 2, 1
    };
    const u8 bufferSize = countOfArr(music);
    u8 pData[bufferSize] = {0};
    u32 jedecId;
    u16 i = 0;


    // ��ʼ������
    MyUSART_Init();
    SPI_FLASH_Configu();
    jedecId = SPI_FLASH_ReadDeviceID();
    printf("jedecId=%x", jedecId);
    // ���ڿ�ʼ��ǰ����page��������
    SPI_FLASH_BufferRead(pData, 0x00, bufferSize);
    printf("��һ��ҳ������\n");
    for (i = 0; i < bufferSize; i++)
    {
        printf("%x ", pData[i]);
    }
    // ������һ������
    SPI_FLASH_EraseSector(0x00);

    // ���ڿ�ʼ��ǰ����page��������
    SPI_FLASH_BufferRead(pData, 0x00, bufferSize);
    printf("�������һ��ҳ������\n");
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
            printf("����û����");
            break;
        }
    }
    if (i ==  bufferSize)
    {
        printf("���ݶ�������");
    }

    while (1)
    {}

}

void testFatFs()
{
    int i = 0;
    /* ��ʼ��LED */
    LEDInit();
    OpenBlueLED();

    /* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
    MyUSART_Init();
    printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
    printf("\r\n ʹ��ָ���ߵװ�ʱ ���Ͻ�����λ�� ��Ҫ��PC0������ñ ��ֹӰ��PC0��SPIFLASHƬѡ�� \r\n");

    //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
    //��ʼ������������������
    //f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
    res_flash = f_mount(&fs, "1:", 1);

//    /*----------------------- ��ʽ������ -----------------*/
//    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if (res_flash == FR_NO_FILESYSTEM)
    {
        printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
        /* ��ʽ�� */
        res_flash = f_mkfs("1:", 0, work, sizeof work);

        if (res_flash == FR_OK)
        {
            printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
            SPI_FLASH_BufferRead(ReadBuffer, 512 << 12, 1024);
            printf("��һ������=\r\n");
            for (i = 0; i < 1024; i++)
            {
                printf("%x", ReadBuffer[i]);
            }
        }
        else
        {
            OpenRedLED();
            printf("������ʽ��ʧ�ܡ����� ��%d)\r\n ", res_flash);
            while (1);
        }
    }
    else if (res_flash != FR_OK)
    {
        printf("���ⲿFlash�����ļ�ϵͳʧ�� ��%d��\r\n", res_flash);
        while (1);
    }
    else
    {
        printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");

    }


//    /*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
//    /* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
//    printf("\r\n****** ���������ļ�д�����... ******\r\n");
//    res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    if (res_flash == FR_OK)
//    {
//        printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
//        /* ��ָ���洢������д�뵽�ļ��� */
//        res_flash = f_write(&fnew, WriteBuffer, sizeof(WriteBuffer), &fnum);
//        if (res_flash == FR_OK)
//        {
//            printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n", fnum);
//            printf("�����ļ�д�������Ϊ��\r\n%s\r\n", WriteBuffer);
//        }
//        else
//        {
//            printf("�����ļ�д��ʧ�ܣ�(%d)\n", res_flash);
//        }
//        /* ���ٶ�д���ر��ļ� */
//        f_close(&fnew);
//    }
//    else
//    {
//        OpenRedLED();
//        printf("������/�����ļ�ʧ�ܡ�:((%d))\r\n", res_flash);
//    }

    /*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
    printf("****** ���������ļ���ȡ����... ******\r\n");
    res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ);
    if (res_flash == FR_OK)
    {
        OpenGreenLED();
        printf("�����ļ��ɹ���\r\n");
        res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (res_flash == FR_OK)
        {
            printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fnum);
            printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            printf("�����ļ���ȡʧ�ܣ�(%d)\n", res_flash);
        }
    }
    else
    {
        OpenRedLED();
        printf("�������ļ�ʧ�ܡ�\r\n");
    }
    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);

    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
    f_mount(NULL, "1:", 1);

    /* ������ɣ�ͣ�� */
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
