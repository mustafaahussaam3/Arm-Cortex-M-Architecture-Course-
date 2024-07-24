
/*
 * SVC Exercise 1
 * Eng Muhammed Tarek Course
 *
 * Author: Mustafa Hussam Eldin
 */
#include "tm4c123gh6pm_registers.h"

#define Enable_Exceptions()     __asm(" CPSIE I ")
#define Disable_Exceptions()    __asm(" CPSID I ")
#define Enable_Faults()         __asm(" CPSIE F ")
#define Disable_Faults()        __asm(" CPSIE F ")

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
    SYSTICK_RELOAD_REG       = 15999999;   /* Interrupt Wokring with the Internal Osc System Clk that tick every 1 Second */
    SYSTICK_CURRENT_REG      = 0;
    SYSTICK_CTRL_REG         = 0x00000007; /* Enable the Interrupt bit Note: No need to setup systick in NVIC it automatically acknowledgement */
    NVIC_SYSTEM_PRI3_REG     = 0x40000000; /* Systick Priority 2 010 0 */
}

void PORTF_CLK (void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
}

int i = 1;
void SysTick_Handler (void)
{
    i++;
}

void Enable_Unprivileged(void)
{
   __asm(" MOV r0, #1 ");
   __asm(" MSR CONTROL, r0 ");
}

int main(void)
{
   PORTF_CLK();
   Leds_Init();
   Enable_Exceptions();
   Enable_Faults();
   Enable_Unprivileged ();
   SysTick_Init();
      while (1)
      {
           for (;i<4;)
          {
              GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | (1<<i);
          }
          i = 1;
      }
}
