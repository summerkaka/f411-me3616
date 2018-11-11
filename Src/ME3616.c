/**
  ******************************************************************************
  * @file    Project/src/ME3616.c
  * @author  xia tian
  * @version V0.00
  * @date    2018.10.22
  * @brief   ME3616.c
  ******************************************************************************
  */
  
#include "stm32f4xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "ME3616.h"
#include "usart.h"

#define AT_(NAME)               "AT+"#NAME
#define GET_SIGNAL_STRENGTH     "AT+CESQ\n"
#define GET_IP                  "AT+CGPADDR=1\n"
#define PING(ADDR)              "AT+PING="###ADDR##"\n"     // "AT+PING="#ADDR"\n" also works, but can't use "AT+PING=##ADDR\n", ## would appear in string, 

#define UART_PORT               huart6

char iot_server_ip[] = "117.60.157.137";
char imei[16] = {0};
char imsi[16] = {0};

//static uint16_t string2decimal(const char *data, int16_t bit_num)
//{
//    uint16_t result = 0;
//    int16_t bit_count = bit_num;
//    
//    do {
//        if (*data >= '0' && *data <= '9')
//            result += *data - '0';
//        else if (*data >= 'A' && *data <= 'F')
//            result += *data - 'A' + 10;
//        else if (*data >= 'a' && *data <= 'f')
//            result += *data - 'a' + 10;
//        if (bit_count > 0) 
//            result *= 10;
//    } while (bit_count-- >= 0);

//    return result;
//}

void get_imei(void)
{
    char rx_buffer[32];
    
    HAL_UART_Transmit(&UART_PORT, AT_(GSN)"\n", strlen(AT_(GSN)"\n"), 10);
    //HAL_UART_Receive(&UART_PORT, (uint8_t *)rx_buffer, 32, 5);
    strncpy(imei, rx_buffer, 15);           // No null-character is implicitly appended at the end of destination if source is longer than num
}

void get_imsi(void)
{
    char rx_buffer[32] = {0};
    
    HAL_UART_Transmit(&UART_PORT, AT_(CIMI)"\n", strlen(AT_(CIMI)"\n"), 10);
    //HAL_UART_Receive(&UART_PORT, (uint8_t *)rx_buffer, 32, 5);
    strncpy(imsi, rx_buffer, 15);
}

int8_t connect_iotnet(void)
{
    uint8_t rx_buffer[256] = {0};
//    float signal_strength = 0;
//    uint8_t *command;
//    uint8_t ip_address[13] = {0};
    
    get_imei();
    get_imsi();
    
    // get signal strength
    if (HAL_UART_Transmit(&UART_PORT, GET_SIGNAL_STRENGTH, strlen(GET_SIGNAL_STRENGTH), 10) != HAL_OK)
        return -1;
//    if (HAL_UART_Receive(&UART_PORT, rx_buffer, 256, 10) != HAL_OK) {
//        printf("fail to get iot signal strength\n\r");
//        return -1;
//    }
//    signal_strength = atof(&rx_buffer[7]);
//    printf("signal_strengh is %f\n\r", signal_strength);
    HAL_UART_Receive(&UART_PORT, rx_buffer, 256, 10);
    printf("get signal strength\n\r %s", rx_buffer);
    
    // get ip address
    if (HAL_UART_Transmit(&UART_PORT, GET_IP, strlen(GET_IP), 10) != HAL_OK)
        return -1;
//    if (HAL_UART_Receive(&UART_PORT, rx_buffer, 20, 10) != HAL_OK) {
//        printf("fail to get iot ip address\n\r");
//        return -1;
//    }
//    strncpy(ip_address, &rx_buffer[13], 12);
//    ip_address[12] = '\0';
//    printf("ip_address is %s\n\r", ip_address);
    HAL_UART_Receive(&UART_PORT, rx_buffer, 256, 10);
    printf("get ip_address: \n\r %s", rx_buffer);
    
    // ping easy-iot
    if (HAL_UART_Transmit(&UART_PORT, PING(117.60.157.137), strlen(PING(117.60.157.137)), 10) != HAL_OK)
        return -1;
//    if (HAL_UART_Receive(&UART_PORT, rx_buffer, 20, 10000) != HAL_OK) {
//        printf("fail to get iot ip address\n\r");
//        return -1;
//    }
    HAL_UART_Receive(&UART_PORT, rx_buffer, 256, 10000);
    printf("ping easy-iot result:\n\r %s\n\r", rx_buffer);
    
    return 0;
}

int8_t lwm2mcli_new(void)
{
    char rx_buffer[128] = {0};
    char tx_buffer[128] = {0};
//    uint8_t count = 0;
    
    strncpy(tx_buffer, AT_(M2MCLINEW), strlen(AT_(M2MCLINEW)));
    strcat(tx_buffer, iot_server_ip);
    strcat(tx_buffer, ",5683,");
    strcat(tx_buffer, imei);
    strcat(tx_buffer, ",90\n");
    
    if (HAL_UART_Transmit(&UART_PORT, (uint8_t *)tx_buffer, strlen(tx_buffer), 10) != HAL_OK)
        return -1;
    
    HAL_UART_Receive(&UART_PORT, (uint8_t *)rx_buffer, 256, 10000);
    printf("create new M2M client:\n\r %s", rx_buffer);
    
    return 0;
}

int8_t lwm2mcli_del(void)
{
    char rx_buffer[128] = {0};
    
    if (HAL_UART_Transmit(&UART_PORT, AT_(M2MCLIDEL)"\n", strlen(AT_(M2MCLIDEL)"\n"), 10) != HAL_OK)
        return -1;
    
    HAL_UART_Receive(&UART_PORT, (uint8_t *)rx_buffer, 256, 10000);
    printf("create new M2M client:\n\r %s", rx_buffer);

    return 0;
}

int8_t lwm2mcli_send(char *message, uint8_t length)
{
    char tx_buffer[256] = {0};
    char rx_buffer[256] = {0};
    
    strncpy(tx_buffer, AT_(M2MCLISEND=), strlen(AT_(M2MCLISEND=)));
    strcat(tx_buffer, message);
    
    if (HAL_UART_Transmit(&UART_PORT, (uint8_t *)tx_buffer, strlen(tx_buffer), 10) != HAL_OK)
        return -1;
    
    HAL_UART_Receive(&UART_PORT, (uint8_t *)rx_buffer, 256, 10000);
    printf("create new M2M client:\n\r %s", rx_buffer);
    
    return 0;
}



