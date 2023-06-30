#ifndef _transmit_h
#define _transmit_h

#define receive_finish         1
#define receive_notfinish      0

void transmit_send(unsigned char xdata *arr, unsigned char sizeofarr, unsigned char addr);
bit transmit_receive(unsigned char xdata *arr_receive, unsigned char sizeofarr_receive, unsigned char addr, unsigned char Buffer_Receive_Uart);
void transmit_get(unsigned char xdata *arr_receive, unsigned char xdata *arr_get, unsigned char sizeofarr_get);


#endif

/*
多机串口通信
*/