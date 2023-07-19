#ifndef _transmit_h
#define _transmit_h

typedef xdata struct Uart_struct {
    volatile unsigned char  i;
    volatile unsigned char  end;
    volatile unsigned char  *Buffer;
    volatile unsigned char  sizeofBuffer;
    volatile unsigned char  timer_100us;
    volatile unsigned char  tail_bak;
} Uart_type;

#define receive_finish    1
#define receive_notfinish 0

void transmit_send(Uart_type *example, unsigned char xdata *arr, \
                   unsigned char sizeofarr, unsigned char addr);
bit transmit_receive(unsigned char xdata *arr_receive, unsigned char sizeofarr_receive, \
                     unsigned char addr, unsigned char Buffer_Receive_Uart);
void transmit_get(unsigned char xdata *arr_receive, unsigned char xdata *arr_get, unsigned char sizeofarr_get);
//----------------------------------------------------------------------------------------
void init_Uart(Uart_type *example, unsigned char *Buffer, unsigned char sizeofbuffer);
void mySend_UART(Uart_type *example, unsigned char Data) reentrant;
void UART_Process_Send(Uart_type *example, unsigned char number_uart);
void UART_Process_Send_ITR(Uart_type *example, unsigned char number_uart);

#endif

/*
multiple uart transmit
*/