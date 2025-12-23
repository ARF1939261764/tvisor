#ifndef PORTCONTEXT_H
#define PORTCONTEXT_H

#include "port.h"
.macro portcontext_SAVE_CONTEXT
    addi sp, sp, -portSTACK_FRAME_SIZE

    store_x  x1,  portSTACK_X1_IDX * portWORD_SIZE(sp)
    store_x  x5,  portSTACK_X5_IDX * portWORD_SIZE(sp)
    store_x  x6,  portSTACK_X6_IDX * portWORD_SIZE(sp)
    store_x  x7,  portSTACK_X7_IDX * portWORD_SIZE(sp)
    store_x  x8,  portSTACK_X8_IDX * portWORD_SIZE(sp)
    store_x  x9,  portSTACK_X9_IDX * portWORD_SIZE(sp)
    store_x x10, portSTACK_X10_IDX * portWORD_SIZE(sp)
    store_x x11, portSTACK_X11_IDX * portWORD_SIZE(sp)
    store_x x12, portSTACK_X12_IDX * portWORD_SIZE(sp)
    store_x x13, portSTACK_X13_IDX * portWORD_SIZE(sp)
    store_x x14, portSTACK_X14_IDX * portWORD_SIZE(sp)
    store_x x15, portSTACK_X15_IDX * portWORD_SIZE(sp)
    store_x x16, portSTACK_X16_IDX * portWORD_SIZE(sp)
    store_x x17, portSTACK_X17_IDX * portWORD_SIZE(sp)
    store_x x18, portSTACK_X18_IDX * portWORD_SIZE(sp)
    store_x x19, portSTACK_X19_IDX * portWORD_SIZE(sp)
    store_x x20, portSTACK_X20_IDX * portWORD_SIZE(sp)
    store_x x21, portSTACK_X21_IDX * portWORD_SIZE(sp)
    store_x x22, portSTACK_X22_IDX * portWORD_SIZE(sp)
    store_x x23, portSTACK_X23_IDX * portWORD_SIZE(sp)
    store_x x24, portSTACK_X24_IDX * portWORD_SIZE(sp)
    store_x x25, portSTACK_X25_IDX * portWORD_SIZE(sp)
    store_x x26, portSTACK_X26_IDX * portWORD_SIZE(sp)
    store_x x27, portSTACK_X27_IDX * portWORD_SIZE(sp)
    store_x x28, portSTACK_X28_IDX * portWORD_SIZE(sp)
    store_x x29, portSTACK_X29_IDX * portWORD_SIZE(sp)
    store_x x30, portSTACK_X30_IDX * portWORD_SIZE(sp)
    store_x x31, portSTACK_X31_IDX * portWORD_SIZE(sp)
    
    csrr t0, sstatus
    store_x t0, portSTACK_SSTATUS_IDX * portWORD_SIZE( sp )
    
    load_x t0, pxCurrentTCB          /* Load pxCurrentTCB. */
    store_x sp, 0 ( t0 )             /* Write sp to first TCB member. */
.endm

.macro portcontext_RESTORE_CONTEXT
    load_x t0, pxCurrentTCB /* Load pxCurrentTCB. */
    load_x sp, 0 ( t0 )     /* Read sp from first TCB member. */


    load_x  x1,  portSTACK_X1_IDX * portWORD_SIZE(sp)
    load_x  x5,  portSTACK_X5_IDX * portWORD_SIZE(sp)
    load_x  x6,  portSTACK_X6_IDX * portWORD_SIZE(sp)
    load_x  x7,  portSTACK_X7_IDX * portWORD_SIZE(sp)
    load_x  x8,  portSTACK_X8_IDX * portWORD_SIZE(sp)
    load_x  x9,  portSTACK_X9_IDX * portWORD_SIZE(sp)
    load_x x10, portSTACK_X10_IDX * portWORD_SIZE(sp)
    load_x x11, portSTACK_X11_IDX * portWORD_SIZE(sp)
    load_x x12, portSTACK_X12_IDX * portWORD_SIZE(sp)
    load_x x13, portSTACK_X13_IDX * portWORD_SIZE(sp)
    load_x x14, portSTACK_X14_IDX * portWORD_SIZE(sp)
    load_x x15, portSTACK_X15_IDX * portWORD_SIZE(sp)
    load_x x16, portSTACK_X16_IDX * portWORD_SIZE(sp)
    load_x x17, portSTACK_X17_IDX * portWORD_SIZE(sp)
    load_x x18, portSTACK_X18_IDX * portWORD_SIZE(sp)
    load_x x19, portSTACK_X19_IDX * portWORD_SIZE(sp)
    load_x x20, portSTACK_X20_IDX * portWORD_SIZE(sp)
    load_x x21, portSTACK_X21_IDX * portWORD_SIZE(sp)
    load_x x22, portSTACK_X22_IDX * portWORD_SIZE(sp)
    load_x x23, portSTACK_X23_IDX * portWORD_SIZE(sp)
    load_x x24, portSTACK_X24_IDX * portWORD_SIZE(sp)
    load_x x25, portSTACK_X25_IDX * portWORD_SIZE(sp)
    load_x x26, portSTACK_X26_IDX * portWORD_SIZE(sp)
    load_x x27, portSTACK_X27_IDX * portWORD_SIZE(sp)
    load_x x28, portSTACK_X28_IDX * portWORD_SIZE(sp)
    load_x x29, portSTACK_X29_IDX * portWORD_SIZE(sp)
    load_x x30, portSTACK_X30_IDX * portWORD_SIZE(sp)
    load_x x31, portSTACK_X31_IDX * portWORD_SIZE(sp)


    load_x t0,portSTACK_PXCODE_IDX * portWORD_SIZE(sp)
    csrw sepc,t0

    load_x t0, portSTACK_SSTATUS_IDX * portWORD_SIZE( sp )
    csrw sstatus, t0  

    addi sp, sp, portSTACK_FRAME_SIZE

.endm


.macro portcontext_SAVE_CONTEXT_EXCEPTION
    portcontext_SAVE_CONTEXT
    csrr a0, scause
    csrr a1, sepc
    addi a1, a1, 4          /* Synchronous so update exception return address to the instruction after the instruction that generated the exception. */
    store_x a1,  portSTACK_PXCODE_IDX * portWORD_SIZE(sp)   /* Save updated exception return address. */
    add t0,x0,sp
    la sp, __freertos_irq_stack_top /* Switch to ISR stack. */
.endm

.macro portcontext_SAVE_CONTEXT_INTR
    portcontext_SAVE_CONTEXT
    csrr a0, scause
    csrr a1, sepc
    store_x a1,  portSTACK_PXCODE_IDX * portWORD_SIZE(sp)   /* Save updated exception return address. */
    la sp, __freertos_irq_stack_top /* Switch to ISR stack. */
.endm

#endif
