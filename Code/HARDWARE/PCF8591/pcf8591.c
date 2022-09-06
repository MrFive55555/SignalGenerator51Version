#include "pcf8591.h"
//数模转换 
/**
*电压转换公式：Vout = Vin*value/255   Vin=5V
得出value = 255*Vout/Vin;
*/
void digitalToAnlog(u8 value){
	//主机向从机写入地址
	IIC_Start();            
	IIC_SendByte(0x90);
	IIC_ReceiveAck();
	//选择模拟输出      
	IIC_SendByte(0x40);
	IIC_ReceiveAck();
	//发送数字量
	IIC_SendByte(value);
	IIC_SendAck(1);
	IIC_Stop();
}