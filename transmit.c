#include "transmit.h"
#include <stdio.h>

void transmit_send(unsigned char xdata *arr, unsigned char sizeofarr, unsigned char addr)
{
    extern send_Uart(unsigned char x);
    static unsigned char i;
    static unsigned char head     = 0;
    static unsigned char tail_bak = 0;

    do {
        head = (unsigned char)rand();
    } while (head == tail_bak);
    send_Uart(head);
    send_Uart(head + 1);
    send_Uart(head + 2);
    for (i = 0; i < sizeofarr; i++) {
        send_Uart(arr[i]);
        send_Uart(~arr[i]);
        if (i == sizeofarr - 1) {
            tail_bak = ~arr[i];
        }
    }
}

bit transmit_receive(unsigned char xdata *arr, unsigned char sizeofarr, unsigned char addr)
{
    static unsigned char xdata data_head[5] = {0};
    static unsigned char xdata p_head = 0, p_data = 0;

    if (p_head < 5) {
        data_head[p_head] = Buffer_Receive_Uart;
        p_head++;
        if (p_head >= 2 && p_head <= 3) {
            if (data_head[p_head - 2] + 1 != data_head[p_head - 1]) p_head = 0;
        } else if (p_head == 5) {
            if ((data_head[3] != ~data_head[4]) || data_head[3] != addr) {
                p_head = 0;
            }
        }
    }
    if (p_head == 5) {
        if (p_data < sizeofarr) {
            arr[p_data] = Buffer_Receive_Uart;
            p_data++;
            if (p_data % 2 == 0) {
                if (arr[p_data - 1] != arr[p_data - 2]) {
                    p_data = 0;
                    p_head = 0;
                    // return receive_notfinish;
                }
                if (p_data == sizeofarr) {
                    p_data = 0;
                    p_head = 0;
                    return receive_finish;
                }
            }
        } else {
            p_data = 0;
            p_head = 0;
            // return receive_notfinish;
        }
    }
    return receive_notfinish;
}

void transmit_get(unsigned char xdata *arr_receive, unsigned char xdata *arr_get, unsigned char sizeofarr)
{
    unsigned char i = 0;

    for (i = 0; i < sizeofarr; i++) {
        arr_get[i] = arr_receive[2 * i];
    }
}