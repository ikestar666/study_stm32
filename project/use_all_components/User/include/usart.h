#ifndef __USART_H
#define __USART_H

#include "stm32f10x_usart.h"
#include <stdio.h>

// ∫Í∂®“Â
#define USARTx                  USART2
#define USARTx_CLK_CMD          RCC_APB1PeriphClockCmd
#define USARTx_CLK              RCC_APB1Periph_USART2
#define USARTx_IRQ              USART2_IRQn

#define USARTx_TX_PORT          GPIOA
#define USARTx_RX_PORT          GPIOA
#define USARTx_RX_PIN           GPIO_Pin_3
#define USARTx_TX_PIN           GPIO_Pin_2
#define USARTx_GPIO_CLK_CMD     RCC_APB2PeriphClockCmd
#define USARTx_GPIO_CLK         RCC_APB2Periph_GPIOA
#define USARTx_AFIO_CLK_CMD         RCC_APB2PeriphClockCmd
#define USARTx_AFIO_CLK                 RCC_APB2Periph_AFIO


void MyUSART_Init(void);
void USART_TI_Cmd(void);

void USART_SentByte(USART_TypeDef *pUSARTx, uint8_t ch);
void USART_SentHalfWord(USART_TypeDef *pUSARTx, uint16_t w);
void USART_SentWord(USART_TypeDef *pUSARTx, uint32_t w);
void USART_SentArray(USART_TypeDef *pUSARTx, uint32_t *array, int len);
void USART_SentString(USART_TypeDef *pUSARTx, const char *str);

uint8_t USART_ReceiveByte(USART_TypeDef *pUSARTx);
uint16_t USART_ReceiveHalfWord(USART_TypeDef *pUSARTx);
uint32_t USART_ReceiveWord(USART_TypeDef *pUSARTx);
void USART_ReceiveArray(USART_TypeDef *pUSARTx, uint32_t *str, int len);
void USART_ReceiveString(USART_TypeDef *pUSARTx, char *str);

int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif
