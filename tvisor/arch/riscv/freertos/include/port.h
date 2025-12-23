#ifndef __PORT_H
#define __PORT_H
#include "stdint.h"
//                             V       MODE[1]    MODE[0]
#define RISCV_PRV_U_MODE   ((0 << 2) | (0 << 1) | (0 << 0))
#define RISCV_PRV_S_MODE   ((0 << 2) | (1 << 1) | (0 << 0))
#define RISCV_PRV_M_MODE   ((0 << 2) | (1 << 1) | (1 << 0))
#define RISCV_PRV_HU_MODE  ((0 << 2) | (0 << 1) | (0 << 0))
#define RISCV_PRV_HS_MODE  ((0 << 2) | (1 << 1) | (0 << 0))
#define RISCV_PRV_VU_MODE  ((1 << 2) | (0 << 1) | (0 << 0))
#define RISCV_PRV_VS_MODE  ((1 << 2) | (1 << 1) | (0 << 0))

typedef uint8_t riscv_prv_mode_t;

#endif
