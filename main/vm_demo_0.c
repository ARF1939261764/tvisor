#include "tvisor.h"

tvisor_dev_ctx_t vm_demo_0_dev_list[] = {
    {.name = "clint",.type = TVISOR_DEV_TYPE_CLINT    ,.start_addr = 0x02000000,.size = 0x00010000},
    {.name = "plic" ,.type = TVISOR_DEV_TYPE_PLIC     ,.start_addr = 0x02000000,.size = 0x00010000},
    {.name = "uart0",.type = TVISOR_DEV_TYPE_UART_8250,.start_addr = 0x10000000,.size = 0x00000100},
    {.name = "dram" ,.type = TVISOR_DEV_TYPE_DRAM     ,.start_addr = 0x80000000,.size = 0x00000000},
};



void vm_demo_0_init(void){

}
