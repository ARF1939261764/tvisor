/*
* Copyright by Syntacore LLC © 2016, 2017. See LICENSE for details
* @file       <link.ld>
* @brief      bare metal tests' linker script
*/

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY {
  RAM (rwx) : ORIGIN = 0x0000000080200000, LENGTH = 256*1024K
}

STACK_SIZE = 1024;
IRQ_STACK_SIZE = 1024;

CL_SIZE = 32;

SECTIONS {
  /* text segment */
  .text ORIGIN(RAM) : { 
    *crt.S.o(.text .text.*)
    *(.text .text.*)
  } >RAM

  /* rodata segment */
  .rodata ALIGN(CL_SIZE) : {
    *(.rodata) *(.rodata.*) *(.gnu.linkonce.r.*)
  } >RAM

  /* data segment */
  .data ALIGN(CL_SIZE) : {
    __global_pointer$ = .;
    PROVIDE(__DATA_START__ = .);
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    *(.tdata .tdata.*)
    *(.data .data.*)
  } >RAM

  /* bss segment */
  .bss ALIGN(CL_SIZE) : {
    PROVIDE(__BSS_START__ = .);
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.tbss .tbss.*)
    *(.dynbss) *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
    PROVIDE(__BSS_END__ = .);
  } >RAM
  
  .irq_stack ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE - IRQ_STACK_SIZE : {
    PROVIDE(__IRQ_STACK_START__ = .);
    . += IRQ_STACK_SIZE;
    PROVIDE(__freertos_irq_stack_top = .);
    PROVIDE(__IRQ_STACK_TOP__ = .);
  } >RAM

  .stack ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE : {
    PROVIDE(__STACK_START__ = .);
    . += STACK_SIZE;
    PROVIDE(__C_STACK_TOP__ = .);
  } >RAM
}
