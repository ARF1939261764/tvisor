#include "port.h"
#include "stdint.h"
#include "tvisor.h"
#include "riscv_csr_encoding.h"
#include "port_asm.h"
#include "tvisor_dev.h"
#include "tvisor_dev_uart16550.h"
#include "tvisor_mmu.h"
#include <stdint.h>
#include "task.h"
#include "tvisor_printf.h"

const uint8_t regs_stack_idx_map[] = {
    [ 0] = 0,
    [ 1] = portSTACK_X1_IDX,
    [ 2] = 0,
    [ 3] = 0,
    [ 4] = 0,
    [ 5] = portSTACK_X5_IDX,
    [ 6] = portSTACK_X6_IDX,
    [ 7] = portSTACK_X7_IDX,
    [ 8] = portSTACK_X8_IDX,
    [ 9] = portSTACK_X9_IDX,
    [10] = portSTACK_X10_IDX,
    [11] = portSTACK_X11_IDX,
    [12] = portSTACK_X12_IDX,
    [13] = portSTACK_X13_IDX,
    [14] = portSTACK_X14_IDX,
    [15] = portSTACK_X15_IDX,
    [16] = portSTACK_X16_IDX,
    [17] = portSTACK_X17_IDX,
    [18] = portSTACK_X18_IDX,
    [19] = portSTACK_X19_IDX,
    [20] = portSTACK_X20_IDX,
    [21] = portSTACK_X21_IDX,
    [22] = portSTACK_X22_IDX,
    [23] = portSTACK_X23_IDX,
    [24] = portSTACK_X24_IDX,
    [25] = portSTACK_X25_IDX,
    [26] = portSTACK_X26_IDX,
    [27] = portSTACK_X27_IDX,
    [28] = portSTACK_X28_IDX,
    [29] = portSTACK_X29_IDX,
    [30] = portSTACK_X30_IDX,
    [31] = portSTACK_X31_IDX,
};

int tvisor_guest_page_fault_exception_handler(uint64_t sstatus,uint64_t sepc,uint64_t stval,uint64_t hstatus,uint64_t htval,uint64_t hinst,uint64_t *stack_ctx){
    uint32_t fault_inst;
    size_t   fault_pc;
    size_t   fault_addr;
    uint64_t fault_value;
    uint8_t  fault_access_type;
    uint8_t  fault_access_size;
    uint64_t pte;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t rd;
    uint8_t dev_idx;
    tvisor_vm_ctx_ptr_t ctx;
    riscv_hypervisor_t hypervisor_ctx;
    hypervisor_ctx = uxTaskCurrentHypervisorCtxGet();
    ctx = hypervisor_ctx.ctx;
    if(hstatus & HSTATUS_SPV){
        //VS/VU Mode
        fault_pc = sepc;
        fault_inst = tvisor_vm_read32(fault_pc);
        if((fault_inst & 0x03) == 0x03){
            rs1 = RISCV_INST_RS1(fault_inst);
            rs2 = RISCV_INST_RS2(fault_inst);
            rd  = RISCV_INST_RD(fault_inst);
            fault_addr = stack_ctx[regs_stack_idx_map[rs1]];
            fault_value = stack_ctx[regs_stack_idx_map[rs2]];
        }
        else{
            rs1 = RISCV_C_INST_RS1(fault_inst);
            rs2 = RISCV_C_INST_RS2(fault_inst);
            rd  = RISCV_C_INST_RD(fault_inst);
            fault_addr = stack_ctx[regs_stack_idx_map[rs1]];
            fault_value = stack_ctx[regs_stack_idx_map[rs2]];
            switch(fault_inst & RISCV_C_INST_DECODE_MASK0){
                case RISCV_INST_DECODE_C_LW:{
                    fault_access_type = 1;
                    fault_access_size = 2;
                    break;
                }
                case RISCV_INST_DECODE_C_SW:{
                    fault_access_type = 0;
                    fault_access_size = 2;
                    break;
                }
                default:{
                    break;
                }
            }
        }
    }
    else{
        fault_pc = sepc;
        fault_inst = *(volatile uint32_t *)fault_pc;
        if((fault_inst & 0x03) == 0x03){
            rs1 = RISCV_INST_RS1(fault_inst);
            rs2 = RISCV_INST_RS2(fault_inst);
            rd  = RISCV_INST_RD(fault_inst);
            fault_addr = stack_ctx[regs_stack_idx_map[rs1]];
            fault_value = stack_ctx[regs_stack_idx_map[rs2]];
        }
        else{
            while(1);
        }
        switch (fault_inst & RISCV_INST_DECODE_MASK0) {
            case RISCV_INST_DECODE_HLV_B:{
                break;
            }
            case RISCV_INST_DECODE_HLV_BU:{
                break;
            }
            case RISCV_INST_DECODE_HLV_H:{
                break;
            }
            case RISCV_INST_DECODE_HLV_HU:{
                break;
            }
            case RISCV_INST_DECODE_HLV_W:{
                break;
            }
            case RISCV_INST_DECODE_HLVX_HU:{
                break;
            }
            case RISCV_INST_DECODE_HLVX_WU:{
                break;
            }
            case RISCV_INST_DECODE_HLV_WU:{
                break;
            }
            case RISCV_INST_DECODE_HLV_D:{
                break;
            }
            default:{
                switch(fault_inst & RISCV_INST_DECODE_MASK1){
                    case RISCV_INST_DECODE_HSV_B:{
                        break;
                    }
                    case RISCV_INST_DECODE_HSV_H:{
                        break;
                    }
                    case RISCV_INST_DECODE_HSV_W:{
                        fault_access_type = 0;
                        fault_access_size = 2;
                        break;
                    }
                    case RISCV_INST_DECODE_HSV_D:{
                        break;
                    }
                    default:{
                        break;
                    }
                }
                break;
            }
        }
    }
    if(tvisor_mmu_get_leaf_pte(ctx, fault_addr, &pte) != TVISOR_STATUS_OK){
        while(1);
    }
    dev_idx = (pte >> 10) & 0xFFFF;
    if(fault_access_type == 0){
        tvisor_dev_ops_list[ctx->dev_list[dev_idx].type]->write_register(
            (tvisor_dev_uart16550_ctx_t *)&(ctx->dev_list[dev_idx].ctx),
            fault_addr - ctx->dev_list[dev_idx].region.start_addr,
            fault_value,
            fault_access_size
        );
    }
    else{
        tvisor_dev_ops_list[ctx->dev_list[dev_idx].type]->read_register(
            (tvisor_dev_uart16550_ctx_t *)&(ctx->dev_list[dev_idx].ctx),
            fault_addr - ctx->dev_list[dev_idx].region.start_addr,
            &fault_value,
            fault_access_size
        );
        stack_ctx[regs_stack_idx_map[rd]] = fault_value;
    }
    return TVISOR_STATUS_OK;
}
