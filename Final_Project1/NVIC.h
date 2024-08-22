 /************************************************************************************************************************************
 *
 * Module: NVIC
 *
 * File Name: NVIC.h
 *
 * Description: Header file for the NVIC ARM driver
 *
 * Author: Mustafa Hussam Eldin
 *
 **************************************************************************************************************************************/

#ifndef NVIC_H_
#define NVIC_H_

/**************************************************************************************************************************************
 *                                                             Inclusions                                                             *
 **************************************************************************************************************************************/

#include "std_types.h"

/**************************************************************************************************************************************
 *                                                      Preprocessor Definitions                                                      *
 **************************************************************************************************************************************/

#define NVIC_EN_BASE  0xE000E100
#define NVIC_DIS_BASE 0xE000E180
#define NVIC_PRI_BASE 0xE000E400

#define Enable_Exceptions()     __asm(" CPSIE I ")
#define Disable_Exceptions()    __asm(" CPSID I ")
#define Enable_Faults()         __asm(" CPSIE F ")
#define Disable_Faults()        __asm(" CPSIE F ")
#define TRIGGER_SVC_EXCEPTIOM   __asm(" SVC #0 ")

#define MEM_FAULT_PRIORITY_MASK              0x000000E0
#define MEM_FAULT_PRIORITY_BITS_POS          5

#define BUS_FAULT_PRIORITY_MASK              0x0000E000
#define BUS_FAULT_PRIORITY_BITS_POS          13

#define USAGE_FAULT_PRIORITY_MASK            0x00E00000
#define USAGE_FAULT_PRIORITY_BITS_POS        21

#define SVC_PRIORITY_MASK                    0xE0000000
#define SVC_PRIORITY_BITS_POS                29

#define DEBUG_MONITOR_PRIORITY_MASK          0x000000E0
#define DEBUG_MONITOR_PRIORITY_BITS_POS      5

#define PENDSV_PRIORITY_MASK                 0x00E00000
#define PENDSV_PRIORITY_BITS_POS             21

#define SYSTICK_PRIORITY_MASK                0xE0000000
#define SYSTICK_PRIORITY_BITS_POS            29

#define MEMORY_MANAGEMENT_DISABLE_NUM        16
#define BUS_FAULT_DISABLE_NUM                17
#define USAGE_FAULT_DISABLE_NUM              18

#define PENDSV_ENABlE_NUM                    28
#define PENDSV_DISABLE_NUM                   27

#define MEM_FAULT_ENABLE_MASK                0x00010000
#define BUS_FAULT_ENABLE_MASK                0x00020000
#define USAGE_FAULT_ENABLE_MASK              0x00040000

/***************************************************************************************************************************************
 *                                                      Data Types Declarations                                                        *
 ***************************************************************************************************************************************/

typedef enum
{
    EXCEPTION_RESET_TYPE,
    EXCEPTION_NMI_TYPE,
    EXCEPTION_HARD_FAULT_TYPE,
    EXCEPTION_MEM_FAULT_TYPE,
    EXCEPTION_BUS_FAULT_TYPE,
    EXCEPTION_USAGE_FAULT_TYPE,
    EXCEPTION_SVC_TYPE,
    EXCEPTION_DEBUG_MONITOR_TYPE,
    EXCEPTION_PEND_SV_TYPE,
    EXCEPTION_SYSTICK_TYPE
}NVIC_ExceptionType;
typedef uint8 NVIC_IRQType;
typedef uint8 NVIC_IRQPriorityType;
typedef uint8 NVIC_ExceptionPriorityType;

/***************************************************************************************************************************************
 *                                                         Global Variables                                                            *
 ***************************************************************************************************************************************/

extern volatile uint32 *nvic_pri_ptr;
extern volatile uint32 *nvic_en_ptr;
extern volatile uint32 *nvic_dis_ptr;

/***************************************************************************************************************************************
 *                                                       Functions Prototypes                                                          *
 ***************************************************************************************************************************************/

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

extern void NVIC_EnableIRQ (NVIC_IRQType IRQ_Num);

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

extern void NVIC_DisableIRQ (NVIC_IRQType IRQ_Num);

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

extern void NVIC_SetPriorityIRQ (NVIC_IRQType IRQ_Num, NVIC_IRQPriorityType IRQ_Priority);

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

extern void NVIC_EnableException (NVIC_ExceptionType Exception_Num);

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


extern void NVIC_DisableException (NVIC_ExceptionType Exception_Num);

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

extern void NVIC_SetPriorityException (NVIC_ExceptionType Exception_Num, NVIC_ExceptionPriorityType Exception_Priority);

/****************************************************************************************************************************************
 *                                                            End of File                                                               *
 ****************************************************************************************************************************************/

#endif /* NVIC_H_ */

