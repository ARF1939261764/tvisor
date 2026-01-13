#ifndef __TVISOR_DEV_UART16550_H
#define __TVISOR_DEV_UART16550_H


#include <stdint.h>

struct tvisor_dev_uart16550_ctx_struct_t;
typedef struct tvisor_dev_uart16550_ctx_struct_t tvisor_dev_uart16550_ctx_t;


typedef int (* tvisor_dev_uart16550_func_rx_t)(tvisor_dev_uart16550_ctx_t *ctx,uint8_t data);
typedef int (* tvisor_dev_uart16550_func_tx_t)(tvisor_dev_uart16550_ctx_t *ctx,uint8_t data);

struct tvisor_dev_uart16550_ctx_struct_t{
    tvisor_dev_uart16550_func_rx_t func_rx;
    tvisor_dev_uart16550_func_tx_t func_tx;
    uint8_t tx_fifo_buffer[64];
    uint8_t rx_fifo_buffer[64];
};

#endif
