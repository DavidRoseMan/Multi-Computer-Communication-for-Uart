# ���ٽ������ڷ��ͻ������Լ�ͨ�����������Ϳ�Ѱַ�����ݰ�

## ���ٽ������ڷ��ͻ�����

```c
Uart_type uart_example;//����һ������������
unsigned char xdata uart_example_send_buffer[128];//�����Ļ������Ĵ�СΪ128�ֽڡ�

UART_Process_Send(& uart_example, 1or2or3or4)//�˺���100usִ��һ�Σ��Ҵ˶����봮��1or2or3or4�������
UART_Process_Send_ITR(& uart_example, 1or2or3or4)//�˺���������Ӧ�Ĵ����жϷ������С�


int main(void)
{
    init_Uart(&uart_example,uart_example_send_buffer,sizeof uart_example_send_buffer);//��ʼ���˶���
}
```

## ͨ�����������Ϳ�Ѱַ�����ݰ�

* ���Ͷ�
```c
unsigned char xdata package[10];//���ݰ������ݴ�СΪ10�ֽڡ�
transmit_send(uart_example,package,10,123);//�������ݰ�ͨ��uart_example�������Ĵ��ڷ��ͳ�ȥ�����͵Ķ���Ϊ��ַΪ123��
```

* ���ն�
```c
unsigned char xdata package_receive_buffer[20];//����һ�����ݰ����յĻ��������˻������Ĵ�СӦ��Ϊ20�ֽڡ�
unsigned char xdata package_receive[10];//���յ����ݾ������ݰ����ջ������Ĵ������ջ����������С�

//����Ӧ�Ĵ����жϷ������У�
if (���ձ��λ == 1) {
        ���ձ��λ = 0;
        if (transmit_receive(package_receive_buffer,20,123,����Ӳ�����ջ�����))//123Ӧ�뷢�Ͷ˵ĵ�ַһ�£�����Ӳ�����ջ�����ΪSBUF�ȵȡ�
            transmit_get(package_receive_buffer,package_recieve,10);//����У��ɹ��󣬴˺����Ὣ���ݰ����ջ����������ݾ�����������������С�
    }
```
