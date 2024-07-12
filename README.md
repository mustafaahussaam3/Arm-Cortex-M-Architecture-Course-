# Arm-Cortex-M-Architecture-Course-
Arm Cortex-M Architecture Course is part 1 of Eng Muhammed Tarek Advanced Embedded System Diploma that Covers Arm Cortex Processor, Autosar and RTOS 



## SVC System Exceptions
# Exerise 1 
- We here need to use the RGB leds in our TIVA-C LaunchPad to try to make an Fault Exeption while calling the Systick Handler in the 
Unpriviliged Mode. 
- As Exercise 2 in SysTick Interrupt we need to make a 1 second Delay using Systick interrupt but in Unprivileged Mode that will cause the processor to give Hard Fault Exception and Calling Fault_ISR 

#How to make the processor go to Unprivileged Mode 
- This will done through 0 bit in the ![Control Register](<Images/Control Register (Processor Specific).PNG>) inside the prcocessor that is one of the Cortex Special Registers and it must reached in Privileged Mode only. (and the challenge is switching the processor to Unprivileged then go back to privileged throught SVC)