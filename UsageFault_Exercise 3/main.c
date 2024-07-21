
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

int divide(int x, int y)
{
    return x/y;
}

int main(void)
{
    UsageFault_Init();
    while (1)
    {
        divide(9,0);
    }
    return 0;
}
