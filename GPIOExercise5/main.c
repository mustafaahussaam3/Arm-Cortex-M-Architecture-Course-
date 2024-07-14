
/*
 * Exercise 5 Bit Banding Feature 2
 *
 * Eng Mohammed Tarek Course
 *
 * Advanced Embedded System
 *
 * Author: Mustafa Hussam Eldin
 */

#include "tm4c123gh6pm_registers.h"

#define Red     1
#define Green   2
#define Blue    3
#define SET_BIT(REG, BIT_NUM)        (REG |= (1<<BIT_NUM))
#define CLEAR_BIT(REG, BIT_NUM)      (REG &=~(1<<BIT_NUM))
#define TOGGLE_BIT(REG, BIT_NUM)     (REG ^= (1<<BIT_NUM))
#define BIT_IS_SET(REG, BIT_NUM)     (REG &  (1<<BIT_NUM))
#define BIT_IS_CLEAR(REG, BIT_NUM) (!(REG &  (1<<BIT_NUM)))
#define PF1_PF2_PF3                  (*((volatile uint32*)(0x40025000 + 0x038)))
#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

void Leds_Init(void)
{
    GPIO_PORTF_AMSEL_REG    &= 0xF1;
    GPIO_PORTF_PCTL_REG     &= 0xFFFF000F;
    GPIO_PORTF_AFSEL_REG    &= 0xF1;
    GPIO_PORTF_DIR_REG      |= 0x0E;
    GPIO_PORTF_DEN_REG      |= 0x0E;
    GPIO_PORTF_DATA_REG     &= 0xF1;
}

void SysTick_Init(void)
{
    SYSTICK_CTRL_REG         &= ~(1<<0);
    SYSTICK_RELOAD_REG       = 7999999;
    SYSTICK_CURRENT_REG      = 0;
    SYSTICK_CTRL_REG         = 0x00000005;
}

void PORTF_CLK (void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
}

int main(void)
{

    PORTF_CLK();
    Leds_Init();
    int i;
    while (1)
    {
        for ( i =1; i<4; i++)
        {
            PF1_PF2_PF3 = (PF1_PF2_PF3 & 0x00) | (1<<i);
            Delay_MS(1000);
        }

    }
}
