
/*
 * Exercise 3 GPIO
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

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

////PF0
//void SW2_Init(void)
//{
//    GPIO_PORTF_LOCK_REG       = 0x4C4F434B;
//    GPIO_PORTF_CR_REG        |= 0x01;
//    GPIO_PORTF_AMSEL_REG     &= 0xFE;
//    GPIO_PORTF_PCTL_REG      &= 0xFFFFFFF0;
//    GPIO_PORTF_AFSEL_REG     &= 0xFE;
//    GPIO_PORTF_DIR_REG       &= 0xFE;
//    GPIO_PORTF_PUR_REG       |= 0x01;
//    GPIO_PORTF_DEN_REG       |= 0x01;
//}

////PF1
//void Red_Init(void)
//{
//    GPIO_PORTF_AMSEL_REG    &= 0xFD;
//    GPIO_PORTF_PCTL_REG     &= 0xFFFFFF0F;
//    GPIO_PORTF_AFSEL_REG    &= 0xFD;
//    GPIO_PORTF_DIR_REG      |= 0x02;
//    GPIO_PORTF_DEN_REG      |= 0x02;
//    GPIO_PORTF_DATA_REG     &= ~(1<<1);
//}
//
////PF2
//void Blue_Init(void)
//{
//    GPIO_PORTF_AMSEL_REG    &= ~(1<<2);
//    GPIO_PORTF_PCTL_REG     &= 0xFFFFF0FF;
//    GPIO_PORTF_AFSEL_REG    &= ~(1<<2);
//    GPIO_PORTF_DIR_REG      |= (1<<2);
//    GPIO_PORTF_DEN_REG      |= (1<<2);
//    GPIO_PORTF_DATA_REG     &= ~(1<<2);
//}
//
//void Led_Green_Init(void)
//{
//    GPIO_PORTF_AMSEL_REG    &= ~(1<<3);
//    GPIO_PORTF_PCTL_REG     &= 0xFFFF0FFF;
//    GPIO_PORTF_AFSEL_REG    &= ~(1<<3);
//    GPIO_PORTF_DIR_REG      |= (1<<3);
//    GPIO_PORTF_DEN_REG      |= (1<<3);
//    GPIO_PORTF_DATA_REG     &= ~(1<<3);
//}


////PF4
//void SW1_Init(void)
//{
//    GPIO_PORTF_AMSEL_REG     &= ~(1<<4);
//    GPIO_PORTF_PCTL_REG      &= 0xFFF0FFFF;
//    GPIO_PORTF_AFSEL_REG     &= ~(1<<4);
//    GPIO_PORTF_DIR_REG       &= ~(1<<4);
//    GPIO_PORTF_PUR_REG       |= (1<<4);
//    GPIO_PORTF_DEN_REG       |= (1<<4);
//}

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
    int i,j;
    while (1)
    {
        for ( i =1; i<4; i++)
        {
            GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (1<<i);
            Delay_MS(1000);
        }

        for (j = 3; j > 0; j--)
        {
            GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (1<<j);
            Delay_MS(1000);
        }
    }
}
