#include "printf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task.h" /* RTOS task related API prototypes. */
#include "queue.h" /* RTOS queue related API prototypes. */
#include "timers.h" /* Software timer related API prototypes. */
#include "semphr.h" /* Semaphore related API prototypes. */
#include <stddef.h>
#include <sys/cdefs.h>
#include "csr.h"
// SBI扩展ID（ASCII码 "TIME"）
#define SBI_EXT_TIMER          0x54494D45
// TIMER扩展功能ID
#define SBI_TIMER_SET_TIMER    0
#define SBI_TIMER_CLEAR_IPI    1
#define SBI_TIMER_SEND_IPI     2

struct sbiret 
{
	long error;
	long value;
};


volatile int a = 5;
volatile int b;

TaskHandle_t task_0_main_handler;
TaskHandle_t task_1_main_handler;

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName ){

                                        }

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
			unsigned long arg1, unsigned long arg2,
			unsigned long arg3, unsigned long arg4,
			unsigned long arg5)
{
	struct sbiret ret;

	register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);
	register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);
	register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);
	register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);
	register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);
	register uintptr_t a5 asm ("a5") = (uintptr_t)(arg5);
	register uintptr_t a6 asm ("a6") = (uintptr_t)(fid);
	register uintptr_t a7 asm ("a7") = (uintptr_t)(ext);
	asm volatile ("ecall"
		      : "+r" (a0), "+r" (a1)
		      : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
		      : "memory");
        
	ret.error = a0;
	ret.value = a1;

	return ret;
}

void sbi_print(char *str){
    do{
        sbi_ecall(1,0,*str,0,0,0,0,0);
    }while(*str++);
}


struct sbiret sbi_timer_set(uint64_t timeout) {
    struct sbiret ret = sbi_ecall(SBI_EXT_TIMER, SBI_TIMER_SET_TIMER, timeout, 0, 0,0,0,0);
    return ret;
}

void irq_stimer_handler(void){
    size_t t;
    t = 100000+read_csr(time);
    sbi_timer_set( t);
}


void task_0_main( void * arg ){
    int i=0;
    printf("enter task_0_main...\n");
    while(1){
        printf("task_0_main:%d\n",i++);
        vTaskDelay(10);
    }
}



void task_1_main( void * arg ){
    int i=0;
    printf("enter task_1_main...\n");
    while(1){
        printf("task_1_main:%d\n",i++);
        vTaskDelay(10);
    }
}

int freertos_main(void){
    sbi_print("hello opensbi!\n");
    xTaskCreate(task_0_main,"task_0_main",2028,NULL,4,&task_0_main_handler);
    xTaskCreate(task_1_main,"task_1_main",2028,NULL,4,&task_1_main_handler);
    vTaskStartScheduler();
    // task_1_main(NULL);
    return 0;
}

 extern int freertos_main(void);

int main(void){
  uint32_t value; 
  // value = read_csr(misa);
  printf("hello world,misa=%08x\n",value);
  // cpu_switch_to_hs_mode();
  // cpu_switch_to_vs_mode();
  printf("freertos_main......\n");
  freertos_main();
}

 