#ifndef __TVISOR_H
#define __TVISOR_H

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#define TVISOR_STATUS_ERROR (-1)
#define TVISOR_STATUS_OK    ( 0)

#define TVISOR_TASK_STACK_DEPTH   (2048)

typedef enum {
    TVISOR_DEV_TYPE_PLIC,
    TVISOR_DEV_TYPE_CLINT,
    TVISOR_DEV_TYPE_UART_8250,
    TVISOR_DEV_TYPE_DRAM
}tvisor_dev_type_t;

typedef void* tvisor_dev_handler_t;
typedef void* tvisor_dev_args_t;

typedef struct{
    char                 *name;
    tvisor_dev_type_t     type;
    tvisor_dev_args_t     args;
    size_t                start_addr;
    size_t                size;
    tvisor_dev_handler_t  handler;
}tvisor_dev_ctx_t;

typedef tvisor_dev_ctx_t *tvisor_dev_ctx_ptr_t;

typedef struct{
    size_t entry_point_addr;
    char *name;
    tvisor_dev_ctx_t *dev_list;
    TaskHandle_t tcb;
    uint8_t vmid;
    UBaseType_t uxPriority;
}tvisor_vm_ctx_t;

typedef tvisor_vm_ctx_t *tvisor_vm_ctx_ptr_t;

extern int tvisor_init(void);
extern int tvisor_vm_create(tvisor_vm_ctx_ptr_t vm_ctx);
extern int tvisor_vm_run(tvisor_vm_ctx_ptr_t vm_ctx);
extern int tvisor_dev_create(tvisor_vm_ctx_ptr_t vm_ctx,tvisor_dev_ctx_ptr_t dev_ctx);


#endif
