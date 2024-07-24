
/*
 * Exercise 3 Bus Fault
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



void BusFault_Init(void)
{
    SET_BIT(NVIC_SYSTEM_SYSHNDCTRL, 17);
    NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG & 0xFFFF1FFF ) | (2 << 13);
    //NVIC_SYSTEM_CFGCTRL
}

void BusFault_Handler()
{
    while(1);
}

int main(void)
{

    volatile uint32 *ptr =  (unsigned long*)0x20008000 ;
    *ptr = 20;

    while (1)
    {
    }
	return 0;
}
