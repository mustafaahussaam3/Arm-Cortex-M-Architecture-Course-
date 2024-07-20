
/*
 * Exercise 1 PendSV System Exception
 *
 * Eng Mohammed Tarek Course
 *
 * Advanced Embedded System
 *
 * Author: Mustafa Hussam Eldin
 */

#include "tm4c123gh6pm_registers.h"


#define SET_BIT(REG, BIT_NUM)        (REG |= (1<<BIT_NUM))
#define CLEAR_BIT(REG, BIT_NUM)      (REG &=~(1<<BIT_NUM))
#define TOGGLE_BIT(REG, BIT_NUM)     (REG ^= (1<<BIT_NUM))
#define BIT_IS_SET(REG, BIT_NUM)     (REG &  (1<<BIT_NUM))
#define BIT_IS_CLEAR(REG, BIT_NUM) (!(REG &  (1<<BIT_NUM)))
#define PF1_PF2_PF3                  (*((volatile uint32*)(0x40025000 + 0x038)))
#define PF0_PF4                      (*((volatile uint32*)(0x40025000 + 0x044)))
#define WAIT_FOR_INTERRUPT            __asm(" WFI ")

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

void PORTF_CLK (void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
}

void SysTick_Init(void)
{
    SYSTICK_CTRL_REG         &= ~(1<<0);
    SYSTICK_RELOAD_REG       = 15999999;
    SYSTICK_CURRENT_REG      = 0;
    SYSTICK_CTRL_REG         = 0x00000007;
    NVIC_SYSTEM_PRI3_REG = (NVIC_SYSTEM_PRI3_REG & 0x1FFFFFFF) | ( 1 << 29) ; /* Setup Priority 1 */
    /*
     * Systick Interrupt doesn't need for Enable in NVIC
     * its the only interrupt that has automatic ACK
     * it clears the flag automatically
     * we only need to add priority to the excpetion
     *
     */
}

void PendSV_Init(void)
{
    NVIC_SYSTEM_PRI3_REG = (NVIC_SYSTEM_PRI3_REG & 0xFF1FFFFF) | (7 << 21) ; /* Set the PendSV to the highest priority */
}

int count = 0;

void SysTick_Handler(void)
{
    count ++;
    if ( count == 5)
    {
      SYSTICK_CTRL_REG  &= ~(1<<0);
      SET_BIT(NVIC_SYSTEM_INTCTRL, 28);
      PF1_PF2_PF3 = 0xF;
      Delay_MS(5000);
      count = 0;
    }
}

void PendSV_Handler(void)
{
    PF1_PF2_PF3 = 0x0;
    Delay_MS(5000);
}

int main(void)
{
    PORTF_CLK();
    SysTick_Init();
    Leds_Init();
    while (1)
    {
      PF1_PF2_PF3 = (PF1_PF2_PF3 & 0xF1) | 0x02;
      Delay_MS(1000);
      PF1_PF2_PF3 = (PF1_PF2_PF3 & 0xF1) | 0x04;
      Delay_MS(1000);
      PF1_PF2_PF3 = (PF1_PF2_PF3 & 0xF1) | 0x08;
      Delay_MS(1000);

    }
}
