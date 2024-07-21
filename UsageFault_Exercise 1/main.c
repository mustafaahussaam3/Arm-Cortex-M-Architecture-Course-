
/*
 * Exercise 1 Usage Fault
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

int divide(int x, int y)
{
    return x/y;
}

int main(void)
{
    SET_BIT(NVIC_SYSTEM_CFGCTRL, DIV0);
    while (1)
    {
        divide(9,0);
    }
    return 0;
}
