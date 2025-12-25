#ifndef PORTCONTEXT_H
#define PORTCONTEXT_H

#define store_x          sd
#define load_x           ld

//portSSTATUS_DEFAULT_VALUE
//SPIE == 1
//SIE  == 0
//FS   == 1(initial)
//UXL  == 2(64bit)
#define portSSTATUS_DEFAULT_VALUE              (0x0000000200002020UL)

#define portHSTATUS_DEFAULT_VALUE              (0x0000000200000000UL)

#define portWORD_SIZE         (8)
#define portSTACK_FRAME_SIZE  (32*portWORD_SIZE)

 #define portSTACK_HSTATUS_IDX                 (31)
 #define portSTACK_SSTATUS_IDX                 (30)
 #define portSTACK_XCRITICALNESTING_IDX        (29)
 #define portSTACK_X31_IDX                     (28)
 #define portSTACK_X30_IDX                     (27)
 #define portSTACK_X29_IDX                     (26)
 #define portSTACK_X28_IDX                     (25)
 #define portSTACK_X27_IDX                     (24)
 #define portSTACK_X26_IDX                     (23)
 #define portSTACK_X25_IDX                     (22)
 #define portSTACK_X24_IDX                     (21)
 #define portSTACK_X23_IDX                     (20)
 #define portSTACK_X22_IDX                     (19)
 #define portSTACK_X21_IDX                     (18)
 #define portSTACK_X20_IDX                     (17)
 #define portSTACK_X19_IDX                     (16)
 #define portSTACK_X18_IDX                     (15)
 #define portSTACK_X17_IDX                     (14)
 #define portSTACK_X16_IDX                     (13)
 #define portSTACK_X15_IDX                     (12)
 #define portSTACK_X14_IDX                     (11)
 #define portSTACK_X13_IDX                     (10)
 #define portSTACK_X12_IDX                     ( 9)
 #define portSTACK_X11_IDX                     ( 8)
 #define portSTACK_X10_IDX                     ( 7)
 #define portSTACK_X9_IDX                      ( 6)
 #define portSTACK_X8_IDX                      ( 5)
 #define portSTACK_X7_IDX                      ( 4)
 #define portSTACK_X6_IDX                      ( 3)
 #define portSTACK_X5_IDX                      ( 2)
 #define portSTACK_X1_IDX                      ( 1)
 #define portSTACK_PXCODE_IDX                  ( 0)

 #define portSTACK_PVPARAMETERS_IDX            (portSTACK_X10_IDX)
 #define portSTACK_PORTTASK_RETURN_ADDRESS_IDX (portSTACK_X1_IDX)

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
    csrr t0, hstatus
    store_x t0, portSTACK_HSTATUS_IDX * portWORD_SIZE( sp )
    
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


    load_x t0, portSTACK_HSTATUS_IDX * portWORD_SIZE( sp )
    csrw hstatus, t0  

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

.macro portcontext_SAVE_CONTEXT_FUNC
    portcontext_SAVE_CONTEXT
    //sstatus
    csrr t0, sstatus
    ori t0,t0,1 << 8
    store_x t0, portSTACK_SSTATUS_IDX * portWORD_SIZE( sp )
    //return address
    store_x ra,  portSTACK_PXCODE_IDX * portWORD_SIZE(sp)   /* Save updated exception return address. */
.endm

#endif
