#include "port_asm.h"

.global pxPortInitialiseStack_asm
.global xPortStartFirstTask_asm
.global freertos_risc_v_mtimer_interrupt_handler
.global freertos_risc_v_stimer_interrupt_handler
.global freertos_risc_v_exception_handler
.global vPortSwitchCtx

/*
 * Unlike other ports pxPortInitialiseStack() is written in assembly code as it
 * needs access to the portasmADDITIONAL_CONTEXT_SIZE constant.  The prototype
 * for the function is as per the other ports:
 * StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );
 *
 * As per the standard RISC-V ABI pxTopOfStack is passed in in a0, pxCode in
 * a1, and pvParameters in a2.  The new top of stack is passed out in a0.
 *
 * RISC-V maps registers to ABI names as follows (X1 to X31 integer registers
 * for the 'I' profile, X1 to X15 for the 'E' profile, currently I assumed).
 *
 * Register      ABI Name    Description                       Saver
 * x0            zero        Hard-wired zero                   -
 * x1            ra          Return address                    Caller
 * x2            sp          Stack pointer                     Callee
 * x3            gp          Global pointer                    -
 * x4            tp          Thread pointer                    -
 * x5-7          t0-2        Temporaries                       Caller
 * x8            s0/fp       Saved register/Frame pointer      Callee
 * x9            s1          Saved register                    Callee
 * x10-11        a0-1        Function Arguments/return values  Caller
 * x12-17        a2-7        Function arguments                Caller
 * x18-27        s2-11       Saved registers                   Callee
 * x28-31        t3-6        Temporaries                       Caller
 *
 * The RISC-V context is saved to FreeRTOS tasks in the following stack frame,
 * where the global and thread pointers are currently assumed to be constant so
 * are not saved:
 *
 */
pxPortInitialiseStack_asm:
    //stack
    addi a0,a0,-portSTACK_FRAME_SIZE
    //sstatus
    li t0,portSSTATUS_DEFAULT_VALUE
    or t0, t0, a3
    //hstatus
    li t1,portHSTATUS_DEFAULT_VALUE
    or t1, t1, a4
    //save context
    store_x t0,portSTACK_SSTATUS_IDX                 * portWORD_SIZE(a0)//sstatus
    store_x t1,portSTACK_HSTATUS_IDX                 * portWORD_SIZE(a0)//hstatus
    store_x x0,portSTACK_XCRITICALNESTING_IDX        * portWORD_SIZE(a0)//xCriticalNesting
    store_x a2,portSTACK_PVPARAMETERS_IDX            * portWORD_SIZE(a0)//pvParameters
    store_x a1,portSTACK_PXCODE_IDX                  * portWORD_SIZE(a0)//task start address
    //ret
    ret

xPortStartFirstTask_asm:
    portcontext_RESTORE_CONTEXT
    sret//ret to U mode
    
freertos_risc_v_mtimer_interrupt_handler:
    j .

freertos_risc_v_exception_handler:
    portcontext_SAVE_CONTEXT_EXCEPTION
    li   t0,0x8
    beq a0,t0,freertos_risc_v_ecall_exception_handler
    li   t0,0xa
    beq a0,t0,freertos_risc_v_ecall_exception_handler
    j freertos_risc_v_other_exception_handler

freertos_risc_v_stimer_interrupt_handler:
    portcontext_SAVE_CONTEXT_INTR
    call irq_stimer_handler
    call prvTickISR
    portcontext_RESTORE_CONTEXT
    sret

vPortSwitchCtx:
    csrr a0,sstatus
    csrr a1,sstatus
    sll a0,a0,(5-1)
    andi a0,a0,1 << 5
    andi a1,a1,~(1 << 1)
    or a0,a0,a1
    csrw sstatus,a0
    portcontext_SAVE_CONTEXT_FUNC
    call vTaskSwitchContext
    portcontext_RESTORE_CONTEXT
    sret

freertos_risc_v_ecall_exception_handler:
    li t0,1
    beq a2,t0,freertos_risc_v_ecall_exception_switch_ctx_handler
    j .

freertos_risc_v_ecall_exception_switch_ctx_handler:
    call vTaskSwitchContext
    portcontext_RESTORE_CONTEXT
    sret

freertos_risc_v_other_exception_handler:
    j .
