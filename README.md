# Arm-Cortex-M-Architecture-Course-
Arm Cortex-M Architecture Course is part 1 of Eng [Mohamed Tarek](https://www.linkedin.com/in/mohamed-tarek-2237a457/) Advanced Embedded System Diploma that Covers Arm Cortex Processor, Autosar and RTOS 

# PLL

## ![Exercise 1](<3_PLL_Exercise1/main.c>) Phase Lock Loop

We need to operate the SysTick Timer with 80Mhz

### How to Program PLL

- Phase-Lock-Loop(PLL) registers are part from the System Control Module, it can be used to speed up or slow down the processor clock. Choice of Frequency is a tradeoff between software execution speed and electrical power(and heat). 
- It derived from Internal or Main 16MHz oscillator.
- PLL get up to 400 MHz but our TivaC can only gives 80MHz maximum frequency.
- to program PLL we need these 3 registers:
1. SYSCTL_RCC2_REG ![image](<Images/Run-Mode Clock Configuration 2 (System Control Module).PNG>)

2. SYSCTL_RCC1_REG ![image](<Images/Run-Mode Clock Configuration (System Control Module).PNG>)

3. SYSCTL_RIS_REG  ![image](<Images/Raw Interrupt Status Register (System Control Module).PNG>)
- Steps to program PLL: 
1. Set USERCC2 in RCC2 Register to have PLL Options.
```bash 
#define USERCC2                       31
 SET_BIT(SYSCTL_RCC2_REG, USERCC2);   
 ```
2. Close the PLL until it ready by Set BYPASS2 that will allow OSC to pass to the system.
```bash 
#define BYPASS2                       11
SET_BIT(SYSCTL_RCC2_REG, BYPASS2);
```
3. Configure XTAL with main OSC frequency in RCC by 0x15(16MHz).
```bash 
#define XTAL_FREQ                     0x15  /*16MHz*/
#define XTAL_BIT                      6
#define XTAL_MASK                     0xFFFFF83F
SYSCTL_RCC_REG = (SYSCTL_RCC_REG & XTAL_MASK) | (XTAL_FREQ << XTAL_BIT);
```
 ![image](<Images/XTAL Frequencies.PNG>)

4. Configure OSCSRC2 to the main Oscillator.
```bash 
#define OSCSRC2_MASK                  0xFFFFFF8F
#define OSCSRC2_BIT                   4
#define OSCSRC2_VALUE                 0x0
SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & OSCSRC2_MASK) | (OSCSRC2_VALUE << OSCSRC2_BIT); 
```
 ![image](<Images/OSCRC2 Selection.PNG>)

5. Clear PWRDN2 in RCC2 to activate PLL.
```bash 
#define PWRDN2                        13
 CLEAR_BIT(SYSCTL_RCC2_REG,PWRDN2);
```
6. Set USESYSDIV to Activate DIV400 when both USERCC2 & USESYSDIV are enabled.
```bash 
#define USESYSDIV                     22
SET_BIT(SYSCTL_RCC_REG, USESYSDIV);
```
7. Configure Frequency through DIV400 (set: 400, Clear: 200).
```bash 
#define DIV400                        30
 SET_BIT(SYSCTL_RCC2_REG, DIV400); 
```
8. Configure System Divison.
```bash 
#define SYSDIV2_MASK                  0xD03FFFFF
#define SYSDIV2_BIT                   22
#define SYSDIV2_N                     0x4    /*400/80 = 5 So, N = 4 */
SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & SYSDIV2_MASK) | (SYSDIV2_N << SYSDIV2_BIT); 
```
9. Wait for PLL to be ready.
```bash 
#define PLLRIS                        6
while (BIT_IS_CLEAR(SYSCTL_RIS_REG, PLLRIS));        
```
10. Clear BYPASS2 to be used as System clk on 80MHz
```bash 
CLEAR_BIT(SYSCTL_RCC2_REG, BYPASS2);  
```

## ![Exercise 2](<PLL_Exercise2/main.c>) Phase Lock Loop
We need to operate PLL on 10MHz

- We will change in the divisor bit only
```bash
#define SYSDIV2_N                     0x27    /*400/10 = 40 So, N = 39 */
SYSCTL_RCC2_REG = (SYSCTL_RCC2_REG & SYSDIV2_MASK) | (SYSDIV2_N << SYSDIV2_BIT); 
```

# SVC System Exceptions
## ![Exerise 1](<6_SVC_Exercise 1/main.c>) SV Call

We here need to use the RGB leds in our TIVA-C LaunchPad trying to make a Fault Exeption while calling the Systick Handler in the 
Unpriviliged Mode. 
As Exercise 2 (SysTick Interrupt), we need to make a 1 second Delay using Systick interrupt but in Unprivileged Mode that will cause the processor to give Hard Fault Exception and Calling Fault_ISR .

### How to make the processor go to Unprivileged Mode:

- This will done through 0 bit in the Control Register  ![Control Register](<Images/Control Register (Processor Specific).PNG>) inside the prcocessor that is one of the Cortex Special Registers and it must reached in Privileged Mode only. (and the challenge is switching the processor to Unprivileged then go back to privileged throught SVC).
- To reach a processor specific registers you reached only through assembly instruction so, we will writing a function with inline assembly (__asm), that will MOV 1 to r0 processor specific register and then load (LDR) this value in the control register.
```bash
void Enable_Unprivileged(void)
{
   __asm(" MOV r0, #1 ");
   __asm(" MSR CONTROL, r0 ");
}
```
- Add this function to your code before the Systick_Init function and then run the code.
- You will got a Fault Exception because of trying to reach a processor peripheral in unprivileged(user mode).
- Now, How can we go back to privileged mode ? we will answer this in exercise 2.

## ![Exercise 2](<6_SVC_Exercise 2/main.c>) SV Call

### How to switch back to privileged Mode:
Now, the challenge is to switch back to privileged Mode through Software Interrupt or In Cortex-M It's call SVC (Supervisor Call), the processor will be switched back to privileged mode inside the handler and then we can adding the instructions to Reset bit 0 in the Control Regiter.
```bash 
void SVC_Handler (void)
{
    __asm(" MOV r0, #0 ");
    __asm(" MSR CONTROL, r0");
}
```
- Also, we need to add this macro to call SVC in the unprivileged mode, triggering the processor to call the SVC_Handler
```
#define Trigger_SVC_Exception   __asm(" SVC #0 ")
```
- We will call this exception before calling the SysTick, then the processor will switching back to the privileged mode and the Hard Fault will stop.

- Now, we need to use the SVC parameter #0 to select different SVC Functions
- This parameter is 8-bits that it can make 256 different SVC Call.

## ![Exercise 3](<6_SVC_Exercise 3/main.c>)
### Selecting Different SVC Functions

# PendSV System Exception 
## ![Exercise 1](<7_PendSV_Exercise 1/main.c>) PendSV

- PendSV is one of the essentials Exceptions that used in OS design.
- it's the lowest priority in system exception.
- it's used to make some tasks pendable due to executing higher priority tasks.
- it has two usage:
    1. To not make context switch while the Systick Interrupt executing over IRQ, because this will make the schedular make a context switch inside the Systick Handler and pend the IRQ which cause a Usage Fault Exception. ( we will write the context switch code inside the PendSV Handler)
    2. To Split tasks to two rigions when the code has too much irq's which will cause a delay in important tasks. (so, we will put the time critical tasks in Systick and the remain of the task in pendSV)
- To trigger PendSV we use:
    1. INTCTRL Register to enable the interrupt that will put it under the category of the Software Interrupt.
    Set (Bit 28) will cause pending interrupt ![INTCTRL](<Images/Interrupt Control and State (INTCTRL).PNG>)
    2. Adjust the priority of the exception to be the lowest priority ( 7 ). ![System Priority](<Images/System Handler Priority 3.PNG>)

# Fault Exceptions

There are 4 types of Fault System Exceptions:
1. Hard Fault. 
2. Bus Fault.
3. Usage Fault.
4. Memory Management Fault

## Hard Fault Exception 

- Has fixed priority level ( -1 ) after Reset, NMI.
- All faults trigger hard fault exceptions if we disabled the other types of fault.
- FaultMask can mask the hard fault exceptions.
- PriMask will trigger fault exeptions even if the memManage, Bus and Usage is enabled.

## Bus Fault Exception

it occurs when: 
1. Accessing peripheral without enable it's clock.
2. accessing reserved memory region.

## ![Exercise 1](<8_BusFault_Exercise 1/main.c>) Bus Fault 

We will use GPIO Exercise 1 without enabling clock to trigger a Hard Fault Exception then, we will use the same code but by activating the bus fault registers to trigger bus fault instead of hard fault exceptions.
 
- After running code we will stuck in the Fault_ISR Exception 
![fault](<Images/Hard Fault Exception.PNG>)

## ![Exercise 2](<8_BusFault_Exercise 2/main.c>) Bus Fault

Use the specified register to enable the Bus Fault and set priority exception. 
- We need only 3 registers 
1. Enable the Bus Fault Excpetion from System Handler Control and State in System Control Block. (Enable Bit 17)
![SYSHNDCTRL](<Images/System Handler Control and State (SYSHNDCTRL).PNG>)

2. Set Exception Priority from ( 0 to 7 ).
![](<Images/System Handler Priority 1.PNG>)

3. After running code you will find the value of the desired register in bus fault address register. 
![](<Images/Fault Address Register.PNG>)

## ![Exercise 3](<8_BusFault_Exercise 3/main.c>)

Here we will trying to access reserved memory area which will cause Bus Fault in case we enable it, and it will cause hard fault by default
```bash
 volatile uint32 *ptr =  (unsigned long*)0x20008000 ;
    *ptr = 20;
```
## ![Exercise 4](<8_BusFault_Exercise 4/main.c>)
Use same code of exercise 3 but enable the bus fault exception.

# Usage Fault 

it occurs due to:
1. Undefined instrution.
2. Unaligned memory access for load/ store multiple.
3. Divide by zero.
4. Switching from IRQ to task without finishing the IRQ.

## ![Exercise 1](<9_UsageFault_Exercise 1/main.c>)
We will use the same registers as Bus Fault Exception.
- We will implement function that divide the domin by zero. 
- A Hard Fault will be triggered if we Enable the ( Trap on Divide by 0 ) in Configuration and Control register. ( because arm by default gives zero if we divide bu zero ).
- Open the trap by Enabling Div0 bit in CFGCTRL Register. 
![](<Images/Configuration and Control Register ( CFGCTRL).PNG>)
## ![Exercise 2](<9_UsageFault_Exercise 2/main.c>)
Same as Exericse 1 but we will enable the Usage Fault Exception.
```bash 
void UsageFault_Init(void)
{
    SET_BIT(NVIC_SYSTEM_SYSHNDCTRL, 18);
    NVIC_SYSTEM_PRI1_REG = ( NVIC_SYSTEM_PRI1_REG & 0xFF1FFFFF )
     | (2 << 13);
    SET_BIT(NVIC_SYSTEM_CFGCTRL, DIV0);
}
```
## ![Exercise 3](<9_UsageFault_Exercise 3/main.c>)
Enable the UNALIGNED Bit in CFGCTRL and make unaligned memory access.
- It will trigger Hard Fault.
## ![Exercise 4](<9_UsageFault_Exercise 4/main.c>)
Same as Exercise 3 but initalize the Usage fault to be triggered.

# Contact
For any questions or inquiries, please contact [Mustafa Hussam Eldin](https://www.linkedin.com/in/mustafahussameldin/).