#include "tvisor.h"
#include "tvisor_dev.h"

tvisor_dev_ctx_t dev_list[] = {
    [0] = {
        .region = {
            .start_addr = 0x80000000,
            .size = 256*1024*1024,
            .attr = TVISOR_MMU_PAGE_ATTR_MEM | TVISOR_MMU_PAGE_ATTR_U,
            .pbmt = TVISOR_MMU_PAGE_PBMT_NONE
        },
    },
    [1] = {
        .uart16550_ctx = {
            
        }
    }
};

int vm_demo0_create_dev_dram(tvisor_vm_ctx_t *ctx,tvisor_dev_ctx_t *dev){
    if(dev->type != TVISOR_DEV_TYPE_DRAM){
        return TVISOR_STATUS_ERROR;
    }
}

void vm_demo_0_init(void){

}
