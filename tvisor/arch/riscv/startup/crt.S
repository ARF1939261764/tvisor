/// Copyright by Syntacore LLC © 2016, 2017. See LICENSE for details
/// @file       <crt_tcm.S>
///

#include "riscv_csr_encoding.h"
#include "reloc.h"

  .globl main
  .globl _start
  .org (0)

_start:
//*****************************
//zero_int_regs
//*****************************
#ifndef __RVE_EXT
    zero_int_regs 1, 31
#else
    zero_int_regs 1, 15
#endif

//*****************************
//multi core
//*****************************
//  csrr a0,CSR_MHARTID
//  beq a0,x0,_continue
//  bne a0,x0,_standby
//
//_standby:
//  j .

_continue:
//*****************************
//__global_pointer
//*****************************
  .option push
  .option norelax
  la    gp, __global_pointer$
  .option pop

//*****************************
//clean bss
//*****************************
  la a0,__BSS_START__
  la a1,__BSS_END__
_clean_bss_start:
  beq a0,a1,_clean_bss_end
  add a0,a0,1
  j _clean_bss_start
_clean_bss_end:


//*****************************
//stack
//*****************************
  la sp,__C_STACK_TOP__

//*****************************
//pmp
//*****************************
  # li a0, 0x1FFFFFFF
  # csrw pmpaddr0, a0
  # li a0, 0x1f
  # csrw pmpcfg0, a0
//*****************************
//expection_handler
//*****************************

  la t0,expection_handler
  or t0,t0,0x01
  csrw stvec, t0

  csrr t0, sstatus                    /* Obtain current mstatus value. */
  andi t0, t0, ~0x2                   /* Ensure interrupts are disabled when the stack is restored within an ISR.  Required when a task is created after the schedulre has been started, otherwise interrupts would be disabled anyway. */
  csrw sstatus,t0

  //sie
  csrr t0, sie
  ori t0, t0, 1 << 5
  csrw sie,t0


//*****************************
//go to main
//*****************************
  li    a0, 0
  li    a1, 0
  call main

//*****************************
//exit
//*****************************
  j .

  .align(2)
expection_handler:
  j     freertos_risc_v_exception_handler        //Exception
  .word 0xdeaddead                               //1
  .word 0xdeaddead                               //2
  .word 0xdeaddead                               //3
  .word 0xdeaddead                               //4
  j     freertos_risc_v_stimer_interrupt_handler //5
  .word 0xdeaddead                               //6
  j     freertos_risc_v_mtimer_interrupt_handler //7
  .word 0xdeaddead                               //8
  .word 0xdeaddead                               //9

.globl cpu_switch_to_s_mode
.globl cpu_switch_to_hs_mode
.globl cpu_switch_to_vs_mode

cpu_switch_to_s_mode:
cpu_switch_to_hs_mode:
  // 设置mpp为0x2(s mode)
  csrr    t0, mstatus
  li      t1, ~(0x3 << 11)
  and     t0, t0, t1
  li      t1, (0x1 << 11)
  or      t0, t0, t1
  csrw    mstatus, t0

  // // 禁用 MMU（裸机场景）：satp=0（S-mode 无分页）
  li      t0, 0
  csrw    satp, t0

  // 将函数的返回地址设置为mret地址
  csrw    mepc, ra
  mret

cpu_switch_to_vs_mode:
  // 设置spp为0x1(s mode)
  csrr    t0, sstatus
  li      t1, (0x1 << 8)
  or      t0, t0, t1
  csrw    sstatus, t0
  // 设置spv为0x1(vs mode)
  csrr    t0, hstatus
  li      t1, (0x1 << 7)
  or      t0, t0, t1
  csrw    hstatus, t0

  // 将函数的返回地址设置为sret地址
  csrw    sepc, ra
  sret


