#ifndef __UART_H
#define __UART_H

#include "stdint.h"

typedef struct {
  volatile uint32_t tx_fifo;
  volatile uint32_t rx_fifo;
  volatile uint32_t tx_ctrl;
  volatile uint32_t rx_ctrl;
  volatile uint32_t ie;
  volatile uint32_t ip;
  volatile uint32_t div;
}uart_t;

extern void uart_initial(uart_t *periph,uint32_t baund);
extern void uart_putc(uart_t *periph,char baund);
extern char uart_getc(uart_t *periph);

#endif
