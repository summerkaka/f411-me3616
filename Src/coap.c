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


static void copy_to_pkt(uint8_t *dest, const void *pdata, uint8_t size)
{
    while (size--) 
    {
        *dest++ = *((uint8_t *)pdata + size);
    }
}

/**
  * @brief  build up a COAP message that will be sent via ME3616 to easy-iot. buffer should be cleared before call this function
  * @param  buffer: user should clear the buffer before call this function.
  * @retval total length of the coap message
  */
int8_t set_message(uint8_t version, uint8_t msg_type, stPublicMsg *publicMsg, stUserMsg *userMsg, uint8_t *buffer)
{
//    char string[16] = {0};
    uint16_t wr_index = 4;
    int16_t i = 0;
    uint32_t crc = 0;
    
    // copy coap-message id, type
    buffer[0] = 0x01;
    buffer[1] = msg_type;
    
    // copy public-message
    copy_to_pkt(&buffer[wr_index], &publicMsg->dtag, 2);            // 'dtag'
    wr_index += 2;
    copy_to_pkt(&buffer[wr_index], &publicMsg->battery_level, 1);   // 'battery-level'
    wr_index++;
    copy_to_pkt(&buffer[wr_index], &publicMsg->signal_strength, 4); // 'signal-strength'
    wr_index += 4;
    memcpy(&buffer[wr_index], imei, 15);
    wr_index += 15;
    memcpy(&buffer[wr_index], imsi, 15);
    wr_index += 15;
    copy_to_pkt(&buffer[wr_index], &publicMsg->time_stamp, 8);
    wr_index += 8;
    
    // copy user-message
    buffer[wr_index++] = userMsg->msg_id;
    copy_to_pkt(&buffer[wr_index], &userMsg->msg_len, 2);
    wr_index += 2;
    buffer[wr_index++] = userMsg->tlv->tlv_type;
    copy_to_pkt(&buffer[wr_index], &userMsg->tlv->tlv_len, 2);
    wr_index += 2;
    copy_to_pkt(&buffer[wr_index], userMsg->tlv->pdata, userMsg->tlv->tlv_len);
    wr_index += userMsg->tlv->tlv_len;
    
    // write total length
    i = wr_index - 3;                                                           // total_length is wr_index+1, msg_len is total_length-4
    copy_to_pkt(&buffer[2], &i, 2);
    
    // calculate crc
    for (i = 0; i < wr_index; i++) {
        crc += buffer[i];
    }
    crc %= 256;     // crc &= 0xff;
    buffer[wr_index] = crc;


    printf("set message: \n\r");
    for (i = 0; i <= wr_index; i++) 
        printf("%x ", buffer[i]);
    printf("\n\r");

    
    return wr_index+1;
}




