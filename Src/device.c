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

#include "coap.h"
#include "ME3616.h"


uint8_t   battery_level = 70;
int32_t   signal_strength = -30;
float     adc_value = 0.0f;

void device_report(void)
{
    uint8_t coap_buffer[256] = {0};
    stPublicMsg public_msg;
    stUserMsg user_msg;
    stTLV tlv;
    uint8_t sensor_value = 1;
    int8_t ret;

    public_msg.dtag = 1;
    public_msg.battery_level = battery_level;
    public_msg.signal_strength = signal_strength;
    public_msg.time_stamp = HAL_GetTick();

    tlv.tlv_type = 24;
    tlv.tlv_len = 1;
    tlv.data = &sensor_value;

    user_msg.msg_id = 1;
    user_msg.msg_len = 7;
    user_msg.tlv = &tlv;

    ret = set_message(1, 24, &public_msg, &user_msg, coap_buffer);
    if (ret > 0)
        lwm2mcli_send((char *)coap_buffer, ret);
}
