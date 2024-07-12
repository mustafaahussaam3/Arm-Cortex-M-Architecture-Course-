
/*
 * Exercise 1
 *
 * Eng Mohammed Tarek Course
 *
 * Advanced Embedded System
 *
 * Author: Mustafa Hussam Eldin
 */

#include "tm4c123gh6pm_registers.h"

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

int main(void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
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