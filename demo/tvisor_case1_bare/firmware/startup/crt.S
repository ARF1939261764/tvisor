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
#   csrr a0,CSR_MHARTID
#   beq a0,x0,_continue
#   bne a0,x0,_standby

# _standby:
#   j .

# _continue:
#   j .
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
//go to system_initial
//*****************************
  li    a0, 0
  li    a1, 0
  call system_initial
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
