/*
 * FreeRTOS Kernel V11.1.0
 * license and copyright intentionally withheld to promote copying into user code.
 */

#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stddef.h>
#include <stdint.h>

BaseType_t xPortStartScheduler( void )
{
    extern void xPortStartFirstTask_asm(void);
    xPortStartFirstTask_asm();
    return pdTRUE;
}

void vPortEndScheduler( void )
{
    for(;;){}
}

StackType_t * pxPortInitialiseStack( StackType_t * pxTopOfStack,
                                     TaskFunction_t pxCode,
                                     void * pvParameters )
{
    StackType_t *StackType;
    uint8_t prv_mode = 0;//0:U,1:S
    uint64_t sstatus_mask = 0;//0:U,1:S
    uint64_t hstatus_mask = 0;//0:U,1:S
    size_t hgatp;
    extern StackType_t * pxPortInitialiseStack_asm( StackType_t * pxTopOfStack,
                                     TaskFunction_t pxCode,
                                     void * pvParameters,
                                    uint64_t sstatus_mask,uint64_t hstatus_mask,size_t hgatp);
    prv_mode = pvParameters!=NULL ? ((task_defualt_args_t *)pvParameters)->prv_mode : RISCV_PRV_U_MODE;
    if((prv_mode == RISCV_PRV_S_MODE) || (prv_mode == RISCV_PRV_HS_MODE) || (prv_mode == RISCV_PRV_VS_MODE)){
        sstatus_mask = 1 << 8;
    }
    if((prv_mode == RISCV_PRV_VS_MODE) || (prv_mode == RISCV_PRV_VU_MODE)){
        hstatus_mask = 1 << 7;
    }
    hgatp = pvParameters!=NULL ? ((task_defualt_args_t *)pvParameters)->hgatp : 0;
    StackType = pxPortInitialiseStack_asm(pxTopOfStack,pxCode,pvParameters,sstatus_mask,hstatus_mask,hgatp);
    
    return StackType;
}


void vPortYield( void )
{
    /* Save the current Context */

    /* Switch to the highest priority task that is ready to run. */
    #if ( configNUMBER_OF_CORES == 1 )
    {
        vTaskSwitchContext();
    }
    #else
    {
        vTaskSwitchContext( portGET_CORE_ID() );
    }
    #endif

    /* Start executing the task we have just switched to. */
}

void prvTickISR( void )
{
    /* Interrupts must have been enabled for the ISR to fire, so we have to
     * save the context with interrupts enabled. */

    #if ( configNUMBER_OF_CORES == 1 )
    {
        /* Maintain the tick count. */
        if( xTaskIncrementTick() != pdFALSE )
        {
            /* Switch to the highest priority task that is ready to run. */
            vTaskSwitchContext();
        }
    }
    #else
    {
        UBaseType_t ulPreviousMask;

        /* Tasks or ISRs running on other cores may still in critical section in
         * multiple cores environment. Incrementing tick needs to performed in
         * critical section. */
        ulPreviousMask = taskENTER_CRITICAL_FROM_ISR();

        /* Maintain the tick count. */
        if( xTaskIncrementTick() != pdFALSE )
        {
            /* Switch to the highest priority task that is ready to run. */
            vTaskSwitchContext( portGET_CORE_ID() );
        }

        taskEXIT_CRITICAL_FROM_ISR( ulPreviousMask );
    }
    #endif /* if ( configNUMBER_OF_CORES == 1 ) */

    /* start executing the new task */
}
