# 快速建立串口发送缓存区以及通过缓存区发送可寻址的数据包

## 快速建立串口发送缓存区

```c
Uart_type uart_example;//创建一个缓存区对象
unsigned char xdata uart_example_send_buffer[128];//创建的缓存区的大小为128字节。

UART_Process_Send(& uart_example, 1or2or3or4)//此函数100us执行一次，且此对象与串口1or2or3or4相关联。
UART_Process_Send_ITR(& uart_example, 1or2or3or4)//此函数放入相应的串口中断服务函数中。


int main(void)
{
    init_Uart(&uart_example,uart_example_send_buffer,sizeof uart_example_send_buffer);//初始化此对象。
}
```

## 通过缓存区发送可寻址的数据包

* 发送端
```c
unsigned char xdata package[10];//数据包的数据大小为10字节。
transmit_send(&uart_example,package,10,123);//将此数据包通过uart_example所关联的串口发送出去，发送的对象为地址为123。
```

* 接收端
```c
unsigned char xdata package_receive_buffer[20];//创建一个数据包接收的缓存区，此缓存区的大小应该为20字节。
unsigned char xdata package_receive[10];//接收的数据经过数据包接收缓存区的处理，最终会放入此数组中。

//在相应的串口中断服务函数中：
if (接收标记位 == 1) {
        接收标记位 = 0;
        if (transmit_receive(package_receive_buffer,20,123,串口硬件接收缓存区))//123应与发送端的地址一致，串口硬件接收缓存区为SBUF等等。
            transmit_get(package_receive_buffer,package_recieve,10);//数据校验成功后，此函数会将数据包接收缓存区的数据经过处理后放入此数组中。
    }
```
