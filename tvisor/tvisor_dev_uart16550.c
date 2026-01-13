#include "tvisor.h"
#include "tvisor_dev.h"
#include "tvisor_dev_uart16550.h"
#include <stddef.h>
#include <stdint.h>

static int tvisor_dev_uart16550_write_register(tvisor_dev_uart16550_ctx_t *ctx, size_t addr_offset, uint64_t value, uint8_t size){

    return TVISOR_STATUS_OK;
}

static int tvisor_dev_uart16550_read_register(tvisor_dev_uart16550_ctx_t *ctx, size_t addr_offset,uint64_t *value,uint8_t size){
    return TVISOR_STATUS_OK;
}

static int tvisor_dev_uart16550_rx_handler(tvisor_dev_uart16550_ctx_t *ctx,uint8_t value){
    return TVISOR_STATUS_OK;
}

static int tvisor_dev_uart16550_init(tvisor_dev_uart16550_ctx_t *ctx){
    ctx->func_rx = tvisor_dev_uart16550_rx_handler;
    return TVISOR_STATUS_OK;
}

const tvisor_dev_ops_t tvisor_dev_uart16550_ops = {
    .init           = tvisor_dev_uart16550_init,
    .read_register  = tvisor_dev_uart16550_read_register,
    .write_register = tvisor_dev_uart16550_write_register,
};
