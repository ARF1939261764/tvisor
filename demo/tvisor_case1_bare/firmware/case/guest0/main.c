#include "stdio.h"
#include "printf.h"
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
  userShellInit();
  while(1);
}

