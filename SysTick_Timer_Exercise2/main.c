
/*
 * Exercise 1 systick
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

//PF2
void Led_Blue_Init(void)
{
    GPIO_PORTF_AMSEL_REG    &= ~(1<<2);
    GPIO_PORTF_PCTL_REG     &= 0xFFFFF0FF;
    GPIO_PORTF_AFSEL_REG    &= ~(1<<2);
    GPIO_PORTF_DIR_REG      |= (1<<2);
    GPIO_PORTF_DEN_REG      |= (1<<2);
    GPIO_PORTF_DATA_REG     &= ~(1<<2);
}

//PF3
void Led_Green_Init(void)
{
    GPIO_PORTF_AMSEL_REG    &= ~(1<<3);
    GPIO_PORTF_PCTL_REG     &= 0xFFFF0FFF;
    GPIO_PORTF_AFSEL_REG    &= ~(1<<3);
    GPIO_PORTF_DIR_REG      |= (1<<3);
    GPIO_PORTF_DEN_REG      |= (1<<3);
    GPIO_PORTF_DATA_REG     &= ~(1<<3);
}

//PF4
void SW1_Init(void)
{
    GPIO_PORTF_AMSEL_REG     &= ~(1<<4);
    GPIO_PORTF_PCTL_REG      &= 0xFFF0FFFF;
    GPIO_PORTF_AFSEL_REG     &= ~(1<<4);
    GPIO_PORTF_DIR_REG       &= ~(1<<4);
    GPIO_PORTF_PUR_REG       |= (1<<4);
    GPIO_PORTF_DEN_REG       |= (1<<4);
}

void SysTick_Init(void)
{
    SYSTICK_CTRL_REG         &= ~(1<<0);
    SYSTICK_RELOAD_REG       = 7999999;
    SYSTICK_CURRENT_REG      = 0;
    SYSTICK_CTRL_REG         = 0x00000005;
}

int main(void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
    SW2_Init();
    Led_Red_Init();
    SysTick_Init();

    while (1)
    {
        if (!(GPIO_PORTF_DATA_REG & (1<<0)))
        {
            GPIO_PORTF_DATA_REG |= 1<<1 ;
            while(!(SYSTICK_CTRL_REG & (1<<16)));
            GPIO_PORTF_DATA_REG &= ~(1<<1) ;
            while(!(SYSTICK_CTRL_REG & (1<<16)));
        }
        else
            GPIO_PORTF_DATA_REG &= ~(1<<1) ;

    }
    return 0;
}
