#ifndef __TVISOR_MMU_H
#define __TVISOR_MMU_H

#include "tvisor.h"
#include <stddef.h>



typedef tvisor_mem_region_t *tvisor_mem_region_ptr_t;

int tvisor_mmu_init(tvisor_vm_ctx_ptr_t vm_ctx);
int tvisor_mmu_uninit(tvisor_vm_ctx_ptr_t vm_ctx);
int tvisor_mmu_map(tvisor_vm_ctx_ptr_t vm_ctx,size_t start_addr,size_t size);
int tvisor_mmu_unmap(tvisor_vm_ctx_ptr_t vm_ctx,size_t start_addr,size_t size);
int tvisor_mmu_dump_map(tvisor_vm_ctx_ptr_t vm_ctx,size_t start_addr);
void *tvisor_mmu_align_malloc(size_t size,size_t align_size);
void tvisor_mmu_align_free(void* addr);
#endif
