#ifndef __TVISOR_DEV_UART16550_H
#define __TVISOR_DEV_UART16550_H


#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"


/* THR:transmitter holding register */
#define TVISOR_UART_DAT    (0x00) /* 数据寄存器*/
#define TVISOR_UART_IER    (0x01) /* 中断使能寄存器*/
#define TVISOR_UART_IIR    (0x02) /* 中断标识寄存器 (read only)*/
#define TVISOR_UART_FCR    (0x02) /* FIFO控制寄存器 (write only)*/
#define TVISOR_UART_LCR    (0x03) /* 线路控制寄存器*/
#define TVISOR_UART_MCR    (0x04) /* MODEN控制寄存器*/
#define TVISOR_UART_LSR    (0x05) /* 线路状态寄存器*/
#define TVISOR_UART_MSR    (0x06) /* MODEN状态寄存器*/

#define TVISOR_UART_DLL (0x00)  /*预分频寄存器低8位*/
#define TVISOR_UART_DLM (0x01)  /*预分频寄存器高8位*/

#define TVISOR_UART_LSR_ERROR   0x80  /* 出错 */
#define TVISOR_UART_LSR_EMPTY    0x40  /* 传输FIFO和移位寄存器为空 */
#define TVISOR_UART_LSR_TFE	    0x20  /* 传输FIFO为空 */
#define TVISOR_UART_LSR_BI	    0x10  /* 传输被打断 */
#define TVISOR_UART_LSR_FE	    0x08  /* 接收到没有停止位的帧 */
#define TVISOR_UART_LSR_PE	    0x04  /* 奇偶校验错误位 */
#define TVISOR_UART_LSR_OE	    0x02  /* 数据溢出 */
#define TVISOR_UART_LSR_DR	    0x01  /* FIFO有数据 */


struct tvisor_dev_uart16550_ctx_struct_t;
typedef struct tvisor_dev_uart16550_ctx_struct_t tvisor_dev_uart16550_ctx_t;


typedef int (* tvisor_dev_uart16550_func_rx_t)(tvisor_dev_uart16550_ctx_t *ctx,uint8_t data);
typedef int (* tvisor_dev_uart16550_func_tx_t)(tvisor_dev_uart16550_ctx_t *ctx,uint8_t data);

struct tvisor_dev_uart16550_ctx_struct_t{
    tvisor_dev_uart16550_func_rx_t func_rx_from_isr;
    tvisor_dev_uart16550_func_rx_t func_rx;
    tvisor_dev_uart16550_func_tx_t func_tx;
    QueueHandle_t rx_queue;
};

#endif
