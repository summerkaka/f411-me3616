/**
  ******************************************************************************
  * @file    Project/src/coap.h
  * @author
  * @version V0.00
  * @date
  * @brief   coap.h
  ******************************************************************************
  */

int8_t command_send(char const *tx_string, char *rx_buf, uint16_t tx_size, uint16_t rx_size, uint16_t wait_time);