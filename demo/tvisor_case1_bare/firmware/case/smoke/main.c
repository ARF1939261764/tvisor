#include "stdio.h"
#include "printf.h"
#include "uart.h"
#include "rocc.h"

volatile int a = 5;
volatile int b;

int main(void){
  printf("hello guest os\n");
  while(1);
}

