#include "IICForAT.h" 
sbit IIC_SCL = P3^6;
sbit IIC_SDA = P3^4;
/**
*此IIC通信协议加入延时 因为at24c02对速度要求不高 且必须加延时 才能进行通信
而pcf8591的IIC不要加延时 否则运行速度更慢了……
*/
/**
*51单片机没有IIC接口 所以需要模拟IIC时序
*/
//OLED 时钟线和数据线
/**
*注意拉高电平需要的时间
*/
//1.起始信号
void IIC_Start_AT(){
	//IIC_SCL高电平期间，IIC_SDA由高到低为起始信号
	IIC_SDA = 1;
	Delay10us();
	IIC_SCL = 1;
	Delay10us();
	IIC_SDA = 0;
	Delay10us();
	IIC_SCL = 0;
	Delay10us();
}
//2.终止信号
void IIC_Stop_AT(){
	//IIC_SCL高电平期间，IIC_SDA由低到高为终止信号
	IIC_SCL = 0;
	Delay10us();
	IIC_SDA = 0;
	Delay10us();
	IIC_SCL = 1;
	Delay10us();
	IIC_SDA = 1;
	Delay10us();
}
//3.发送数据
void IIC_SendByte_AT(u8 Byte){
	u8 i;
	for(i=0;i<8;i++){
		//从最高位依次赋值给IIC_SDA数据线上
		IIC_SDA = Byte&(0x80>>i); 
		//IIC_SCL高电平时 IIC_SDA才发送
		Delay10us();
		IIC_SCL = 1;
		Delay10us();
		IIC_SCL = 0;
		Delay10us();
	}
}
//4.接收数据
u8 IIC_ReceiveByte_AT(){
	u8 i,Byte=0x00;
	//拉高电平 释放线权
	IIC_SDA = 1;
	Delay10us();
	for(i=0;i<8;i++){
		//IIC_SCL和IIC_SDA均为高电平释放线权
		IIC_SCL = 1;
		Delay10us();
		if(IIC_SDA){
			Byte|=(0x80>>i);
		}
		IIC_SCL = 0;
		Delay10us();
	}
	return Byte;
}
//5.发送应答位
void IIC_SendAck_AT(u8 ack){
	IIC_SDA = ack;
	Delay10us();
	IIC_SCL = 1;
	Delay10us();
	IIC_SCL = 0;
	Delay10us();
}
//6.接收应答
u8 IIC_ReceiveAck_AT(){
	u8 ack;
	//SDA拉高 释放线权给从机
	IIC_SDA = 1;
	Delay10us();
	IIC_SCL = 1;
	Delay10us();
	ack = IIC_SDA;
	Delay10us();
	IIC_SCL = 0;
	Delay10us();
	return ack;
}
void Delay10us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 2;
	while (--i);
}
