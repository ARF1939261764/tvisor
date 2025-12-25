#ifndef __TVISOR_MMU_H
#define __TVISOR_MMU_H

#include "tvisor.h"
#include <stddef.h>



typedef tvisor_mem_region_t *tvisor_mem_region_ptr_t;

int tvisor_mmu_init(tvisor_vm_ctx_ptr_t vm_ctx);
int tvisor_mmu_uninit(tvisor_vm_ctx_ptr_t vm_ctx);
int tvisor_mmu_region_register(tvisor_vm_ctx_ptr_t vm_ctx,tvisor_mem_region_ptr_t region);
int tvisor_mmu_region_unregister(tvisor_vm_ctx_ptr_t vm_ctx,tvisor_mem_region_ptr_t region);
int tvisor_mmu_map(tvisor_vm_ctx_ptr_t vm_ctx,void *start_addr,size_t size);
int tvisor_mmu_unmap(tvisor_vm_ctx_ptr_t vm_ctx,void *start_addr,size_t size);

#endif
