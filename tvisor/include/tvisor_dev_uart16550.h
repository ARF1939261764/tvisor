#ifndef __TVISOR_DEV_UART16550_H
#define __TVISOR_DEV_UART16550_H


#include <stdint.h>

typedef int (* tvisor_dev_uart16550_func_rx_t)(uint8_t data);
typedef int (* tvisor_dev_uart16550_func_tx_t)(uint8_t data);

typedef struct{
    tvisor_dev_uart16550_func_rx_t func_rx;
    tvisor_dev_uart16550_func_tx_t func_tx;
}tvisor_dev_uart16550_ctx_t;

#endif
