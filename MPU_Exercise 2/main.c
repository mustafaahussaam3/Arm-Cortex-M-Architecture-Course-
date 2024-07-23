
/*
 * Exercise 1 MPU
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
#define DMB()                         __asm(" DMB ")
#define DSB()                         __asm(" DSB ")
#define ISB()                         __asm(" ISB ")

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

#define MEM_MANAGE_FAULT_PRIORITY_MASK        0xFFFFFF1F
#define MEM_MANAGE_FAULT_PRIORITY                 1
#define MEM_MANAGE_FAULT_PRIORITY_BITS_POS        5
#define MEM_MANAGE_FAULT_ENABLE_BIT_POS           16

#define MPU_ATTR_SIZE_BITS_POS                     1
#define MPU_ATTR_AP_BITS_POS                      24
#define MPU_ATTR_REGION_ENABLE_MASK           0x00000001
#define MPU_ATTR_B_MASK                       0x00010000
#define MPU_ATTR_C_MASK                       0x00020000
#define MPU_ATTR_S_MASK                       0x00040000
#define MPU_BASE_VALID_MASK                   0x00000010
#define MPU_CTRL_ENABLE_MASK                  0x00000001
#define MPU_CTRL_PRIVDEFEN_MASK               0x00000004

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
    NVIC_SYSTEM_PRI3_REG = (NVIC_SYSTEM_PRI3_REG & 0x1FFFFFFF) | ( 3 << 29) ;

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

void MemoryManageFault_Handler(void)
{
    while(1);
}

void MemManage_Fault_Init(void)
{
    /* Assign priority level 1 to the MemManage Fault Exception */
    NVIC_SYSTEM_PRI1_REG = (NVIC_SYSTEM_PRI1_REG & MEM_MANAGE_FAULT_PRIORITY_MASK) | (MEM_MANAGE_FAULT_PRIORITY << MEM_MANAGE_FAULT_PRIORITY_BITS_POS);

    /* Enable the MemManage Fault Exception */
    NVIC_SYSTEM_SYSHNDCTRL |= (1<<MEM_MANAGE_FAULT_ENABLE_BIT_POS);
}

void MPU_Init(void)
{
    MemManage_Fault_Init();
    /* Check if this target supports MPU or not */
        if(MPU_TYPE_REG == 0)
            return;

    DMB();                       /* ensure that memory has finished all its previous operations before programming MPU */
    CLEAR_BIT(MPU_CTRL_REG, 0);  /* Close the MPU to defend corruption and mistakes */
    SET_BIT(MPU_CTRL_REG, 2);    /* set the background bit */
//    SET_BIT  (MPU_CTRL_REG, 1);  /* Enable MPU in Hard Faults, NMI and Fault Mask */

    /********************** Programming Region 0 - Flash memory (256KB) **********************
     * Normal Memory, Cacheable, Not Shareable and Write Through
     * Base Address 0x00000000
     * Region Size 256KB --> SIZE field in the attribute register should be 0x11
     * Privilege has FULL ACCESS (RW) --> AP field in the attribute register should be 0x03
     * Non-Privilege has FULL ACCESS (RW) --> AP field in the attribute register should be 0x03
     ******************************************************************************************/

    MPU_BASE_REG =    0x00000000 | MPU_BASE_VALID_MASK | 0; /* Base Address of Flash Memory with 1 on bit 4 to override region from this position */
    SET_BIT  (MPU_ATTR_REG,   0); /* Enable Region */
    MPU_ATTR_REG |= (0x11 <<  1); /* Set size bit to 17 because flash memory is 256k and this equals log2(256K) - 1 = size */
    SET_BIT  (MPU_ATTR_REG,  17); /* Set the Cashable bit as recommended from data sheet when we program the MPU for flash memory */
    MPU_ATTR_REG |= (0x03 << 24); /* Set the Access to Full access in Priviliged and Unpriviliged mode */

    /*************************** Programming Region 1 - SRAM (32KB) *************************
     * Normal Memory, Cacheable, Shareable and Write Through
     * Base Address 0x20000000
     * Region Size 32KB --> SIZE field in the attribute register should be 0x0E
     * Privilege has FULL ACCESS (RW) --> AP field in the attribute register should be 0x03
     * Non-Privilege has FULL ACCESS (RW) --> AP field in the attribute register should be 0x03
     *****************************************************************************************/

    MPU_BASE_REG = 0x20000000 | MPU_BASE_VALID_MASK | 1 ;
    MPU_ATTR_REG = (0x0E << MPU_ATTR_SIZE_BITS_POS) | (0x03 << MPU_ATTR_AP_BITS_POS) | MPU_ATTR_C_MASK | MPU_ATTR_S_MASK | MPU_ATTR_REGION_ENABLE_MASK ;

    /******************** Programming region 2 - PORTF Memory Map Area **********************
      * Device Memory Sharable and Bufferable
      * Base Address 0x40025000
      * region Size 4k(0x1000) --> SIZE field in the attribute register should be 0x0B
      * Privilege has FULL ACCESS (RW) --> AP field in the attribute register should be 0x03
      * Non-Privilege has FULL ACCESS (RW) --> AP field in the attribute register should be 0x03
      *****************************************************************************************/

    MPU_BASE_REG = 0x40025000 | MPU_BASE_VALID_MASK | 2 ;
    MPU_ATTR_REG = (0x0B << MPU_ATTR_SIZE_BITS_POS) | (0x03 << MPU_ATTR_AP_BITS_POS) | MPU_ATTR_B_MASK | MPU_ATTR_S_MASK | MPU_ATTR_REGION_ENABLE_MASK ;

    SET_BIT(MPU_CTRL_REG , 0); /* Enable the MPU */
    DSB();
    ISB();

}



int main(void)
{
    MPU_Init();
    PORTF_CLK();
    SysTick_Init();
    Leds_Init();
    while (1)
    {
        WAIT_FOR_INTERRUPT;
    }
}
