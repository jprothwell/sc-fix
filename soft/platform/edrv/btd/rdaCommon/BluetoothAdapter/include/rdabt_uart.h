#ifndef __RDABT_UART_H__
#define __RDABT_UART_H__
#include "cs_types.h"
typedef void (*UART_RX_CB_FUNC)(u_int8 *buff, u_int32 len); ;


void rda5868_uart_stop(void);

void rda5868_uart_start(void);
void rda5868_uart_configure(unsigned baud,char flow_control);
unsigned int rda5868_uart_rx(void);
unsigned char rda5868_uart_tx(const kal_uint8 *buf, unsigned short num_to_send, unsigned short *num_send);
void rdabt_uart_eint_hdlr(void);
void rdabt_uart_register_eint(void);
void RDA5868_uart_rx_cb(UART_PORT port);
void rdabt_set_uart_rx_cb(UART_RX_CB_FUNC func);
void RDA5868_uart_register_cb(void);

#endif
