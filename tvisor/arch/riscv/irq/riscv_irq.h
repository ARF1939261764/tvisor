#ifndef __RISCV_IRQ_H
#define __RISCV_IRQ_H
#include <stdlib.h>
#include <stdint.h>
#include "tvisor_io.h"

typedef void (* riscv_irq_handler_t)(void);

#define RISCV_PLIC_CTX_NUM                    (2)
#define RISCV_PLIC_IRQ_NUM                    (256)

#define RISCV_PLIC_BASE                       ((size_t)(0x0c000000))
#define RISCV_PLIC_REG_IRQ_PRIOR(irq)         ((size_t)(RISCV_PLIC_BASE + 0x4 * (irq)))
#define RISCV_PLIC_REG_IRQ_CTX_EN(ctx,irq)    ((size_t)(RISCV_PLIC_BASE + 0x002000 + (ctx) * 0x80 + (irq / 32) * 4))
#define RISCV_PLIC_REG_IRQ_CTX_PRIOR(ctx)     ((size_t)(RISCV_PLIC_BASE + 0x200000 + (ctx) * 0x01000 + 0x0))
#define RISCV_PLIC_REG_IRQ_CTX_COMPLETE(ctx)  ((size_t)(RISCV_PLIC_BASE + 0x200000 + (ctx) * 0x01000 + 0x4))

#define IRQ_CTX_HART0_S       (1)

#define IRQ_UART0             (10)

extern int  riscv_irq_init(void);
extern int  riscv_irq_enable(uint16_t irq);
extern int  riscv_irq_disable(uint16_t irq);
extern int  riscv_irq_register(uint16_t irq,riscv_irq_handler_t handler);
extern int  riscv_irq_set_prior(uint16_t irq,uint32_t prior);
extern int  riscv_irq_set_prior_th(uint32_t prior);

#endif
