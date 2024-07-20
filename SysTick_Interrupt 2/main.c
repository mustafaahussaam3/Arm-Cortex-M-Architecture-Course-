
/*
 * Exercise 2 SysTick Interrupt 2
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

#define USERCC2                       31
#define DIV400                        30
#define PWRDN2                        13
#define BYPASS2                       11
#define USESYSDIV                     22
#define XTAL_FREQ                     0x15  /*16MHz*/
#define XTAL_BIT                      6
#define XTAL_MASK                     0xFFFFF83F
#define SYSDIV2_MASK                  0xD03FFFFF
#define SYSDIV2_BIT                   22
#define SYSDIV2_N                     0x4    /*400/80 = 5 So, N = 4 */
#define PLLRIS                        6
#define OSCSRC2_MASK                  0xFFFFFF8F
#define OSCSRC2_BIT                   4
#define OSCSRC2_VALUE                 0x0

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

void PLL_Init(void)
{
    SET_BIT(SYSCTL_RCC2_REG, USERCC2);                                                    /*Set USERCC2 in RCC2 Register to have Pll options */
    SET_BIT(SYSCTL_RCC2_REG, BYPASS2);                                                    /*Close the PLL until it ready by Set BYPASS2 that will allow OSC to pass to the system*/
    SYSCTL_RCC_REG = (SYSCTL_RCC_REG & XTAL_MASK) | (XTAL_FREQ << XTAL_BIT);              /*Configure XTAL with main osc frequency in RCC by 0x15(16MHz)*/
    SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & OSCSRC2_MASK) | (OSCSRC2_VALUE << OSCSRC2_BIT);  /*Configure OSCSRC2 to the main Oscillator*/
    CLEAR_BIT(SYSCTL_RCC2_REG,PWRDN2);                                                    /*Clear PWRDN2 in RCC2 to activate PLL*/
    SET_BIT(SYSCTL_RCC_REG, USESYSDIV);                                                   /* Set USESYSDIV to Activate DIV400 when both USERCC2 & USESYSDIV are enabled*/
    SET_BIT(SYSCTL_RCC2_REG, DIV400);                                                     /*Configure Frequency through DIV400 (set: 400, Clear: 200) */
    SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & SYSDIV2_MASK) | (SYSDIV2_N << SYSDIV2_BIT);      /* Configure System Divison */
    while (BIT_IS_CLEAR(SYSCTL_RIS_REG, PLLRIS));                                         /*Wait for PLL to be ready*/
    CLEAR_BIT(SYSCTL_RCC2_REG, BYPASS2);                                                  /*Clear BYPASS2 to be used as System clk on 80MHz*/
}

int count = 0;

void SysTick_Handler(void)
{
    count ++;

    if ( count == 5 )
    {
        PF1_PF2_PF3 = (PF1_PF2_PF3 & 0xF1) | 0x02;
    }
    else if ( count == 10 )
    {
        PF1_PF2_PF3 = (PF1_PF2_PF3 & 0xF1) | 0x04;
    }
    else if ( count == 15 )
    {
        PF1_PF2_PF3 = (PF1_PF2_PF3 & 0xF1) | 0x08;
        count = 0 ;
    }

}

int main(void)
{
    PORTF_CLK();
    SysTick_Init();
    PLL_Init();
    Leds_Init();
    while (1)
    {
        WAIT_FOR_INTERRUPT;
    }
}
