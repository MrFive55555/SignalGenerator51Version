#ifndef NIXIETUBE_H
#define NIXIETUBE_H
#include <regx52.h>
#include <intrins.h>

#define u8 unsigned char
#define u16 unsigned int
	
//74LS573参数配置
sbit OE = P1^3;
sbit LE1 = P1^4;
sbit LE2 = P1^5;
void Delay_nixieTube(u16 time);
//位选
void weixuan(u8 place);
//段选
void duanxuan(u8 num,u8 dot);	
#endif
