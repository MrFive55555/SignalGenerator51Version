#ifndef AT24C02_H
#define AT24C02_H
#include <regx52.h>
#include <intrins.h>
#define u8 unsigned char
#define AT24C02_ADDRESS		0xA0
void AT24C02WriteByte(u8 page,Data);
u8 AT24C02ReadByte(u8 page);
#endif
