#include "transmit.h"
#include <stdlib.h>

void transmit_send(unsigned char xdata *arr, unsigned char sizeofarr, unsigned char addr)
{
    extern void Send_UART(unsigned char a);

    static unsigned char i;
    static unsigned char head     = 0;
    static unsigned char tail_bak = 0;

    do {
        head = (unsigned char)rand();
    } while (head == tail_bak);
    Send_UART(head);
    Send_UART(head + 1);
    Send_UART(head + 2);
    Send_UART(addr);
    Send_UART(~addr);
    for (i = 0; i < sizeofarr; i++) {
        Send_UART(arr[i]);
        Send_UART(~arr[i]);
        if (i == sizeofarr - 1) {
            tail_bak = ~arr[i];
        }
    }
}

bit transmit_receive(unsigned char xdata *arr_receive, unsigned char sizeofarr_receive, unsigned char addr, unsigned char Buffer_Receive_Uart)
{
    static unsigned char xdata datahead_bak0 = 0, datahead_bak1 = 0, datahead_bak2 = 0;
    static unsigned char xdata datahead0 = 0, datahead1 = 0;
    static unsigned char xdata ptt = 0;//point to tail

    datahead_bak0 = datahead_bak1;
    datahead_bak1 = datahead_bak2;
    datahead_bak2 = Buffer_Receive_Uart;
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
    if ((datahead_bak0 + 1 == datahead_bak1) && (datahead_bak1 + 1 == datahead_bak2)) {
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