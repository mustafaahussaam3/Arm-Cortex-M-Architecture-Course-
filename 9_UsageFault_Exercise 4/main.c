
/*
 * Exercise 3 Usage Fault
 *
 * Eng Mohammed Tarek Course
 *
 * Advanced Embedded System
 *
 * Author: Mustafa Hussam Eldin
 */

#include "tm4c123gh6pm_registers.h"


#define Enable_Exceptions()     __asm(" CPSIE I ")
#define Disable_Exceptions()    __asm(" CPSID I ")
#define Enable_Faults()         __asm(" CPSIE F ")
#define Disable_Faults()        __asm(" CPSIE F ")

#define SET_BIT(REG, BIT_NUM)         (REG |= (1<<BIT_NUM))
#define CLEAR_BIT(REG, BIT_NUM)       (REG &=~(1<<BIT_NUM))
#define TOGGLE_BIT(REG, BIT_NUM)      (REG ^= (1<<BIT_NUM))
#define BIT_IS_SET(REG, BIT_NUM)      (REG &  (1<<BIT_NUM))
#define BIT_IS_CLEAR(REG, BIT_NUM)    (!(REG &  (1<<BIT_NUM))
#define DIV0                           4
#define UNALIGNED                      3


void UsageFault_Init(void)
{
    SET_BIT(NVIC_SYSTEM_SYSHNDCTRL, 18);
    NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG & 0xFF1FFFFF ) | (2 << 13);
    SET_BIT(NVIC_SYSTEM_CFGCTRL, UNALIGNED);
}

void UsageFault_Handler()
{
    while(1);
}



int main(void)
{
    UsageFault_Init();
    /* Point to the SysTick Control Register Address */
    volatile uint32 * ptr1 = (volatile unsigned long *)0xE000E010;

    /* Point to the SysTick Control Register Address + 1 */
    volatile uint8 * ptr2 = (volatile unsigned char *)0xE000E011;

    /* Point to the SysTick Control Register Address + 2 */
    volatile uint16 * ptr3 = (volatile unsigned short *)0xE000E012;

    /* Point to the SysTick Control Register Address + 1 */
    volatile uint32 * ptr4 = (volatile unsigned long *)0xE000E011;

    /* Enable Interrupts, Exceptions and Faults */
    Enable_Exceptions();
    Enable_Faults();

    /* Set the UNALIGNED bit in the CFGCTRL register to trigger exception when unaligned memory access happen */
    NVIC_SYSTEM_CFGCTRL |= (1<<3);

    *ptr1 = 10; /* Memory aligned access */

    *ptr2 = 10; /* Memory aligned access */

    *ptr3 = 10; /* Memory aligned access */

    *ptr4 = 10; /* Memory Unaligned access, It should trigger Hard Fault as the Usage Fault is disabled */
    while (1){}
}
