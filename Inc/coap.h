/**
  ******************************************************************************
  * @file    Project/src/coap.h
  * @author
  * @version V0.00
  * @date
  * @brief   coap.h
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"

typedef struct {
    uint8_t     tlv_type;
    uint16_t    tlv_len;
    uint8_t     *pdata;
} stTLV;

typedef struct {
    uint8_t     msg_id;
    uint16_t    msg_len;
    stTLV      *tlv;
} stUserMsg;

typedef struct {
    uint16_t        dtag;
    uint8_t         battery_level;
    int32_t         signal_strength;
    const char     *p_imei;
    const char     *p_imsi;
    uint64_t        time_stamp;
} stPublicMsg;

// typedef struct {
//     uint8_t         version;
//     uint8_t         coapmsg_type;
//     uint16_t        coapmsg_len;
//     stPublicMsg    *global_msg;
//     stUserMsg      *user_msg;
// } stCoapMSG;

extern int8_t set_message(uint8_t version, uint8_t msg_type, stPublicMsg *publicMsg, stUserMsg *userMsg, uint8_t *buffer);

