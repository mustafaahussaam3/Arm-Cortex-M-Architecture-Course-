
/*
 * Exercise 2 PLL Operate Pll at 10MHz
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
#define USERCC2                       31
#define DIV400                        30
#define PWRDN2                        13
#define BYPASS2                       11
#define USESYSDIV                     22
#define XTAL_FREQ                     0x15  /*16MHz*/
#define OSCSRC2_MASK                  0x0  /*Main Osc*/
#define XTAL_BIT                      6
#define XTAL_MASK                     0xFFFFF83F
#define SYSDIV2_MASK                  0xD03FFFFF
#define SYSDIV2_BIT                   22
#define SYSDIV2_N                     0x27    /*400/10 = 40 So, N = 39 */
#define PLLRIS                        6
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
    SYSTICK_RELOAD_REG       = 9999999;
    SYSTICK_CURRENT_REG      = 0;
    SYSTICK_CTRL_REG         = 0x00000005;
}

void PLL_Init(void)
{
    SET_BIT(SYSCTL_RCC2_REG, USERCC2);                                           /*Set USERCC2 in RCC2 Register to have Pll options like 16MHz*/
    SET_BIT(SYSCTL_RCC2_REG, BYPASS2);                                           /*Close the PLL until it ready by Set BYPASS2 that will allow OSC to pass to the system*/
    SYSCTL_RCC_REG = (SYSCTL_RCC_REG & XTAL_MASK) | (XTAL_FREQ << XTAL_BIT);     /*Configure XTAL with main osc frequency in RCC by 0x15(16MHz)*/
    CLEAR_BIT(SYSCTL_RCC2_REG,PWRDN2);                                           /*Clear PWRDN2 in RCC2 to activate PLL*/
    SET_BIT(SYSCTL_RCC_REG, USESYSDIV);                                          /* Set USESYSDIV to Activate DIV400 when both USERCC2 & USESYSDIV are enabled*/
    SET_BIT(SYSCTL_RCC2_REG, DIV400);                                            /*Configure Frequency through DIV400 (set: 400, Clear: 200) */
    SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & SYSDIV2_MASK) | (SYSDIV2_N << SYSDIV2_BIT);  /* Configure System Divison */
    while (BIT_IS_CLEAR(SYSCTL_RIS_REG, PLLRIS));                                     /*Wait for PLL to be ready*/
    CLEAR_BIT(SYSCTL_RCC2_REG, BYPASS2);                                              /*Clear BYPASS2 to be used as System clk on 80MHz*/
}

int main(void)
{
    PORTF_CLK();
    SysTick_Init();
    Leds_Init();
    PLL_Init();
    int i = 0;
    while (1)
    {
        for (i = 0; i < 4; i++)
        {
          SET_BIT(PF1_PF2_PF3, i);
          while(BIT_IS_CLEAR(SYSTICK_CTRL_REG, 16));
          CLEAR_BIT(PF1_PF2_PF3,i);
        }
    }
}
