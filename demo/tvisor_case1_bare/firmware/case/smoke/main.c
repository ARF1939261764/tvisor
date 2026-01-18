#include "stdio.h"
#include "printf.h"
#include "uart.h"
#include "rocc.h"
#include <stdint.h>

volatile int a = 5;
volatile int b;

int virt_puts(char *str){
    while(*str++){
        *(volatile uint32_t *)0x10000000 = str[-1];
    }
    return 0;
}

int main(void){
  virt_puts("hello guest os\n");
  while(1);
}

