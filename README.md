# Arm-Cortex-M-Architecture-Course-
Arm Cortex-M Architecture Course is part 1 of Eng Muhammed Tarek Advanced Embedded System Diploma that Covers Arm Cortex Processor, Autosar and RTOS 
# PLL
## ![Exercise 1](<PLL_Exercise1/main.c>)
### How to Program PLL
- Phase-Lock-Loop(PLL) regsters are part from the System Control Module, it can be used to speed up or slow down the processor clock. Choice of Frequency is a tradeoff between software execution speed and electrical power(and heat). 
- it derived from Internal or Main 16MHz oscillator.
- PLL get up to 400 MHz but our TivaC can only gives 80MHz maximum frequency.
- to program PLL we need these 3 registers:
1. SYSCTL_RCC2_REG ![image](<Images/Run-Mode Clock Configuration 2 (System Control Module).PNG>)
2. SYSCTL_RCC1_REG
3. SYSCTL_RIS_REG
- Steps to program PLL: 
1. Set USERCC2 in RCC2 Register to have Pll options like 16MHz.
2. Close the PLL until it ready by Set BYPASS2 that will allow OSC to pass to the system.
3. Configure XTAL with main osc frequency in RCC by 0x15(16MHz).
4. Clear PWRDN2 in RCC2 to activate PLL.
5. Set USESYSDIV to Activate DIV400 when both USERCC2 & USESYSDIV are enabled.
6. Configure Frequency through DIV400 (set: 400, Clear: 200).
7. Configure System Divison.
8. Wait for PLL to be ready.
9. Clear BYPASS2 to be used as System clk on 80MHz



# SVC System Exceptions
## ![Exerise 1](<SVC_Exercise 1/main.c>)
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

## ![Exercise 2](<SVC_Exercise 2/main.c>) 

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

## ![Exercise 3](<SVC_Exercise 3/main.c>)
### Selecting Different SVC Functions