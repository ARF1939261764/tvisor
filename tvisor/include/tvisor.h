#ifndef __TVISOR_H
#define __TVISOR_H

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "tvisor_dev.h"


#define TVISOR_STATUS_ERROR (-1)
#define TVISOR_STATUS_OK    ( 0)

#define TVISOR_TASK_STACK_DEPTH   (2048)

#define TVISOR_MMU_PAGE_ATTR_V       (1 << 0)
#define TVISOR_MMU_PAGE_ATTR_R       (1 << 1)
#define TVISOR_MMU_PAGE_ATTR_W       (1 << 2)
#define TVISOR_MMU_PAGE_ATTR_X       (1 << 3)
#define TVISOR_MMU_PAGE_ATTR_U       (1 << 4)
#define TVISOR_MMU_PAGE_ATTR_G       (1 << 5)

#define TVISOR_MMU_PAGE_ATTR_MEM     (TVISOR_MMU_PAGE_ATTR_U | TVISOR_MMU_PAGE_ATTR_V | TVISOR_MMU_PAGE_ATTR_R | TVISOR_MMU_PAGE_ATTR_W | TVISOR_MMU_PAGE_ATTR_X)
#define TVISOR_MMU_MMIO_PTE_ATTR     (TVISOR_MMU_PAGE_ATTR_U | TVISOR_MMU_PAGE_ATTR_V | TVISOR_MMU_PAGE_ATTR_X)

#define TVISOR_MMU_PAGE_PBMT_NONE    (0UL)
#define TVISOR_MMU_PAGE_PBMT_NC_MEM  (1UL)
#define TVISOR_MMU_PAGE_PBMT_NC_MMIO (2UL)

#define TVISOR_MMU_MODE_SV39         ( 8UL)
#define TVISOR_MMU_MODE_SV48         ( 9UL)
#define TVISOR_MMU_MODE_SV57         (10UL)
#define TVISOR_MMU_MODE_SV64         (11UL)

#define TVISOR_MMU_PTE_PPN_MASK      (((1UL << 44) - 1) << 10)
#define TVISOR_MMU_MODE_SV39_ADDR_MASK  ((1UL << (39 + 2)) - 1)

#define TVISOR_MMU_L0_PTE_NUM        (2048)
#define TVISOR_MMU_L1_PTE_NUM        ( 512)
#define TVISOR_MMU_L2_PTE_NUM        ( 512)

#define TVISOR_MMU_L0_LEAF_PAGE_SIZE  (512 * TVISOR_MMU_L1_LEAF_PAGE_SIZE)
#define TVISOR_MMU_L1_LEAF_PAGE_SIZE  (512 * TVISOR_MMU_L2_LEAF_PAGE_SIZE)
#define TVISOR_MMU_L2_LEAF_PAGE_SIZE  (4096)

#define TVISOR_MMU_L0_PTE_SIZE      (TVISOR_MMU_L0_PTE_NUM * 8)
#define TVISOR_MMU_L1_PTE_SIZE      (TVISOR_MMU_L1_PTE_NUM * 8)
#define TVISOR_MMU_L2_PTE_SIZE      (TVISOR_MMU_L2_PTE_NUM * 8)

typedef uint8_t tvisor_vmid_t;




typedef struct{
    TaskHandle_t tcb;
    TaskFunction_t entry_point_addr;
    char *name;
    size_t hgatp;
    tvisor_dev_ctx_t *dev_list;
    uint32_t dev_num;
    tvisor_vmid_t vmid;
    UBaseType_t uxPriority;
}tvisor_vm_ctx_t;

typedef tvisor_vm_ctx_t *tvisor_vm_ctx_ptr_t;

extern int tvisor_init(void);
extern int tvisor_vm_create(tvisor_vm_ctx_ptr_t vm_ctx);
extern int tvisor_vm_run(tvisor_vm_ctx_ptr_t vm_ctx);
extern int tvisor_dev_create(tvisor_vm_ctx_ptr_t vm_ctx,tvisor_dev_ctx_ptr_t dev_ctx);
extern uint32_t tvisor_vm_read32(size_t addr);
extern void tvisor_vm_write32(size_t addr,uint32_t data);

#endif
