#include "uart0.h"

int main(void)
{
    uint8 data1[5];
    uint8 data2;
    uint8 Str[20];
    UART0_Init();

    /* Receive 5-bytes from PC Terminal and re-send them again */
    UART0_ReceiveData(data1,5);
    UART0_SendData(data1,5);
//
    /* Receive String from PC Terminal until the'#' character and re-send it again */
    UART0_ReceiveString(Str);
    UART0_SendString(Str);

    while(1)
    {
        /* Receive byte from PC Terminal and re-send it again */
        data2 = UART0_ReceiveByte();
        UART0_SendByte(data2);
    }
}
