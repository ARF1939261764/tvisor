
#include <stdint.h>

void system_initial(void){
  // uart_initial((uart_t *)0x54000000,115200);
}

void _putchar(char character)
{
  *(volatile uint32_t *)0x10000000 = character;
}
