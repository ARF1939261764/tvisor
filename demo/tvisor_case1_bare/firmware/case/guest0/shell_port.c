/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright (c) 2019 Letter
 * 
 */

// // #include "FreeRTOS.h"
// #include "task.h"
#include "shell.h"
#include <stdint.h>
#include <stddef.h>
#include "riscv_csr_encoding.h"

uint64_t get_tick(void){
    return read_csr(time) / 10000;
}

Shell shell;
char shellBuffer[512];


/**
 * @brief 用户shell写
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    uint32_t count = len;
    while(count--){
        *(volatile uint32_t *)0x10000000 =*data++;
    }
    return len;
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @param len 数据长度
 * 
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    uint32_t i;
    if(len == 0){
        return 0;
    }
    i = 0;
    while((*(volatile uint8_t *)(0x10000000 + 0x05) & 0x01)){
        data[i++] = *(volatile uint32_t *)(0x10000000 + 0x00);
        // userShellWrite(data + i - 1,1);
        if(i >= len){
            break;
        }
    }
    return i;
}

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(void)
{
    uint64_t time;
    shell.write = userShellWrite;
    shell.read = userShellRead;
    shellInit(&shell, shellBuffer, 512);
    shellTask(&shell);

}
