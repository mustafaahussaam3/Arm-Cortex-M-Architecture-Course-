
/*
 * Exercise 6 Bit Banding Feature 3 (Read Operation)
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
#define PF0_PF4                      (*((volatile uint32*)(0x40025000 + 0x044)))
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

void SW1_Init(void)
{
    GPIO_PORTF_AMSEL_REG     &= ~(1<<4);
    GPIO_PORTF_PCTL_REG      &= 0xFFF0FFFF;
    GPIO_PORTF_AFSEL_REG     &= ~(1<<4);
    GPIO_PORTF_DIR_REG       &= ~(1<<4);
    GPIO_PORTF_PUR_REG       |= (1<<4);
    GPIO_PORTF_DEN_REG       |= (1<<4);
}

void PORTF_CLK (void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
}

int main(void)
{
    PORTF_CLK();
    SW1_Init();
    SW2_Init();
    Leds_Init();
    while (1)
    {
          if (!(PF0_PF4 & 0x10)){
              PF1_PF2_PF3 = 0x03;   /* Purble */
              Delay_MS(500);
          }
          else if(!(PF0_PF4 & 0x01)){
              PF1_PF2_PF3 = 0x0C;   /* Cyan */
              Delay_MS(500);
          }
          else
              PF1_PF2_PF3 = 0x00;
    }
}
