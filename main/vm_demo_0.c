#include "tvisor.h"

static tvisor_vm_ctx_t vm_demo_0_ctx;

    // {.name = "clint",.type = TVISOR_DEV_TYPE_CLINT    ,.start_addr = 0x02000000,.size = 0x000010000},
    // {.name = "plic" ,.type = TVISOR_DEV_TYPE_PLIC     ,.start_addr = 0x02000000,.size = 0x000010000},
    // {.name = "uart0",.type = TVISOR_DEV_TYPE_UART_8250,.start_addr = 0x10000000,.size = 0x000000100},

tvisor_dev_ctx_t vm_demo_0_dev_list[] = {
    {.name = "dram" ,.type = TVISOR_DEV_TYPE_DRAM     ,.start_addr = 0x80000000,.size = 0x000100000},
};

int vm_demo0_create_dev_dram(tvisor_vm_ctx_t *ctx,tvisor_dev_ctx_t *dev){
    if(dev->type != TVISOR_DEV_TYPE_DRAM){
        return TVISOR_STATUS_ERROR;
    }
}

void vm_demo_0_init(void){

}
