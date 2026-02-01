#include "tvisor_dev.h"

extern const tvisor_dev_ops_t tvisor_dev_uart16550_ops;
extern const tvisor_dev_ops_t tvisor_dev_mem;

const tvisor_dev_ops_t *tvisor_dev_ops_list[] = {
    [TVISOR_DEV_TYPE_MEM]        = &tvisor_dev_mem,
    [TVISOR_DEV_TYPE_UART_16550] = &tvisor_dev_uart16550_ops
};
