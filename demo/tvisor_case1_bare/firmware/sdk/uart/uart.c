#include "stdint.h"
#include "uart.h"
#include "system.h"

void uart_initial(uart_t *periph,uint32_t baund){
  periph->div = 417;//FREQUENCY_CPU_CLK / baund;
  periph->tx_ctrl  = 1;
  periph->rx_ctrl  = 1;
}

void uart_putc(uart_t *periph,char ch){
  while(periph->tx_fifo & (1<<31));
  periph->tx_fifo = ch;
}
