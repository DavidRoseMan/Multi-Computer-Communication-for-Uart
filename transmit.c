#include "transmit.h"
#include <stdlib.h>
#include "variable.h"
#include <STDARG.H>

void transmit_send(Uart_type *example, unsigned char xdata *arr, \
                   unsigned char sizeofarr, unsigned char addr)
{
    extern void mySend_UART(Uart_type * example, unsigned char Data) reentrant;

    unsigned char xdata i;
    unsigned char xdata head = 0;

    do {
        head = (unsigned char)rand();
    } while (head == example->tail_bak + 2);
    mySend_UART(example, head);
    mySend_UART(example, head + 2);
    mySend_UART(example, head + 4);
    mySend_UART(example, addr);
    mySend_UART(example, ~addr);
    for (i = 0; i < sizeofarr; i++) {
        mySend_UART(example, arr[i]);
        mySend_UART(example, ~arr[i]);
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
    unsigned char xdata i = 0;

    for (i = 0; i < sizeofarr_get; i++) {
        arr_get[i] = arr_receive[2 * i];
    }
}

//--------------------------·¢ËÍ-------------------------------------------------------------

void init_Uart(Uart_type *example, unsigned char xdata *Buffer, unsigned char sizeofbuffer)
{
    example->i            = 0;
    example->end          = 0;
    example->Buffer       = Buffer;
    example->sizeofBuffer = sizeofbuffer;
    example->timer_100us  = 0;
    example->tail_bak     = 0;
}

void mySend_UART(Uart_type *example, unsigned char Data) reentrant
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
    unsigned char xdata a;

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
            if(example->i < example ->sizeofBuffer-1)
                example->i++;
            else
                example->i = 0;
            if (number_uart == 1)
                SBUF = example->Buffer[example->i];
            else if (number_uart == 2)
                S2BUF = example->Buffer[example->i];
            else if (number_uart == 3)
                S3BUF = example->Buffer[example->i];
            else if (number_uart == 4)
                S4BUF = example->Buffer[example->i];
        } else {
            example->timer_100us = 255;
        }
    }

}

void myprintf(Uart_type *uart_example, char *str, ...) reentrant
{
    char *char_temp;
    va_list ap;
    unsigned int i;
    unsigned int number_get;
    unsigned char head_flag; // print start from the number's head_value which is not zero.

    char_temp = str;
    va_start(ap, str);
    while (*char_temp != '\0') {
        if (*char_temp != '%') {
            mySend_UART(uart_example, (unsigned char)*char_temp);
            char_temp++;
        } else {
            char_temp++;
            if (*char_temp == '\0') {
                mySend_UART(uart_example, (unsigned char)'%');
                break;
            } else if (*char_temp != 'd') {
                mySend_UART(uart_example, (unsigned char)'%');
                mySend_UART(uart_example, (unsigned char)*char_temp);
                char_temp++;
                continue;
            }
            number_get = va_arg(ap, unsigned int);
            i          = 10000;
            head_flag  = 1;
            while (i) {
                if (number_get / i != 0) head_flag = 0;
                if (!head_flag)
                    mySend_UART(uart_example, (unsigned char)('0' + number_get / i));
                number_get %= i;
                i /= 10;
            }
            char_temp++;
        }
    }
    va_end(ap);
}

