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
#include "usart_app.h"

#define AT_(NAME)               "AT+"#NAME
#define GET_SIGNAL_STRENGTH     "AT+CESQ\r"
#define GET_IP                  "AT+CGPADDR=1\r"
#define PING(ADDR)              "AT+PING="###ADDR##"\r"     // "AT+PING="#ADDR"\n" also works, but can't use "AT+PING=##ADDR\n", ## would appear in string, 

#define UART_PORT               huart6                      // uart port used to communicate with me3616
#define BUF_SIZE                256

char iot_server_ip[] = "117.60.157.137";
char imei[16] = {0};
char imsi[16] = {0};
char rx_buffer[BUF_SIZE] = {0};
char tx_buffer[BUF_SIZE] = {0};

int8_t lwm2mcli_new(void);
int8_t lwm2mcli_del(void);


int8_t connect_iotnet(void)
{
    uint8_t d_offset = 0;
    
    printf("iot sybsystem initialize...\n\r");
    
    // get imei
    if (command_send(AT_(GSN)"\r", rx_buffer, strlen(AT_(GSN)"\r"), 32, 100) == 0) {
        d_offset = strlen(AT_(GSN)) + 3;
        strncpy(imei, (char *)rx_buffer + d_offset, 15);
    }
    // get_imsi
    if (command_send(AT_(CIMI)"\r", rx_buffer, strlen(AT_(CIMI)"\r"), 32, 100) == 0) {
        d_offset = strlen(AT_(CIMI)) + 3;
        strncpy(imsi, (char *)rx_buffer + d_offset, 15);
    }
    
    // get signal strength
    command_send(AT_(CESQ)"\r", rx_buffer, strlen(AT_(CESQ)"\r"), 256, 100);
  
    // get ip address
    command_send(GET_IP, rx_buffer, strlen(GET_IP), 256, 1000);
    
    // ping easy-iot
    command_send(PING(117.60.157.137), rx_buffer, strlen(PING(117.60.157.137)), 256, 1000);
    
    // create lwm2mclient
    lwm2mcli_del();
    if (lwm2mcli_new() != 0)
        return -1;
    
    return 0;
}

int8_t lwm2mcli_new(void)
{
    memset(tx_buffer, 0, 256);
    memset(rx_buffer, 0, 256);
    strncpy(tx_buffer, AT_(M2MCLINEW=), strlen(AT_(M2MCLINEW=)));
    strcat(tx_buffer, iot_server_ip);
    strcat(tx_buffer, ",5683,");
    strcat(tx_buffer, imei);
    strcat(tx_buffer, ",90\r");
    
    if (command_send(tx_buffer, rx_buffer, strlen(tx_buffer), 256, 10000) == 0) {
         printf("create new M2M client:\n\r %s", rx_buffer);
         return 0;
    } else {
        printf("fail to create new M2M client\n\r");
        return -1;
    }
}

int8_t lwm2mcli_del(void)
{    
    memset(rx_buffer, 0, BUF_SIZE);
    memset(tx_buffer, 0, BUF_SIZE);
    
    if (command_send(AT_(M2MCLIDEL)"\r", rx_buffer, strlen(AT_(M2MCLIDEL)"\r"), 256, 10000) == 0) {
        printf("delete M2M client:\n\r%s", rx_buffer);
        return 0;
    } else {
        printf("fail to delete M2M client\n\r");
        return -1;
    }
}

int8_t lwm2mcli_notify(char *message, uint8_t length)
{
    char s[] = AT_(M2MCLISEND=);
    uint8_t len = strlen(s);
    uint8_t i = 0, j = 0;
    
    memset(tx_buffer, 0, BUF_SIZE);
    memset(rx_buffer, 0, BUF_SIZE);
    memcpy(tx_buffer, s, len);
    do {
        printf("%02X ", message[i]);
        snprintf(tx_buffer + len, 3, "%02X", message[i++]);
        len += 2;
    } while (i < length);
    tx_buffer[len++] = '\r';
    printf("notification: %s\n\r", tx_buffer);
    
    if (command_send(tx_buffer, rx_buffer, len, BUF_SIZE, 0) == 0) {
        printf("send M2M notification:\n\r");
        for (int i = 0; i < len; i++) {
            printf("%x ", tx_buffer[i]);
        }
        printf("\n\r");
        return 0;
    } else {
        printf("fail to send M2M notification\n\r");
        return -1;
    }
}



