#include "SysTick.h"
#include "NVIC.h"
#include "tm4c123gh6pm_registers.h"

#define GPIO_PORTF_IRQ_NUM                30
#define GPIO_PORTF_INTERRUPT_PRIORITY     2
#define SYSTICK_INTERRUPT_PRIORITY        1
#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 364

/* Global variable to count time in seconds */
volatile uint8 g_Counter = 0;

void Delay_MS(unsigned long long n)
{
    volatile uint64 count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}

/* GPIO PORTF External Interrupt - ISR */
void GPIOPortF_Handler(void)
{
    SysTick_Stop();
    GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x0E; /* Turn on the Red, Blue and Green LEDs */
    Delay_MS(5000);
    SysTick_Start();
    GPIO_PORTF_ICR_REG   |= (1<<0);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
}

/* Enable PF0 (SW2) and activate external interrupt with falling edge */
void SW2_Init(void)
{
    GPIO_PORTF_LOCK_REG   = 0x4C4F434B;   /* Unlock the GPIO_PORTF_CR_REG */
    GPIO_PORTF_CR_REG    |= (1<<0);       /* Enable changes on PF0 */
    GPIO_PORTF_AMSEL_REG &= ~(1<<0);      /* Disable Analog on PF0 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFFFFF0;   /* Clear PMCx bits for PF0 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   &= ~(1<<0);      /* Configure PF0 as input pin */
    GPIO_PORTF_AFSEL_REG &= ~(1<<0);      /* Disable alternative function on PF0 */
    GPIO_PORTF_PUR_REG   |= (1<<0);       /* Enable pull-up on PF0 */
    GPIO_PORTF_DEN_REG   |= (1<<0);       /* Enable Digital I/O on PF0 */
    GPIO_PORTF_IS_REG    &= ~(1<<0);      /* PF0 detect edges */
    GPIO_PORTF_IBE_REG   &= ~(1<<0);      /* PF0 will detect a certain edge */
    GPIO_PORTF_IEV_REG   &= ~(1<<0);      /* PF0 will detect a falling edge */
    GPIO_PORTF_ICR_REG   |= (1<<0);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
    GPIO_PORTF_IM_REG    |= (1<<0);       /* Enable Interrupt on PF0 pin */

    /* Enable NVIC GPIO PORTF IRQ and set its priority */
    NVIC_EnableIRQ(GPIO_PORTF_IRQ_NUM);
    NVIC_SetPriorityIRQ(GPIO_PORTF_IRQ_NUM,GPIO_PORTF_INTERRUPT_PRIORITY);
}

/* Enable PF1, PF2 and PF3 (RED, Blue and Green LEDs) */
void Leds_Init(void)
{
    GPIO_PORTF_AMSEL_REG &= 0xF1;         /* Disable Analog on PF1, PF2 and PF3 */
    GPIO_PORTF_PCTL_REG  &= 0xFFFF000F;   /* Clear PMCx bits for PF1, PF2 and PF3 to use it as GPIO pin */
    GPIO_PORTF_DIR_REG   |= 0x0E;         /* Configure PF1, PF2 and PF3 as output pin */
    GPIO_PORTF_AFSEL_REG &= 0xF1;         /* Disable alternative function on PF1, PF2 and PF3 */
    GPIO_PORTF_DEN_REG   |= 0x0E;         /* Enable Digital I/O on PF1, PF2 and PF3 */
    GPIO_PORTF_DATA_REG  &= 0xF1;         /* Clear bit 0, 1 and 2 in Data register to turn off the leds */
}

volatile void SysTick_CallBackFunc(void)
{
    g_Counter++;

    switch(g_Counter)
    {
    case 1:
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x02; /* Turn on the Red LED and disable the others */
        break;
    case 2:
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x04; /* Turn on the Blue LED and disable the others */
        break;
    case 3:
        GPIO_PORTF_DATA_REG = (GPIO_PORTF_DATA_REG & 0xF1) | 0x08; /* Turn on the Green LED and disable the others */
        g_Counter = 0;
        break;
    }
}

int main(void)
{
    /* Enable clock for PORTF and wait for clock to start */
    SYSCTL_RCGCGPIO_REG |= 0x20;
    while(!(SYSCTL_PRGPIO_REG & 0x20));

    /* Initialize the SW2(PF0) as GPIO Pin and activate external interrupt with falling edge */
    SW2_Init();

    /* Initialize the LEDs as GPIO Pins */
    Leds_Init();

    /* Start SysTick Timer to generate interrupt every 1 second */
    SysTick_Init(1000);
    NVIC_SetPriorityException(EXCEPTION_SYSTICK_TYPE,SYSTICK_INTERRUPT_PRIORITY);
    SysTick_SetCallBack(SysTick_CallBackFunc);

    /* Enable Interrupts, Exceptions and Faults */
    Enable_Exceptions();
    Enable_Faults();

    while(1)
    {

    }
}
