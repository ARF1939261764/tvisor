#include "stdio.h"
#include "uart.h"
#include "rocc.h"
#include <stdint.h>
#include "shell.h"
#include "shell_port.h"

volatile int a = 5;
volatile int b;

extern void uart_putc(uart_t *periph,char ch);

int virt_puts(char *str){
    while(*str++){
        uart_putc((uart_t *)0x10000000,str[-1]);
    }
    return 0;
}

int main(void){
  virt_puts("hello guest os0\n");
  // printf("hello world\r\n");
  userShellInit();
  // while(1){
  //   if(*(volatile uint8_t *)(0x10000000 + 0x05) & 0x01){
  //     uart_putc((uart_t *)0x10000000,*(volatile uint8_t *)(0x10000000));
  //   }
  // }
}

