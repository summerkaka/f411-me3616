/**
  ******************************************************************************
  * @file    Project/src/usart_app.c
  * @author  xia tian
  * @version V0.00
  * @date    2018.10.26
  * @brief   usart_app.c
  ******************************************************************************
  */
  
#include "stm32f4xx_hal.h"
#include "stdint.h"

#define DMA_BUF_SIZE 256

uint8_t usart2_dma[DMA_BUF_SIZE] = {0};
uint8_t usart6_dma[DMA_BUF_SIZE] = {0};

void usart_listen(UART_HandleTypeDef *huart)
{
    
}

void usart_config(void)
{
    
}

void HAL_UART_RxIdleCallback(UART_HandleTypeDef *huart)
{
    
}
