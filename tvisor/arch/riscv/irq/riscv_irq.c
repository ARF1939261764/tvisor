#include <stdlib.h>
#include <stdint.h>
#include "tvisor.h"
#include "tvisor_io.h"
#include "riscv_irq.h"

static riscv_irq_handler_t irq_handler[RISCV_PLIC_CTX_NUM][RISCV_PLIC_IRQ_NUM];

void irq_sext_handler(void){
  uint32_t irq_num;
  irq_num = read32(RISCV_PLIC_REG_IRQ_CTX_COMPLETE(IRQ_CTX_HART0_S));
  if(irq_handler[IRQ_CTX_HART0_S][irq_num] != NULL){
    irq_handler[IRQ_CTX_HART0_S][irq_num]();
  }
  write32(RISCV_PLIC_REG_IRQ_CTX_COMPLETE(IRQ_CTX_HART0_S),irq_num);
}

int riscv_irq_init(void){
  riscv_irq_set_prior_th(0);
  for(int i=0;i<RISCV_PLIC_CTX_NUM;i++){
    for(int j=0;j<RISCV_PLIC_IRQ_NUM;j++){
      irq_handler[i][j] = NULL;
    }
  }
  return TVISOR_STATUS_OK;
}

int  riscv_irq_enable(uint16_t irq){
  if((irq >= RISCV_PLIC_IRQ_NUM)){
    return TVISOR_STATUS_ERROR;
  }
  write32(RISCV_PLIC_REG_IRQ_CTX_EN(IRQ_CTX_HART0_S,irq), read32(RISCV_PLIC_REG_IRQ_CTX_EN(IRQ_CTX_HART0_S,irq)) | (1 << (irq & 0x1F)));
  return TVISOR_STATUS_OK;
}

int  riscv_irq_disable(uint16_t irq){
  if((irq >= RISCV_PLIC_IRQ_NUM)){
    return TVISOR_STATUS_ERROR;
  }
  write32(RISCV_PLIC_REG_IRQ_CTX_EN(IRQ_CTX_HART0_S,irq), read32(RISCV_PLIC_REG_IRQ_CTX_EN(IRQ_CTX_HART0_S,irq)) & ~(1 << (irq & 0x1F)));
  return TVISOR_STATUS_OK;
}

int riscv_irq_register(uint16_t irq,riscv_irq_handler_t handler){
  if((irq >= RISCV_PLIC_IRQ_NUM)){
    return TVISOR_STATUS_ERROR;
  }
  irq_handler[IRQ_CTX_HART0_S][irq] = handler;
  return TVISOR_STATUS_OK;
}

int riscv_irq_set_prior(uint16_t irq,uint32_t prior){
  write32(RISCV_PLIC_REG_IRQ_PRIOR(irq),prior);
  return TVISOR_STATUS_OK;
}

int riscv_irq_set_prior_th(uint32_t prior){
  write32(RISCV_PLIC_REG_IRQ_CTX_PRIOR(IRQ_CTX_HART0_S),prior);
  return TVISOR_STATUS_OK;
}
