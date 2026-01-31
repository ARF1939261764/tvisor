#ifndef __IO_H
#define __IO_H

#include "stdint.h"

#define read8(addr)   (*(volatile uint8_t *)(addr))
#define write8(addr,value)   (*(volatile uint8_t *)(addr) = (value))
#define write32(addr,value)  (*(volatile uint32_t *)(addr) = (value))
#define read32(addr)         (*(volatile uint32_t *)(addr))

#endif
