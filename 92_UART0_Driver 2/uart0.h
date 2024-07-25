 /******************************************************************************
 *
 * Module: UART0
 *
 * File Name: uart0.h
 *
 * Description: Header file for the TM4C123GH6PM UART0 driver
 *
 * Author: Edges for Training Team
 *
 *******************************************************************************/

#ifndef UART0_H_
#define UART0_H_

#include "std_types.h"
#include "tm4c123gh6pm_registers.h"


/*******************************************************************************
 *                             PreProcessor Macros                             *
 *******************************************************************************/

#define UART_DATA_5BITS          0x0
#define UART_DATA_6BITS          0x1
#define UART_DATA_7BITS          0x2
#define UART_DATA_8BITS          0x3
#define UART_LCRH_WLEN_BITS_POS  5
#define UART_CTL_UARTEN_MASK     0x00000001
#define UART_CTL_TXE_MASK        0x00000100
#define UART_CTL_RXE_MASK        0x00000200
#define UART_FR_TXFE_MASK        0x00000080
#define UART_FR_RXFE_MASK        0x00000010
#define BIT_IS_SET(REG, BIT_NUM)     (REG &  (1<<BIT_NUM))
#define BIT_IS_CLEAR(REG, BIT_NUM) (!(REG &  (1<<BIT_NUM)))

/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/

extern void UART0_Init(void);

extern void UART0_SendByte(uint8 data);

extern uint8 UART0_ReceiveByte(void);

extern void UART0_SendData(const uint8 *pData, uint32 uSize);

extern void UART0_ReceiveData(uint8 *pData, uint32 uSize);

extern void UART0_SendString(const uint8 *pData);

extern void UART0_ReceiveString(uint8 *pData);

#endif
