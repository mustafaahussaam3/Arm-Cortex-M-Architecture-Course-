# Arm-Cortex-M-Architecture-Course-
Arm Cortex-M Architecture Course is part 1 of Eng Muhammed Tarek Advanced Embedded System Diploma that Covers Arm Cortex Processor, Autosar and RTOS 



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