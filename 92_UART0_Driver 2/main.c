#include "uart0.h"

#define PF1_PF2_PF3                  (*((volatile uint32*)(0x40025000 + 0x038)))

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

int main(void)
{
    uint8 data2;
    PORTF_CLK();
    Leds_Init();
    UART0_Init();

    while(1)
    {
        /* Receive byte from PC Terminal and re-send it again */
        data2 = UART0_ReceiveByte();
        switch (data2)
        {
        case '0': PF1_PF2_PF3 = 0x00;
            break;
        case '1': PF1_PF2_PF3 = 0x02;
            break;
        case '2': PF1_PF2_PF3 = 0x04;
             break;
        case '3': PF1_PF2_PF3 = 0x06;
             break;
        case '4': PF1_PF2_PF3 = 0x08;
             break;
        case '5': PF1_PF2_PF3 = 0x0A;
             break;
        case '6': PF1_PF2_PF3 = 0x0C;
             break;
        case '7': PF1_PF2_PF3 = 0x0E;
             break;
        }

    }
}
