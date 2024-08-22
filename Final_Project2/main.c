#include "SysTick.h"
#include "NVIC.h"
#include "tm4c123gh6pm_registers.h"
#include <assert.h>

#define MEM_FAULT_PRIORITY                  1
#define BUS_FAULT_PRIORITY                  2
#define USAGE_FAULT_PRIORITY                3
#define SVC_EXCEPTION_PRIORITY              4
#define DEBUG_MONITOR_EXCEPTION_PRIORITY    5
#define PENDSV_EXCEPTION_PRIORITY           6
#define SYSTICK_EXCEPTION_PRIORITY          7

/* Enable PF1, PF2 and PF3 (RED, Blue and Green LEDs) */
void Leds_Init(void)
{
    GPIO_PORTF_AMSEL_REG &= 0xF1;         /* Disable Analog on PF1, PF2 and PF3 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFF000F;   /* Clear PMCx bits for PF1, PF2 and PF3 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   |= 0x0E;         /* Configure PF1, PF2 and PF3 as output pin */
    GPIO_PORTF_AFSEL_REG &= 0xF1;         /* Disable alternative function on PF1, PF2 and PF3 */
    GPIO_PORTF_DEN_REG   |= 0x0E;         /* Enable Digital I/O on PF1, PF2 and PF3 */
    GPIO_PORTF_DATA_REG  &= 0xF1;         /* Clear bit 0, 1 and 2 in Data register to turn off the leds */
}

void Test_Exceptions_Settings(void)
{
    /* Enable Memory Management Fault and Setup its priority */
    NVIC_EnableException(EXCEPTION_MEM_FAULT_TYPE);
    NVIC_SetPriorityException(EXCEPTION_MEM_FAULT_TYPE,MEM_FAULT_PRIORITY);

    assert(NVIC_SYSTEM_SYSHNDCTRL & MEM_FAULT_ENABLE_MASK);
    assert(((NVIC_SYSTEM_PRI1_REG & MEM_FAULT_PRIORITY_MASK) >> MEM_FAULT_PRIORITY_BITS_POS) == MEM_FAULT_PRIORITY);

    /* Disable Memory Management Fault */
    NVIC_DisableException(EXCEPTION_MEM_FAULT_TYPE);
    assert(!(NVIC_SYSTEM_SYSHNDCTRL & MEM_FAULT_ENABLE_MASK));

    /* Enable Bus Fault and Setup its priority */
    NVIC_EnableException(EXCEPTION_BUS_FAULT_TYPE);
    NVIC_SetPriorityException(EXCEPTION_BUS_FAULT_TYPE,BUS_FAULT_PRIORITY);

    assert(NVIC_SYSTEM_SYSHNDCTRL & BUS_FAULT_ENABLE_MASK);
    assert(((NVIC_SYSTEM_PRI1_REG & BUS_FAULT_PRIORITY_MASK) >> BUS_FAULT_PRIORITY_BITS_POS) == BUS_FAULT_PRIORITY);

    /* Disable Bus Fault */
    NVIC_DisableException(EXCEPTION_BUS_FAULT_TYPE);
    assert(!(NVIC_SYSTEM_SYSHNDCTRL & BUS_FAULT_ENABLE_MASK));

    /* Enable Usage Fault and Setup its priority */
    NVIC_EnableException(EXCEPTION_USAGE_FAULT_TYPE);
    NVIC_SetPriorityException(EXCEPTION_USAGE_FAULT_TYPE,USAGE_FAULT_PRIORITY);

    assert(NVIC_SYSTEM_SYSHNDCTRL & USAGE_FAULT_ENABLE_MASK);
    assert(((NVIC_SYSTEM_PRI1_REG & USAGE_FAULT_PRIORITY_MASK) >> USAGE_FAULT_PRIORITY_BITS_POS) == USAGE_FAULT_PRIORITY);

    /* Disable Usage Fault */
    NVIC_DisableException(EXCEPTION_USAGE_FAULT_TYPE);
    assert(!(NVIC_SYSTEM_SYSHNDCTRL & USAGE_FAULT_ENABLE_MASK));

    /* Setup priority for SVC Exception */
    NVIC_SetPriorityException(EXCEPTION_SVC_TYPE,SVC_EXCEPTION_PRIORITY);
    assert(((NVIC_SYSTEM_PRI2_REG & SVC_PRIORITY_MASK) >> SVC_PRIORITY_BITS_POS) == SVC_EXCEPTION_PRIORITY);

    /* Setup priority for Debug Exception */
    NVIC_SetPriorityException(EXCEPTION_DEBUG_MONITOR_TYPE,DEBUG_MONITOR_EXCEPTION_PRIORITY);
    assert(((NVIC_SYSTEM_PRI3_REG & DEBUG_MONITOR_PRIORITY_MASK) >> DEBUG_MONITOR_PRIORITY_BITS_POS) == DEBUG_MONITOR_EXCEPTION_PRIORITY);

    /* Setup priority for PendSV Exception */
    NVIC_SetPriorityException(EXCEPTION_PEND_SV_TYPE,PENDSV_EXCEPTION_PRIORITY);
    assert(((NVIC_SYSTEM_PRI3_REG & PENDSV_PRIORITY_MASK) >> PENDSV_PRIORITY_BITS_POS) == PENDSV_EXCEPTION_PRIORITY);

    /* Setup priority for SysTick Exception */
    NVIC_SetPriorityException(EXCEPTION_SYSTICK_TYPE,SYSTICK_EXCEPTION_PRIORITY);
    assert(((NVIC_SYSTEM_PRI3_REG & SYSTICK_PRIORITY_MASK) >> SYSTICK_PRIORITY_BITS_POS) == SYSTICK_EXCEPTION_PRIORITY);
}

int main(void)
{
    /* Enable clock for PORTF and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x20;
    while(!(SYSCTL_PRGPIO_REG & 0x20));

    /* Initialize the LEDs as GPIO Pins */
    Leds_Init();

    /* Enable Interrupts, Exceptions and Faults */
    Enable_Exceptions();
    Enable_Faults();

    /* Test all System and Fault Exceptions settings */
    Test_Exceptions_Settings();

    while(1)
    {
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x02; /* Turn on the Red LED and disable the others */
        SysTick_StartBusyWait(1000); /* Wait 1 second using SysTick Timer */
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x04; /* Turn on the Blue LED and disable the others */
        SysTick_StartBusyWait(1000); /* Wait 1 second using SysTick Timer */
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x08; /* Turn on the Green LED and disable the others */
        SysTick_StartBusyWait(1000); /* Wait 1 second using SysTick Timer */
    }
}
