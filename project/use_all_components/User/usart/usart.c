#include "usart.h"
#include <stdio.h>

void MyUSART_Init(void)
{

    GPIO_InitTypeDef USARTx_RX_InitStruct;
    GPIO_InitTypeDef USARTx_TX_InitStruct;
    USART_InitTypeDef USARTx_InitStruct;

    // 配置时钟
    // USART
    USARTx_CLK_CMD(USARTx_CLK, ENABLE);
    // GPIO RX
    USARTx_GPIO_CLK_CMD(USARTx_GPIO_CLK, ENABLE);
    // AFIO TX
    USARTx_AFIO_CLK_CMD(USARTx_AFIO_CLK, ENABLE);

    // 配置IO
    //RX
    USARTx_RX_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    USARTx_RX_InitStruct.GPIO_Pin = USARTx_RX_PIN;
    GPIO_Init(USARTx_RX_PORT, &USARTx_RX_InitStruct);
    //TX
    USARTx_TX_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    USARTx_TX_InitStruct.GPIO_Pin = USARTx_TX_PIN;
    USARTx_TX_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(USARTx_TX_PORT, &USARTx_TX_InitStruct);

    // 配置USART
    USARTx_InitStruct.USART_BaudRate = 115200;
    USARTx_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USARTx_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USARTx_InitStruct.USART_Parity = USART_Parity_Even;
    USARTx_InitStruct.USART_StopBits = USART_StopBits_1;
    USARTx_InitStruct.USART_WordLength = USART_WordLength_9b;
    USART_Init(USARTx, &USARTx_InitStruct);

    // 使能USART
    USART_Cmd(USARTx, ENABLE);
}

void USART_TI_Cmd(void)
{
    // 使能接受和发送中断
    uint16_t IE_Type = USART_IT_RXNE;
    USART_ITConfig(USARTx, IE_Type, ENABLE);
}

void USART_SentByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
    USART_SendData(pUSARTx, ch);

    // 等待发送数据寄存器的数据被完全送入移位寄存器
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

}

void USART_SentHalfWord(USART_TypeDef *pUSARTx, uint16_t w)
{
    uint16_t w_h = (w & 0xff00) >> 8;
    uint16_t w_l = (w & 0x00ff);
    // 先发高8位
    USART_SentByte(pUSARTx, w_h);
    USART_SentByte(pUSARTx, w_l);
}

void USART_SentWord(USART_TypeDef *pUSARTx, uint32_t w)
{
    uint32_t w_h = (w & 0xffff0000) >> 16;
    uint32_t w_l = (w & 0x0000ffff);

    // 先发高16位
    USART_SentHalfWord(pUSARTx, w_h);
    USART_SentHalfWord(pUSARTx, w_l);
}

void USART_SentArray(USART_TypeDef *pUSARTx, uint32_t *array, int num)
{
    int i;
    for (i = 0; i < num; i++)
    {
        USART_SentWord(pUSARTx, array[i]);
    }
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

void USART_SentString(USART_TypeDef *pUSARTx, const char *str)
{
    int i = 0;
    do
    {
        USART_SentByte(pUSARTx, *(str + i));
        i++;
    }
    while (*(str + i) != '\0');
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

uint8_t USART_ReceiveByte(USART_TypeDef *pUSARTx)
{
    uint16_t ret;

    // 等待发送数据寄存器的数据被完全送入移位寄存器
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_RXNE) == RESET);
    ret = USART_ReceiveData(pUSARTx);

    return (uint8_t)(ret & 0xFF);
}
uint16_t USART_ReceiveHalfWord(USART_TypeDef *pUSARTx)
{
    uint16_t ret = 0;
    uint16_t ret_h = USART_ReceiveByte(pUSARTx);
    uint16_t ret_l = USART_ReceiveByte(pUSARTx);

    ret |= (ret_h << 8) & 0xFF00;
    ret |= ret_l & 0xFF;
}
uint32_t USART_ReceiveWord(USART_TypeDef *pUSARTx)
{
    uint32_t ret = 0;
    uint32_t ret_h = USART_ReceiveHalfWord(pUSARTx);
    uint32_t ret_l = USART_ReceiveHalfWord(pUSARTx);

    ret |= (ret_h << 16) & 0xFFFF0000;
    ret |= ret_l & 0xFFFF;
}

void USART_ReceiveArray(USART_TypeDef *pUSARTx, uint32_t *str, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        uint32_t w = USART_ReceiveWord(pUSARTx);
        *(str + i) = w;
    }
}

void USART_ReceiveString(USART_TypeDef *pUSARTx, char *str)
{
    int i = 0;
    while (*(str + i) != '\0')
    {
        char ch = (char) USART_ReceiveByte(pUSARTx);
        str[i] = ch;
        i++;
    }
}

int fputc(int ch, FILE *f)
{
    USART_SendData(USARTx, (uint8_t) ch);

    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

    return (ch);
}

int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

    return (int) USART_ReceiveData(USARTx);
}