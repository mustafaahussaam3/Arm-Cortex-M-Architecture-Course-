
/*
 * Exercise 2 Bus Fault
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

//PF0
void SW2_Init(void)
{
    GPIO_PORTF_LOCK_REG       = 0x4C4F434B;
    GPIO_PORTF_CR_REG        |= 0x01;
    GPIO_PORTF_AMSEL_REG     &= 0xFE;
    GPIO_PORTF_PCTL_REG      &= 0xFFFFFFF0;
    GPIO_PORTF_AFSEL_REG     &= 0xFE;
    GPIO_PORTF_DIR_REG       &= 0xFE;
    GPIO_PORTF_PUR_REG       |= 0x01;
    GPIO_PORTF_DEN_REG       |= 0x01;
}

//PF1
void Led_Red_Init(void)
{
    GPIO_PORTF_AMSEL_REG    &= 0xFD;
    GPIO_PORTF_PCTL_REG     &= 0xFFFFFF0F;
    GPIO_PORTF_AFSEL_REG    &= 0xFD;
    GPIO_PORTF_DIR_REG      |= 0x02;
    GPIO_PORTF_DEN_REG      |= 0x02;
    GPIO_PORTF_DATA_REG     &= ~(1<<1);
}

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
    BusFault_Init();
    SW2_Init();
    Led_Red_Init();


    while (1)
    {
        if (!(GPIO_PORTF_DATA_REG&0x01))
        {
            GPIO_PORTF_DATA_REG |= 1<<1 ;
        }
        else
            GPIO_PORTF_DATA_REG &= ~(1<<1);
    }
	return 0;
}
