#include "port.h"
#include "projdefs.h"
#include "stdint.h"
#include "tvisor.h"
#include "tvisor_config.h"
#include <stdint.h>
#include "tvisor_printf.h"


static tvisor_vm_ctx_ptr_t tvisor_vm_list[TVISOR_MAX_VM_NUM];

static void tvisor_vm_host_task(void *args){
    void sbi_print(char *str);
    while(1){
        vTaskDelay(100);
        sbi_print("hello world!vm.....\r\n");
    }
}

int tvisor_init(void){
    for(int i=0;i<TVISOR_MAX_VM_NUM;i++){
        tvisor_vm_list[i] = NULL;
    }
    return TVISOR_STATUS_OK;
}

int tvisor_vm_malloc_vmid(uint8_t *id){
    for(int i=0;i<TVISOR_MAX_VM_NUM;i++){
        if(tvisor_vm_list[i] == NULL){
            *id = i;
        }
        return TVISOR_STATUS_OK;
    }
    return TVISOR_STATUS_ERROR;
}

int tvisor_vm_free_vmid(uint8_t id){
    if(id >= TVISOR_MAX_VM_NUM){
        return TVISOR_STATUS_ERROR;
    }
    tvisor_vm_list[id] = NULL;
    return TVISOR_STATUS_OK;
}

int tvisor_vm_create(tvisor_vm_ctx_ptr_t vm_ctx){
    uint8_t vmid = TVISOR_MAX_VM_NUM;
    BaseType_t xreturn;
    tvisor_vm_ctx_ptr_t  new_vm_ctx;
    task_defualt_args_t vm_task_args = {
        .prv_mode = RISCV_PRV_S_MODE,
        .args     = NULL
    };
    //malloc vmid
    if(tvisor_vm_malloc_vmid(&vmid) != TVISOR_STATUS_OK){
        return TVISOR_STATUS_ERROR;
    }
    vm_ctx->vmid = vmid;
    tvisor_vm_list[vmid] = vm_ctx;
    //tcb
    vTaskEnterCritical();
    xreturn = xTaskCreate((TaskFunction_t)vm_ctx->entry_point_addr,vm_ctx->name,TVISOR_TASK_STACK_DEPTH,&vm_task_args,vm_ctx->uxPriority,&(vm_ctx->tcb));
    if(xreturn == pdPASS){
        vTaskSuspend(vm_ctx->tcb);
    }
    vTaskExitCritical();
    if(xreturn != pdPASS){
        tvisor_vm_free_vmid(vmid);
        vm_ctx->vmid = TVISOR_MAX_VM_NUM;
    }
    return TVISOR_STATUS_OK;
}

int tvisor_vm_run(tvisor_vm_ctx_ptr_t vm_ctx){
    vTaskResume(vm_ctx->tcb);
    return TVISOR_STATUS_OK;
}
