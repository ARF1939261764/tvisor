
#include "tvisor.h"
#include "tvisor_mmu.h"

int tvisor_mmu_init(tvisor_vm_ctx_ptr_t vm_ctx){
    if(vm_ctx == NULL){
        return TVISOR_STATUS_ERROR;
    }

}

int tvisor_mmu_region_register(tvisor_vm_ctx_ptr_t vm_ctx,tvisor_mem_region_ptr_t region){

}

int tvisor_mmu_map(tvisor_vm_ctx_ptr_t vm_ctx,void *start_addr,size_t size){

}
