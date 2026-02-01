#ifndef __TVISOR_DEV_H
#define __TVISOR_DEV_H

#include "stddef.h"
#include "stdint.h"

#include "tvisor_dev_plic.h"
#include "tvisor_dev_uart16550.h"

typedef enum {
    TVISOR_DEV_TYPE_NONE,
    TVISOR_DEV_TYPE_PLIC,
    TVISOR_DEV_TYPE_CLINT,
    TVISOR_DEV_TYPE_UART_8250,
    TVISOR_DEV_TYPE_UART_16550,
    TVISOR_DEV_TYPE_MEM
}tvisor_dev_type_t;

typedef void* tvisor_dev_handler_t;
typedef void* tvisor_dev_args_t;

typedef struct tvisor_mem_region_t_struct{
    size_t start_addr;
    size_t size;
    uint8_t attr;//mmu leaf pte attribute
    uint8_t pbmt;
}tvisor_mem_region_t;

struct tvisor_dev_ctx_struct_t;
typedef struct tvisor_dev_ctx_struct_t tvisor_dev_ctx_t;
typedef tvisor_dev_ctx_t *tvisor_dev_ctx_ptr_t;
typedef int (* tvisor_dev_create_func_t)(tvisor_dev_ctx_ptr_t dev);

struct tvisor_dev_ctx_struct_t{
    uint8_t              valid;
    tvisor_mem_region_t  region;
    char                 *name;
    tvisor_dev_type_t     type;
    union{
        uint32_t                   base;
        tvisor_dev_uart16550_ctx_t uart16550_ctx;
        tvisor_dev_plic_ctx_t      plic_ctx;
    }ctx;
};

typedef int (*tvisor_dev_init_t)(void *ctx);

typedef int (*tvisor_dev_write_register_t)(void *ctx, size_t addr_offset, uint64_t value, uint8_t size);
typedef int (*tvisor_dev_read_register_t)(void *ctx, size_t addr_offset,uint64_t *value,uint8_t size);


typedef struct tvisor_dev_struct_ops_t{
    tvisor_dev_init_t           init;
    tvisor_dev_write_register_t write_register;
    tvisor_dev_read_register_t  read_register;
}tvisor_dev_ops_t;

extern const tvisor_dev_ops_t *tvisor_dev_ops_list[];

#endif
