#include "at24c02.h"
#include "IICForAT.h"
/**
  * @brief  AT24C02写入一个字节
  * @param  page 要写入字节的地址
  * @param  Data 要写入的数据
  * @retval 无
  */
void AT24C02WriteByte(u8 page,Data)
{
	//连接设备
	IIC_Start_AT();
	IIC_SendByte_AT(AT24C02_ADDRESS);
	IIC_ReceiveAck_AT();
	//发送页面 0-255页 255字节
	IIC_SendByte_AT(page);
	IIC_ReceiveAck_AT();
	//发送数据
	IIC_SendByte_AT(Data);
	IIC_ReceiveAck_AT();
	IIC_Stop_AT();
}

/**
  * @brief  AT24C02读取一个字节
  * @param  page 要读出字节的地址
  * @retval 读出的数据
  */
u8 AT24C02ReadByte(u8 page)
{
	u8 Data;
	//连接设备
	IIC_Start_AT();
	IIC_SendByte_AT(AT24C02_ADDRESS);
	IIC_ReceiveAck_AT();
	IIC_SendByte_AT(page);
	IIC_ReceiveAck_AT();
	//读取设备
	IIC_Start_AT();
	IIC_SendByte_AT(AT24C02_ADDRESS|0x01);
	IIC_ReceiveAck_AT();
	//读取数据
	Data=IIC_ReceiveByte_AT();
	IIC_SendAck_AT(1);
	IIC_Stop_AT();
	return Data;
}
