/**
  ******************************************************************************
  * @file    Project/src/coap.c
  * @author
  * @version V0.00
  * @date
  * @brief   coap.c
  ******************************************************************************
  */
  
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "coap.h"
#include "ME3616.h"

/**
  * @brief  build up a COAP message that will be sent via ME3616 to easy-iot. buffer should be cleared before call this function
  * @param  buffer: user should clear the buffer before call this function.
  * @retval total length of the coap message
  */
int8_t set_message(uint8_t version, uint8_t msg_type, stPublicMsg *publicMsg, stUserMsg *userMsg, uint8_t *buffer)
{
//    char string[16] = {0};
    uint8_t wr_index = 4;
    int8_t i = 0;
    uint32_t crc = 0;
    uint16_t total_length = userMsg->msg_len + 49;  // infer from easy-iot device simulation message
    
    // copy coap-message id, type, total length
    buffer[0] = 0x01;
    buffer[1] = msg_type;
    buffer[2] = total_length >> 8 & 0xff;
    buffer[3] = total_length & 0xff;
    
    // copy public-message
    memcpy(&buffer[wr_index], publicMsg, 7);    // including 'dtag', 'battery-level', 'signal-strength'
    wr_index += 7;
    memcpy(&buffer[wr_index], imei, 15);
    wr_index += 15;
    memcpy(&buffer[wr_index], imsi, 15);
    wr_index += 15;
    for (i = 7; i >= 0; i--) {
        buffer[wr_index + i] = publicMsg->time_stamp >> (7 - i)*8;
    }
    wr_index += 8;                              // count += (strlen(imei) + strlen(imsi) + strlen(time_stamp));
    
    // copy user-message
    memcpy(&buffer[wr_index], userMsg, 3);
    wr_index += 3;
    memcpy(&buffer[wr_index], userMsg->tlv, 3);
    wr_index += 3;
    memcpy(&buffer[wr_index], userMsg->tlv->data, userMsg->tlv->tlv_len);
    wr_index += userMsg->tlv->tlv_len;
    
    // calculate crc
    for (i = 0; i < wr_index; i++) {
        crc += buffer[i];
    }
    crc %= 256;
    buffer[wr_index] = crc;
    
    return wr_index;
}




