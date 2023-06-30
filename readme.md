适用于多机串口通信
* 发送端函数：
    * void transmit_send(unsigned char xdata *arr, unsigned char sizeofarr, unsigned char addr);
* 接收端函数：
    * bit transmit_receive(unsigned char xdata *arr_receive, unsigned char sizeofarr_receive, unsigned char addr, unsigned char Buffer_Receive_Uart);
    * void transmit_get(unsigned char xdata *arr_receive, unsigned char xdata *arr_get, unsigned char sizeofarr_get);

函数参数说明：
1. tranmit_send函数：
    * arr: 发送的数据数组。
    * sizeofarr: 数组中的元素个数。
    * addr: 接收端地址
2. transimit_receive函数： 
    * arr_receive: 处理接收到的数据的中间层。
    * sieofarr_receive: 中间层数组中元素的个数,其大小应该为发送的数据数组的两倍。 
    * addr: 接收端地址。 
    * Buffer_Receive_Uart: 串口缓冲区，例如51的串口1为SBUF。
3. transimit_get函数：
    * arr_receive: 即是tranmit_receive函数中的arr_receive。
    * arr_get: 用于存储接收到的数据的数组。
    * sizeofarr_get: arr_get中元素的个数。

例子：
1. 发送端：
    transmit_send(...);
2. 接收端：
    if(transmit_receive(...))transmit_get(...); //在对数据的最后一位校验成功后，transmit_get函数会返回receive_finish,其余时候则返回receive_notfinish。
