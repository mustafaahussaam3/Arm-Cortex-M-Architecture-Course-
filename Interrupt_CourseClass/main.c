
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

#define Enable_Exceptions()  __asm(" CPSIE I ")
#define Disable_Exceptions() __asm(" CPSID I ")
#define Enable_Faults()      __asm(" CPSIE F ")
#define Disable_Faults()     __asm(" CPSIE F ")
//#define Wait_Until_Interrupt __asm(" WIF ")
#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364

#define SYSCTL_RCC2_USERCC2_MASK        0x80000000  /* USERCC2 Bit MASK */
#define SYSCTL_RCC2_BYPASS2_MASK        0x00000800  /* PLL BYPASS2 Bit MASK */
#define SYSCTL_RCC_XTAL_MASK            0x000007C0  /* XTAL Bits MASK */
#define SYSCTL_RCC_XTAL_8MHZ            0x0E        /* 8 MHz Crystal Value */
#define SYSCTL_RCC_XTAL_16MHZ           0x15        /* 16 MHz Crystal Value */
#define SYSCTL_RCC_XTAL_BIT_POS         6           /* XTAL Bits Position start from bit number 6 */
#define SYSCTL_RCC2_OSCSRC2_MASK        0x00000070  /* OSCSRC2 Bits MASK */
#define SYSCTL_RCC2_OSCSRC2_MOSC        0x0         /* MOSC(Main Oscillator) value */
#define SYSCTL_RCC2_OSCSRC2_BIT_POS     4           /* OSCSRC2 Bits Position start from bit number 4 */
#define SYSCTL_RCC2_PWRDN2_MASK         0x00002000  /* PWRDN2 Bit MASK */
#define SYSCTL_RCC2_DIV400_MASK         0x40000000  /* DIV400 Bit MASK to Divide PLL as 400 MHz vs. 200 */
#define SYSCTL_RCC2_SYSDIV2_MASK        0x1FC00000  /* SYSDIV2 Bits MASK */
#define SYSCTL_RIS_PLLLRIS_MASK         0x00000040  /* PLLLRIS Bit MASK */
#define SYSCTL_RCC2_SYSDIV2_BIT_POS     22       /* SYSDIV2 Bits Position start from bit number 22 */
#define SYSDIV2_VALUE                   4

void PLL_Init(void)
{
    /* 1) Configure the system to use RCC2 for advanced features
          such as 400 MHz PLL and non-integer System Clock Divisor */
    SYSCTL_RCC2_REG |= SYSCTL_RCC2_USERCC2_MASK;

    /* 2) Bypass PLL while initializing, Don�t use PLL while initialization */
    SYSCTL_RCC2_REG |= SYSCTL_RCC2_BYPASS2_MASK;

    /* 3) Select the crystal value and oscillator source */
    SYSCTL_RCC_REG  &= ~SYSCTL_RCC_XTAL_MASK;     /* clear XTAL field */
    SYSCTL_RCC_REG  |= (SYSCTL_RCC_XTAL_16MHZ << SYSCTL_RCC_XTAL_BIT_POS);  /* Set the XTAL bits for 16 MHz crystal */

    SYSCTL_RCC2_REG &= ~SYSCTL_RCC2_OSCSRC2_MASK; /* clear oscillator source field (OSCSRC2 bits) */
    SYSCTL_RCC2_REG |= (SYSCTL_RCC2_OSCSRC2_MOSC << SYSCTL_RCC2_OSCSRC2_BIT_POS);  /* configure for main oscillator source */

    /* 4) Activate PLL by clearing PWRDN2 */
    SYSCTL_RCC2_REG &= ~SYSCTL_RCC2_PWRDN2_MASK;

    /* 5) Set the desired system divider and the system divider least significant bit */
    SYSCTL_RCC2_REG |= SYSCTL_RCC2_DIV400_MASK;  /* use 400 MHz PLL */

    SYSCTL_RCC2_REG  = (SYSCTL_RCC2_REG & ~SYSCTL_RCC2_SYSDIV2_MASK)        /* clear system clock divider field */
                       | (SYSDIV2_VALUE << SYSCTL_RCC2_SYSDIV2_BIT_POS);      /* configure for 80MHz clock */

    /* 6) Wait for the PLL to lock by polling PLLLRIS bit */
    while(!(SYSCTL_RIS_REG & SYSCTL_RIS_PLLLRIS_MASK));

    /* 7) Enable use of PLL by clearing BYPASS2 */
    SYSCTL_RCC2_REG &= ~SYSCTL_RCC2_BYPASS2_MASK;
}

void Delay_MS(unsigned long long n)
{
    volatile unsigned long long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

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
    GPIO_PORTF_IS_REG        &= ~(1<<0);
    GPIO_PORTF_IBE_REG       &= ~(1<<0);
    GPIO_PORTF_IEV_REG       &= ~(1<<0);
    GPIO_PORTF_IM_REG        |= (1<<0);
//    GPIO_PORTF_RIS_REG

    NVIC_PRI7_REG    = 0x00400000;
    NVIC_EN0_REG     = 0x40000000;
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
    SYSTICK_RELOAD_REG       = 15999999;
    SYSTICK_CURRENT_REG      = 0;
    SYSTICK_CTRL_REG         = 0x00000007;
    NVIC_SYSTEM_PRI3_REG     = 0x40000000;
}

void PortF_Int_Handler(void) //take void return void is handler
{
    GPIO_PORTF_DATA_REG |= (1<<1) | (1<<2) | (1<<3) ;
    Delay_MS(5000);
    GPIO_PORTF_ICR_REG |= (1<<0);

}
int count = 0;
void SysTick_Handler (void)
{
    count++;
}

int main(void)
{
    SYSCTL_RCGC2_REG |= 0x00000020;
    while (!(SYSCTL_PRGPIO_REG & 0x20));


    SW2_Init();
    Led_Red_Init();
    Led_Green_Init();
    Led_Blue_Init();
    Enable_Exceptions();
    Enable_Faults();
    SysTick_Init();
    PLL_Init();

    while (1)
    {
        if (count == 5)
        {
            GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x02;
        }
        else if (count == 10)
        {
            GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x04;
        }
        else if (count == 15 )
        {
            GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x08;
            count = 0;
        }
    }
    return 0;
}