/**
  ******************************************************************************
  * @file    Project/src/device.c
  * @author  xia tian
  * @version V0.00
  * @date    2018.10.22
  * @brief   device.c
  ******************************************************************************
  */
  
#include "stdint.h"
#include "string.h"

#include "coap.h"
#include "ME3616.h"
#include "usart_app.h"

uint8_t     battery_level = 70;
int32_t     signal_strength = -50;
uint8_t     power_status = 0;
int8_t      aio_value = 15;
const char  debug_msg[] = "AT+M2MCLISEND=01F00035000150FFFFFFCE3836393636323033303639373835353436303131333030393530303534380000016782AFD6486400040A0001008A\r";
char        rx_buf[8];  
void device_report(void)
{
    uint8_t coap_buffer[256] = {0};
    stPublicMsg public_msg;
    stUserMsg user_msg;
    stTLV tlv0;
    int8_t len;

    public_msg.dtag = 1;
    public_msg.battery_level = 0x50;
    battery_level = battery_level == 0 ? 100 : battery_level;
    public_msg.signal_strength = signal_strength;
    public_msg.time_stamp = 0x0000016782AFD648;

    tlv0.tlv_type = 10;
    tlv0.tlv_len = 1;
    tlv0.pdata = &power_status;

    user_msg.msg_id = 100;
    user_msg.msg_len = 4;
    user_msg.tlv = &tlv0;

    len = set_message(1, 0xf0, &public_msg, &user_msg, coap_buffer);
    if (len > 0)
        lwm2mcli_notify((char *)coap_buffer, len);

    // command_send(debug_msg, rx_buf, strlen(debug_msg), 0, 0);
    
}
