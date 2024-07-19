
/*
 * Exercise 1 GPIO Interrupt
 *
 * Eng Mohammed Tarek Course
 *
 * Advanced Embedded System
 *
 * Author: Mustafa Hussam Eldin
 */

#include "tm4c123gh6pm_registers.h"


#define SET_BIT(REG, BIT_NUM)         (REG |= (1<<BIT_NUM))
#define CLEAR_BIT(REG, BIT_NUM)       (REG &=~(1<<BIT_NUM))
#define TOGGLE_BIT(REG, BIT_NUM)      (REG ^= (1<<BIT_NUM))
#define BIT_IS_SET(REG, BIT_NUM)      (REG &  (1<<BIT_NUM))
#define BIT_IS_CLEAR(REG, BIT_NUM)    (!(REG &  (1<<BIT_NUM)))
#define PF1_PF2_PF3                   (*((volatile uint32*)(0x40025000 + 0x038)))
#define PF0_PF4                       (*((volatile uint32*)(0x40025000 + 0x044)))

void PORTF_CLK (void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));
}

//PF2
void Blue_Led_Init(void)
{
    GPIO_PORTF_AMSEL_REG    &= ~(1<<2);
    GPIO_PORTF_PCTL_REG     &= 0xFFFFF0FF;
    GPIO_PORTF_AFSEL_REG    &= ~(1<<2);
    GPIO_PORTF_DIR_REG      |= (1<<2);
    GPIO_PORTF_DEN_REG      |= (1<<2);
    GPIO_PORTF_DATA_REG     &= ~(1<<2);
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
    CLEAR_BIT(GPIO_PORTF_IS_REG,  4); /* Set: sense levels, Clear*/
    CLEAR_BIT(GPIO_PORTF_IBE_REG, 4);
    CLEAR_BIT(GPIO_PORTF_IS_REG, 4);
    CLEAR_BIT(GPIO_PORTF_IS_REG, 4);

    GPIO_PORTF_IEV_REG
    GPIO_PORTF_IM_REG
    GPIO_PORTF_RIS_REG
    GPIO_PORTF_ICR_REG
}

void PORTF_Handler(void)
{

}
int main(void)
{
    PORTF_CLK();
    Blue_Led_Init();
    SW1_Init();

    while(1)
    {

    }


}
