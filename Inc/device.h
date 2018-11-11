/**
  ******************************************************************************
  * @file    Project/src/device.h
  * @author  xia tian
  * @version V0.00
  * @date    2018.10.22
  * @brief   device.h
  ******************************************************************************
  */
  
#include "stdint.h"

extern uint8_t    battery_level;
extern int32_t    signal_strength;
extern float      adc_value;

extern void device_report(void);
