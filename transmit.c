#include "transmit.h"
#include <stdlib.h>
#include "variable.h"

void transmit_send(Uart_type *example, unsigned char xdata *arr, \
                   unsigned char sizeofarr, unsigned char addr)
{
    extern void Send_UART(Uart_type * example, unsigned char Data);

    unsigned char i;
    unsigned char head = 0;

    do {
        head = (unsigned char)rand();
    } while (head == example->tail_bak + 2);
    Send_UART(example, head);
    Send_UART(example, head + 2);
    Send_UART(example, head + 4);
    Send_UART(example, addr);
    Send_UART(example, ~addr);
    for (i = 0; i < sizeofarr; i++) {
        Send_UART(example, arr[i]);
        Send_UART(example, ~arr[i]);
        if (i == sizeofarr - 1) {
            example->tail_bak = ~arr[i];
        }
    }
}

bit transmit_receive(unsigned char xdata *arr_receive, unsigned char sizeofarr_receive, \
                     unsigned char addr, unsigned char Buffer_Receive_Uart)
{
    static unsigned char xdata arr_datahead_bak[3] = {0};
    static unsigned char xdata datahead0 = 0, datahead1 = 0;
    static unsigned char xdata ptt = 0xff; // point to tail

    arr_datahead_bak[0] = arr_datahead_bak[1];
    arr_datahead_bak[1] = arr_datahead_bak[2];
    arr_datahead_bak[2] = Buffer_Receive_Uart;
    //----------------------------------------------------------------
    if (ptt == 1) {
        datahead0 = Buffer_Receive_Uart;
    } else if (ptt == 2) {
        datahead1 = Buffer_Receive_Uart;
        if (datahead0 != addr || datahead0 != ~datahead1) {
            ptt = 0xff;
        }
    }
    if (ptt >= 3 && ptt <= (sizeofarr_receive + 2)) {
        arr_receive[ptt - 3] = Buffer_Receive_Uart;
        if (ptt % 2 == 0) {
            if (arr_receive[ptt - 3] != ~arr_receive[ptt - 4]) ptt = 0xff;
        }
    }
    if (ptt == (sizeofarr_receive + 2)) {
        ptt = 0xff;
        return receive_finish;
    }
    //-----------------------------------------------------------------
    if ((arr_datahead_bak[0] + 2 == arr_datahead_bak[1]) &&
        (arr_datahead_bak[1] + 2 == arr_datahead_bak[2])) {
        ptt = 0;
    }
    if (ptt != 0xff) ptt++;

    return receive_notfinish;
}

void transmit_get(unsigned char xdata *arr_receive, unsigned char xdata *arr_get, unsigned char sizeofarr_get)
{
    unsigned char i = 0;

    for (i = 0; i < sizeofarr_get; i++) {
        arr_get[i] = arr_receive[2 * i];
    }
}

//--------------------------����-------------------------------------------------------------

void init_Uart(Uart_type *example, unsigned char *Buffer, unsigned char sizeofbuffer)
{
    example->i            = 0;
    example->end          = 0;
    example->Buffer       = Buffer;
    example->sizeofBuffer = sizeofbuffer;
    example->timer_100us  = 0;
    example->tail_bak     = 0;
}

void Send_UART(Uart_type *example, unsigned char Data)
{
    unsigned char i;

    i = example->end;
    if (i >= example->i) {
        if (i != example->sizeofBuffer - 1)
            i++;
        else {
            while (example->i == 0)
                ;
            i = 0;
        }
    } else {
        {
            while (example->i == 1)
                ;
            i++;
        }
    }
    example->Buffer[i] = Data;
    example->end       = i;
}

void UART_Process_Send(Uart_type *example, unsigned char number_uart)
{
    if (example->timer_100us != 255)
        example->timer_100us++;
    else if (example->i != example->end) {
        if (number_uart == 1)
            SCON |= 0x02;
        else if (number_uart == 2)
            S2CON |= 0x02;
        else if (number_uart == 3)
            S3CON |= 0x02;
        else if (number_uart == 4)
            S4CON |= 0x02;
    }
}

void UART_Process_Send_ITR(Uart_type *example, unsigned char number_uart)
{
    unsigned char a;

    if (number_uart == 1)
        a = SCON;
    else if (number_uart == 2)
        a = S2CON;
    else if (number_uart == 3)
        a = S3CON;
    else if (number_uart == 4)
        a = S4CON;

    if (a & 0x02) {
        if (number_uart == 1)
            SCON &= ~0x02;
        else if (number_uart == 2)
            S2CON &= ~0x02;
        else if (number_uart == 3)
            S3CON &= ~0x02;
        else if (number_uart == 4)
            S4CON &= ~0x02;
        example->timer_100us = 0;
        if (example->i != example->end) {
            example->i++;
            if (number_uart == 1)
                SBUF = example->Buffer[example->i];
            else if (number_uart == 1)
                S2BUF = example->Buffer[example->i];
            else if (number_uart == 1)
                S3BUF = example->Buffer[example->i];
            else if (number_uart == 1)
                S4BUF = example->Buffer[example->i];
        } else {
            example->timer_100us = 255;
        }
    }
}