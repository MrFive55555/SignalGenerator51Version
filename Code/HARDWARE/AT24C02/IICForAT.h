#ifndef IICFORAT_H
#define IICFORAT_H
#include <regx52.h>
#include <intrins.h>
#define u8 unsigned char
void IIC_Start_AT();
void IIC_Stop_AT();
void IIC_SendByte_AT(u8 Byte);
u8 IIC_ReceiveByte_AT();
void IIC_SendAck_AT(u8 ack);
u8 IIC_ReceiveAck_AT();
void Delay10us();
#endif