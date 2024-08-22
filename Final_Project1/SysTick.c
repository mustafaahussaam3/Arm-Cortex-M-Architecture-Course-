 /******************************************************************************
 *
 * Module: SysTick
 *
 * File Name: SysTick.c
 *
 * Description: source file for the SysTick ARM driver
 *
 * Author: Mustafa Hussam Eldin
 *
 *******************************************************************************/
#include "SysTick.h"
#include "tm4c123gh6pm_registers.h"

static volatile void (*g_CallBackPtr)(void) = 0;

/*****************************************************************************************
 * Service Name: SysTick_Init
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): a_TimeInMilliSeconds - required time in ms
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Initialize the SysTick timer with the specified time in milliseconds using
 *  interrupts. This function is used to setup the timer to generate periodic interrupts
 *  every specified time in milliseconds.
 *****************************************************************************************/

void SysTick_Init(uint16 a_TimeInMilliSeconds)
{
    SysTick_Stop();
    uint32 counter = ( 15999999 * a_TimeInMilliSeconds ) / 1000 ;
    SYSTICK_RELOAD_REG  = counter;
    SYSTICK_CURRENT_REG = 0;
    SysTick_Start();
}

/*****************************************************************************************
 * Service Name: SysTick_StartBusyWait
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): a_TimeInMilliSeconds - required time in ms
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Initialize the SysTick timer with the specified time in milliseconds
 *  using polling or busy-wait technique. The function should exit when the time
 *  is elapsed and stops the timer at the end.
 *****************************************************************************************/

void SysTick_StartBusyWait(uint16 a_TimeInMilliSeconds)
{
    SysTick_Stop();
    uint32 counter = ( 15999999 * a_TimeInMilliSeconds ) / 1000 ;
    SYSTICK_RELOAD_REG  = counter;
    SYSTICK_CURRENT_REG = 0;
    SysTick_Start();
    while(! ( SYSTICK_CTRL_REG & ( 1 << FLAG_BIT ) ));
    SysTick_Stop();
}

/*****************************************************************************************
 * Service Name: SysTick_Handler
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Handler for SysTick interrupt use to call the call-back function.
 *****************************************************************************************/

void SysTick_Handler(void)
{
    if ( g_CallBackPtr != 0 )
    {
        (*g_CallBackPtr)();
    }
}

/*****************************************************************************************
 * Service Name: SysTick_DeInit
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to De Initalize SysTick.
 *****************************************************************************************/

void SysTick_DeInit(void)
{
    SysTick_Stop();
    SysTick_Start();
}

/*****************************************************************************************
 * Service Name: SysTick_SetCallBack
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): *Ptr2Func - Pointer to funtion has the required function to be set
 *                              in the call back operation
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to setup the SysTick Timer call back to be executed
 *              in SysTick Handler.
 *****************************************************************************************/

void SysTick_SetCallBack(volatile void (*Ptr2Func) (void))
{
    g_CallBackPtr = Ptr2Func;
}

/*****************************************************************************************
 * Service Name: SysTick_Stop
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Stop SysTick.
 *****************************************************************************************/

void SysTick_Stop(void)
{
    SYSTICK_CTRL_REG &= ~( 1<< ENABLE_BIT );
}

/*****************************************************************************************
 * Service Name: SysTick_Start
 * Sync/Async: Synchronous
 * Reentrancy: reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Start SysTick.
 *****************************************************************************************/

void SysTick_Start(void)
{
    SYSTICK_CTRL_REG = ( 1 << ENABLE_BIT ) | ( 1 << INTERRUPT_ENABLE_BIT ) | ( 1 << CLK_SELECT );
}

