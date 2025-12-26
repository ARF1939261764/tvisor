
#include "tvisor.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "tvisor_mmu.h"
#include "tvisor_printf.h"
#include "FreeRTOS.h"

void *tvisor_mmu_align_malloc(size_t size,size_t align_size){
    size_t* addr = (size_t *)pvPortMalloc(size + align_size);
    size_t* aligned_addr =  (size_t *)(((size_t)addr + size) & ~(align_size - 1));
    aligned_addr[-1] = (size_t)addr;
    return aligned_addr;
}

void tvisor_mmu_align_free(void* addr){
    vPortFree((void *)((size_t *)addr)[-1]);
}

int tvisor_mmu_init(tvisor_vm_ctx_ptr_t vm_ctx){
    uint64_t* hgatp;
    if(vm_ctx == NULL){
        return TVISOR_STATUS_ERROR;
    }
    hgatp = tvisor_mmu_align_malloc(TVISOR_MMU_L0_PTE_SIZE,TVISOR_MMU_L0_PTE_SIZE);;
    if(hgatp == NULL){
        return TVISOR_STATUS_ERROR;
    }
    vm_ctx->hgatp = ((size_t)hgatp | (TVISOR_MMU_MODE_SV39 << 60) | ((size_t)vm_ctx->vmid << 44));
    for(int i=0;i<TVISOR_MMU_L0_PTE_NUM;i++){
        hgatp[i] = 0;
    }
    return TVISOR_STATUS_OK;
}

static int tvisor_mmu_fill_pte(tvisor_vm_ctx_ptr_t vm_ctx,tvisor_mem_region_t *region,uint64_t *pte_base,uint16_t vpn,uint8_t pte_level,uint8_t ptw_level,size_t page_size,size_t pte_size){
    size_t malloc_size;
    if(!(pte_base[vpn] & TVISOR_MMU_PAGE_ATTR_V)){
        malloc_size = ptw_level == (pte_level + 1) ? page_size : pte_size;
        pte_base[vpn] = ((size_t)tvisor_mmu_align_malloc(malloc_size,malloc_size)) << 10;
        if(pte_base[vpn] == (size_t)NULL){
            return TVISOR_STATUS_ERROR;
        }
        pte_base[vpn] |= TVISOR_MMU_PAGE_ATTR_V;
    }
    if(ptw_level == (pte_level + 1)){
        pte_base[vpn] |= region->attr;
        pte_base[vpn] |= (size_t)(region->pbmt) << 61;
    }
    return TVISOR_STATUS_OK;
}

int tvisor_mmu_dump_map(tvisor_vm_ctx_ptr_t vm_ctx,size_t start_addr){
    uint16_t vpn[3];
    uint64_t *pte_base;
    uint64_t pte;
    vpn[2] = (start_addr >> 30) & 0x7FF;
    vpn[1] = (start_addr >> 21) & 0x1FF;
    vpn[0] = (start_addr >> 12) & 0x1FF;
    pte_base = (uint64_t *)(vm_ctx->hgatp & TVISOR_MMU_MODE_SV39_ADDR_MASK);
    for(int i=0;i<3;i++){
        pte = pte_base[vpn[2-i]];
        tvisor_printf("tvisor_mmu_dump_map,level %d,pte = %016lx\r\n",i,pte);
        if((pte & 0xE) != 0x00){
            break;
        }
        pte_base = (uint64_t *)((pte >> 10) & TVISOR_MMU_MODE_SV39_ADDR_MASK);
    }
    return TVISOR_STATUS_OK;
}

int tvisor_mmu_map(tvisor_vm_ctx_ptr_t vm_ctx,size_t start_addr,size_t size){
    uint32_t dev_idx;
    uint64_t *hgatp_addr;
    uint64_t *pte_base;
    uint32_t ptw_level;
    uint16_t vpn2,vpn1,vpn0;
    uint16_t vpn[3];
    size_t malloc_size;
    size_t page_size[3];
    size_t pte_size[3];
    size_t leaf_page_size;
    for(dev_idx=0;dev_idx<vm_ctx->dev_num;dev_idx++){
        if(
            (((size_t)start_addr       ) >= ((size_t)(vm_ctx->dev_list[dev_idx].region.start_addr)                                        )) && 
            (((size_t)start_addr + size) <= ((size_t)(vm_ctx->dev_list[dev_idx].region.start_addr) + vm_ctx->dev_list[dev_idx].region.size))
        ){
            break;
        }
    }
    if(dev_idx == vm_ctx->dev_num){
        return TVISOR_STATUS_ERROR;
    }
    hgatp_addr = (uint64_t *)(vm_ctx->hgatp & TVISOR_MMU_MODE_SV39_ADDR_MASK);
    if(vm_ctx->dev_list[dev_idx].region.size <= TVISOR_MMU_L0_LEAF_PAGE_SIZE){
        ptw_level = 3;
    }
    else if(vm_ctx->dev_list[dev_idx].region.size <= TVISOR_MMU_L1_LEAF_PAGE_SIZE){
        ptw_level = 3;
    }
    else if(vm_ctx->dev_list[dev_idx].region.size <= TVISOR_MMU_L2_LEAF_PAGE_SIZE){
        ptw_level = 2;
    }
    else{
        ptw_level = 1;
    }

    page_size[0] = TVISOR_MMU_L0_LEAF_PAGE_SIZE;
    page_size[1] = TVISOR_MMU_L1_LEAF_PAGE_SIZE;
    page_size[2] = TVISOR_MMU_L2_LEAF_PAGE_SIZE;

    pte_size[0] = TVISOR_MMU_L0_PTE_SIZE;
    pte_size[1] = TVISOR_MMU_L1_PTE_SIZE;
    pte_size[2] = TVISOR_MMU_L2_PTE_SIZE;

    leaf_page_size = page_size[ptw_level - 1];
    do{
        vpn[2] = (start_addr >> 30) & 0x7FF;
        vpn[1] = (start_addr >> 21) & 0x1FF;
        vpn[0] = (start_addr >> 12) & 0x1FF;
        pte_base = hgatp_addr;
        for(int i=0;i<ptw_level;i++){
            if(tvisor_mmu_fill_pte(vm_ctx,&(vm_ctx->dev_list[dev_idx].region),pte_base,vpn[2-i],
                i,ptw_level,
                page_size[i],pte_size[i]) != TVISOR_STATUS_OK
            ){
                return TVISOR_STATUS_ERROR;
            }
            pte_base = (uint64_t *)((pte_base[vpn[2-i]] >> 10) & TVISOR_MMU_MODE_SV39_ADDR_MASK);
        }
        start_addr += leaf_page_size;
        if(size < leaf_page_size){
            size = 0;
        }
        else{
            size -= leaf_page_size;
        }
    }while(size != 0);

    return TVISOR_STATUS_OK;
}
