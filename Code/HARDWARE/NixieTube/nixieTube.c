#include "nixieTube.h"
/**
*双片74LS573数码管
*/
u8 code tubeNum[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0x71,0x77,0x5e,0x00}; //共阴极段码 10:- 11:F 12:A 13:d 14:灭
u8 code errorNum[]={0x79,0x77,0x77,0x3f,0x77,0x00,0x00,0x00}; //error
void Delay_nixieTube(u16 time)		//@12.000MHz
{
	u16 x;
	for(x=time;x>0;x--);
}

void weixuan(u8 place){
	//选择显示位置
	//位选 低电平有效
	LE2 = 1;
	P0 = ~(0x01<<place); //改进版
	//下降沿锁存
	LE2 = 0;
	//低电平使能输出
	OE = 0;
}
void duanxuan(u8 num,u8 dot){
	//选择显示数字
	//段选
	LE1 = 1;
	P0 = 0x00;
	LE1 = 0;
	
	LE1 = 1;
	if(dot==1){ //显示小数点
		P0 = tubeNum[num];
		P0_7 = 1;
	}else if(dot==2) P0 = errorNum[num]; //显示错误提示
	else P0 = tubeNum[num]; //正常显示
	//清除段选
	//Delay_nixieTube(100);
	//P0 = 0x00;
	LE1 = 0;
	OE = 0;
}