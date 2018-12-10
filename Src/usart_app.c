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
#include "string.h"

#include "usart.h"

#define DMA_BUF_SIZE 256
#define UART_PORT    huart6

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

/**
  * @brief  send AT command to ME3616 module
  * @param  tx_string: command sent to module
            rx_buf: buffer to store feedback
            tx_size: 
            rx_size: not stop until receive rx_size byte message or timeout
            timeout: wait time for receive feedback, 0 if no need to wait for feedback
  * @retval 0: success, negative: fail
  */
int8_t command_send(char const *tx_string, char *rx_buf, uint16_t tx_size, uint16_t rx_size, uint16_t timeout)
{
    printf("send AT command:\n\r");
    
    for (uint16_t i = 0; i < tx_size; i++) {
        printf("%x ", tx_string[i]);
    }
    printf("\n\r");
    if (HAL_UART_Transmit(&UART_PORT, (uint8_t *)tx_string, tx_size, 100) != HAL_OK) {
        //printf("fail to send ATcommand"#tx_string"\n\r");
        snprintf(rx_buf, sizeof(rx_buf), "%s%s", "fail to send ATcommand\n\r");
        printf("%s", rx_buf);
        return -1;
    }
    memset(rx_buf, 0, 256);
    if (timeout) {
        HAL_UART_Receive(&UART_PORT, (uint8_t *)rx_buf, rx_size, timeout);
        if (strlen(rx_buf) == 0) {
            printf("fail to get answer\n\r");
            return -1;
        } else {
            printf("get answer: %s\n\r", rx_buf);
            return 0;
        }
    } else 
        return 0;

}







