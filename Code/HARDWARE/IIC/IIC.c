#include "IIC.h"
sbit IIC_SCL = P3^6;
sbit IIC_SDA = P3^4;
/**
*51单片机没有IIC接口 所以需要模拟IIC时序
*/
//OLED 时钟线和数据线
/**
*注意拉高电平需要的时间
*/
//1.起始信号
void IIC_Start(){
	//IIC_SCL高电平期间，IIC_SDA由高到低为起始信号
	IIC_SDA = 1;
	IIC_SCL = 1;
	IIC_SDA = 0;
	IIC_SCL = 0;
	
}
//2.终止信号
void IIC_Stop(){
	//IIC_SCL高电平期间，IIC_SDA由低到高为终止信号
	IIC_SCL = 0;
	IIC_SDA = 0;
	IIC_SCL = 1;
	IIC_SDA = 1;
	
}
//3.发送数据
void IIC_SendByte(u8 Byte){
	u8 i;
	for(i=0;i<8;i++){
		//从最高位依次赋值给IIC_SDA数据线上
		IIC_SDA = Byte&(0x80>>i); 
		//IIC_SCL高电平时 IIC_SDA才发送
		IIC_SCL = 1;
		IIC_SCL = 0;
	}
}
//4.接收数据
u8 IIC_ReceiveByte(){
	u8 i,Byte=0x00;
	//拉高电平 释放线权
	IIC_SDA = 1;
	for(i=0;i<8;i++){
		//IIC_SCL和IIC_SDA均为高电平释放线权
		IIC_SCL = 1;
		if(IIC_SDA){
			Byte|=(0x80>>i);
		}
		IIC_SCL = 0;
	}
	return Byte;
}
//5.发送应答位
void IIC_SendAck(u8 ack){
	IIC_SDA = ack;
	IIC_SCL = 1;
	IIC_SCL = 0;
	
}
//6.接收应答
u8 IIC_ReceiveAck(){
	u8 ack;
	//SDA拉高 释放线权给从机
	IIC_SDA = 1;
	IIC_SCL = 1;
	ack = IIC_SDA;
	IIC_SCL = 0;
	return ack;
}