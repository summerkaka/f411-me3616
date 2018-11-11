/**
  ******************************************************************************
  * @file    Project/src/ME3616.h
  * @author
  * @version V0.00
  * @date
  * @brief   ME3616.h
  ******************************************************************************
  */

#include "stdint.h"


extern char imei[];
extern char imsi[];
extern char time_stamp[];

extern int8_t connect_iotnet(void);
extern int8_t lwm2mcli_new(void);
extern int8_t lwm2mcli_del(void);
extern int8_t lwm2mcli_send(char *message, uint8_t length);

