#ifndef IIC_H
#define IIC_H
#include <regx52.h>
#include <intrins.h>
#define u8 unsigned char

void IIC_Start();
void IIC_Stop();
void IIC_SendByte(u8 Byte);
u8 IIC_ReceiveByte();
void IIC_SendAck(u8 ack);
u8 IIC_ReceiveAck();

#endif
