#include "tvisor.h"
#include "tvisor_dev.h"
#include "tvisor_dev_uart16550.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "queue.h"


static int tvisor_dev_uart16550_write_register(void *ctx, size_t addr_offset, uint64_t value, uint8_t size){
    tvisor_dev_uart16550_ctx_t *uart_ctx = (tvisor_dev_uart16550_ctx_t *)ctx;
    switch(addr_offset){
        case TVISOR_UART_DAT:{
            uart_ctx->func_tx(ctx,(uint8_t)value);
            break;
        }
        default:{
            break;
        }
    }
    return TVISOR_STATUS_OK;
}

static int tvisor_dev_uart16550_read_register(void *ctx, size_t addr_offset,uint64_t *value,uint8_t size){
    tvisor_dev_uart16550_ctx_t *uart_ctx = (tvisor_dev_uart16550_ctx_t *)ctx;
    switch(addr_offset){
        case TVISOR_UART_DAT:{
            xQueueReceive(uart_ctx->rx_queue,value,0);
            break;
        }
        case TVISOR_UART_LSR:{
            *value = uxQueueMessagesWaiting(uart_ctx->rx_queue) != 0;
            break;
        }
        case 0x80:{
            
            break;
        }
        default:{
            break;
        }
    }
    return TVISOR_STATUS_OK;
}

static int tvisor_dev_uart16550_rx_handler_from_isr(tvisor_dev_uart16550_ctx_t *ctx,uint8_t value){
    xQueueSendFromISR(ctx->rx_queue,&value,0);
    return TVISOR_STATUS_OK;
}

static int tvisor_dev_uart16550_rx_handler(tvisor_dev_uart16550_ctx_t *ctx,uint8_t value){
    xQueueSend(ctx->rx_queue,&value,0);
    return TVISOR_STATUS_OK;
}

static int tvisor_dev_uart16550_init(void *ctx){
    tvisor_dev_uart16550_ctx_t *uart_ctx = (tvisor_dev_uart16550_ctx_t *)ctx;
    uart_ctx->rx_queue = xQueueCreate(64,1);
    if(uart_ctx->rx_queue == NULL){
        while(1);
    }
    uart_ctx->func_rx_from_isr = tvisor_dev_uart16550_rx_handler_from_isr;
    uart_ctx->func_rx          = tvisor_dev_uart16550_rx_handler;
    return TVISOR_STATUS_OK;
}

const tvisor_dev_ops_t tvisor_dev_uart16550_ops = {
    .init           = tvisor_dev_uart16550_init,
    .read_register  = tvisor_dev_uart16550_read_register,
    .write_register = tvisor_dev_uart16550_write_register,
};
