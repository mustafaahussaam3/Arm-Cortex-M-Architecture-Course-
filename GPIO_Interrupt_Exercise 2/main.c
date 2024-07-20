
/*
 * Exercise 2 GPIO Interrupt
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
#define BIT_IS_CLEAR(REG, BIT_NUM)    (!(REG &  (1<<BIT_NUM)))
#define PF1_PF2_PF3                   (*((volatile uint32*)(0x40025000 + 0x038)))
#define PF0_PF4                       (*((volatile uint32*)(0x40025000 + 0x044)))
#define WAIT_FOR_INTERRUPT            (__asm(" WFI "))

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

void PORTF_CLK (void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
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

//PF4
void SW1_Init(void)
{
    GPIO_PORTF_AMSEL_REG     &= ~(1<<4);
    GPIO_PORTF_PCTL_REG      &= 0xFFF0FFFF;
    GPIO_PORTF_AFSEL_REG     &= ~(1<<4);
    GPIO_PORTF_DIR_REG       &= ~(1<<4);
    GPIO_PORTF_PUR_REG       |= (1<<4);
    GPIO_PORTF_DEN_REG       |= (1<<4);
    CLEAR_BIT(GPIO_PORTF_IS_REG,  4); /* Set: Sense levels, Clear: Sense Edge */
    CLEAR_BIT(GPIO_PORTF_IBE_REG, 4); /* Set: Both Edges,   Clear: Only Edge */
    CLEAR_BIT(GPIO_PORTF_IEV_REG, 4); /* Set: Raising Edge, Clear: Falling Edge*/
    SET_BIT(GPIO_PORTF_IM_REG,    4); /* Allow Interrupt to be send to NVIC */

    NVIC_PRI7_REG = ( NVIC_PRI7_REG & 0xFF1FFFFF) | (0x2 << 21); /* Set priority 2 ) */
    SET_BIT(NVIC_EN0_REG,       30); /* Enable PORTF NVIC */

    //GPIO_PORTF_RIS_REG
}

int i;

void PORTF_Handler(void)
{
    SET_BIT(GPIO_PORTF_ICR_REG,  4); /* Clear Trigger flag for PF4 */
    for ( i =1; i<4; i++)
         {
             PF1_PF2_PF3 = (PF1_PF2_PF3 & 0x00) | (1<<i);
             Delay_MS(5000);
         }
}



int main(void)
{
    Enable_Exceptions();
    Enable_Faults();
    PORTF_CLK();
    Leds_Init();
    SW1_Init();
    while(1)
    {
        for ( i =1; i<4; i++)
          {
              PF1_PF2_PF3 = (PF1_PF2_PF3 & 0x00) | (1<<i);
              Delay_MS(1000);
          }
    }
}
