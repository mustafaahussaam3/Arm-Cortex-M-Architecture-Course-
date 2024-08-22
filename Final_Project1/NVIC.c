 /**************************************************************************************************
 *
 * Module: NVIC
 *
 * File Name: NVIC.c
 *
 * Description: source file for the NVIC ARM driver
 *
 * Author: Mustafa Hussam Eldin
 *
 ***************************************************************************************************/

#include "NVIC.h"
#include "tm4c123gh6pm_registers.h"

 /**************************************************************************************************
 * Name: NVIC_EnableIRQ
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): IRQ_Num - Number of the IRQ from the target vector table
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to enable Interrupt request for specific IRQ
 ***************************************************************************************************/

void NVIC_EnableIRQ (NVIC_IRQType IRQ_Num)
{
    uint8 IRQ_Num_Map = IRQ_Num % 32 ;
    uint8 IRQ_Num_ENn = (IRQ_Num - IRQ_Num_Map) / 32;
    volatile uint32 *nvic_en_ptr = (volatile uint32*)(NVIC_EN_BASE + (IRQ_Num_ENn*4));
    *nvic_en_ptr |= ( 1 << IRQ_Num_Map );
}

/**************************************************************************************************
* Name: NVIC_DisableIRQ
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): IRQ_Num - Number of the IRQ from the target vector table
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to disable Interrupt request for specific IRQ
***************************************************************************************************/

void NVIC_DisableIRQ (NVIC_IRQType IRQ_Num)
{
    uint8 IRQ_Num_Map = IRQ_Num % 32 ;
    uint8 IRQ_Num_DISn = (IRQ_Num - IRQ_Num_Map) / 32;
    volatile uint32 *nvic_dis_ptr = (volatile uint32*)(NVIC_DIS_BASE + (IRQ_Num_DISn*4));
    *nvic_dis_ptr |= ( 1 << IRQ_Num_Map );
}

/**************************************************************************************************
* Name: NVIC_SetPriorityIRQ
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): IRQ_Num - Number of the IRQ from the target vector table,
*                  IRQ_Priority - Priority from 0(Highest) to 7 in NVIC
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to set priority for specific IRQ
***************************************************************************************************/

void NVIC_SetPriorityIRQ (NVIC_IRQType IRQ_Num, NVIC_IRQPriorityType IRQ_Priority)
{
    uint8 IRQ_Num_Map = IRQ_Num % 4;
    uint8 IRQ_Num_PRIn = ( IRQ_Num - IRQ_Num_Map) / 4 ;
    uint8 PRI_Map = ( IRQ_Num_Map * 8 ) + 5;
    volatile uint32 *nvic_pri_ptr = (volatile uint32 *)(NVIC_PRI_BASE + (IRQ_Num_PRIn*4));
    *nvic_pri_ptr = ( *nvic_pri_ptr & ~( 0xF << PRI_Map)) | (IRQ_Priority << PRI_Map);
}

/**************************************************************************************************
* Name: NVIC_EnableException
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Exception_Num - Number of the Exception from the target vector table
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to enable Interrupt request for specific Exception
***************************************************************************************************/

void NVIC_EnableException (NVIC_ExceptionType Exception_Num)
{
    switch (Exception_Num)
    {
    case EXCEPTION_MEM_FAULT_TYPE :   NVIC_SYSTEM_SYSHNDCTRL =  MEM_FAULT_ENABLE_MASK;
    break;
    case EXCEPTION_BUS_FAULT_TYPE :   NVIC_SYSTEM_SYSHNDCTRL =  BUS_FAULT_ENABLE_MASK;
      break;
    case EXCEPTION_USAGE_FAULT_TYPE : NVIC_SYSTEM_SYSHNDCTRL =  USAGE_FAULT_ENABLE_MASK;
      break;
    case EXCEPTION_SVC_TYPE: TRIGGER_SVC_EXCEPTIOM ;
      break;
    case EXCEPTION_PEND_SV_TYPE:      NVIC_SYSTEM_INTCTRL    |= ( 1 << PENDSV_ENABlE_NUM );
      break;
    }
}

/**************************************************************************************************
* Name: NVIC_DisableException
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Exception_Num - Number of the Exception from the target vector table
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to disable Interrupt request for specific Exception
***************************************************************************************************/

void NVIC_DisableException (NVIC_ExceptionType Exception_Num)
{
    switch (Exception_Num)
    {
    case EXCEPTION_MEM_FAULT_TYPE :   NVIC_SYSTEM_SYSHNDCTRL &= ~( 1 << MEMORY_MANAGEMENT_DISABLE_NUM );
      break;
    case EXCEPTION_BUS_FAULT_TYPE :   NVIC_SYSTEM_SYSHNDCTRL &= ~( 1 << BUS_FAULT_DISABLE_NUM );
      break;
    case EXCEPTION_USAGE_FAULT_TYPE : NVIC_SYSTEM_SYSHNDCTRL &= ~( 1 << USAGE_FAULT_DISABLE_NUM );
      break;
    case EXCEPTION_PEND_SV_TYPE:      NVIC_SYSTEM_INTCTRL    |=  ( 1 << PENDSV_DISABLE_NUM );
      break;
    }

}

/**************************************************************************************************
* Name: NVIC_SetPriorityException
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Exception_Num - Number of the Exception from the target vector table
*                  Exception_Priority - Select Priority for exception from 0 to 7
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to enable Interrupt request for specific Exception
***************************************************************************************************/

void NVIC_SetPriorityException (NVIC_ExceptionType Exception_Num, NVIC_ExceptionPriorityType Exception_Priority)
{
    switch (Exception_Num)
    {
    case EXCEPTION_MEM_FAULT_TYPE:  NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG & (MEM_FAULT_PRIORITY_MASK))   |
            ( Exception_Priority << MEM_FAULT_PRIORITY_BITS_POS );
          break;
    case EXCEPTION_BUS_FAULT_TYPE:  NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG & (BUS_FAULT_PRIORITY_MASK))   |
            ( Exception_Priority << BUS_FAULT_PRIORITY_BITS_POS );
          break;
    case EXCEPTION_USAGE_FAULT_TYPE:NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG & (USAGE_FAULT_PRIORITY_MASK)) |
            ( Exception_Priority << USAGE_FAULT_PRIORITY_BITS_POS );
          break;
    case EXCEPTION_SVC_TYPE:        NVIC_SYSTEM_PRI2_REG = ( NVIC_SYSTEM_PRI2_REG &  (SVC_PRIORITY_MASK))        |
            ( Exception_Priority << SVC_PRIORITY_BITS_POS );
          break;
    case EXCEPTION_DEBUG_MONITOR_TYPE: NVIC_SYSTEM_PRI3_REG=(NVIC_SYSTEM_PRI3_REG &(DEBUG_MONITOR_PRIORITY_MASK))|
            ( Exception_Priority << DEBUG_MONITOR_PRIORITY_BITS_POS );
          break;
    case EXCEPTION_PEND_SV_TYPE:    NVIC_SYSTEM_PRI3_REG = ( NVIC_SYSTEM_PRI3_REG & (PENDSV_PRIORITY_MASK))      |
            ( Exception_Priority << PENDSV_PRIORITY_BITS_POS );
          break;
    case EXCEPTION_SYSTICK_TYPE:    NVIC_SYSTEM_PRI3_REG = ( NVIC_SYSTEM_PRI3_REG & (SYSTICK_PRIORITY_MASK))     |
            ( Exception_Priority << SYSTICK_PRIORITY_BITS_POS );
          break;
    }
}
