#include "matrixKeyboard.h"
void delay_Matrix(u16 z){
	u16 x,y;
	for(x=z;x>0;x--){
		for(y=144;y>0;y--);
	}
}

/**
*矩阵键盘：扫描法
*/
u8 getMatrixKey(){
//逐列扫描
	u8 key = 16;
	P2 = 0xFF;
	P2_0 = 0;  //消抖 
	if(P2_4==0){delay_Matrix(20);while(P2_4==0);delay_Matrix(20);key=0;}
	if(P2_5==0){delay_Matrix(20);while(P2_5==0);delay_Matrix(20);key=1;}
	if(P2_6==0){delay_Matrix(20);while(P2_6==0);delay_Matrix(20);key=2;}
	if(P2_7==0){delay_Matrix(20);while(P2_7==0);delay_Matrix(20);key=3;}
	
	P2 = 0xFF;
	P2_1 = 0;  //消抖 
	if(P2_4==0){delay_Matrix(20);while(P2_4==0);delay_Matrix(20);key=4;}
	if(P2_5==0){delay_Matrix(20);while(P2_5==0);delay_Matrix(20);key=5;}
	if(P2_6==0){delay_Matrix(20);while(P2_6==0);delay_Matrix(20);key=6;}
	if(P2_7==0){delay_Matrix(20);while(P2_7==0);delay_Matrix(20);key=7;}
	
	P2 = 0xFF;
	P2_2 = 0;  //消抖 
	if(P2_4==0){delay_Matrix(20);while(P2_4==0);delay_Matrix(20);key=10;}
	if(P2_5==0){delay_Matrix(20);while(P2_5==0);delay_Matrix(20);key=8;}
	if(P2_6==0){delay_Matrix(20);while(P2_6==0);delay_Matrix(20);key=9;}
	if(P2_7==0){delay_Matrix(20);while(P2_7==0);delay_Matrix(20);key=11;}
	
	P2 = 0xFF;
	P2_3 = 0;  //消抖 
	if(P2_4==0){delay_Matrix(20);while(P2_4==0);delay_Matrix(20);key=12;}
	if(P2_5==0){delay_Matrix(20);while(P2_5==0);delay_Matrix(20);key=13;}
	if(P2_6==0){delay_Matrix(20);while(P2_6==0);delay_Matrix(20);key=14;}
	if(P2_7==0){delay_Matrix(20);while(P2_7==0);delay_Matrix(20);key=15;}
	
	return key;
}
/**
*矩阵键盘：线翻转法 有bug 有时候按键会检测多次
*/
//static u8 longPress = 0; //长按标志
/*u8 getMatrixKey(){
	
	u8 key;//键值
	u8 row,column; //行，列
	u8 position; //坐标
	
	//先列赋值为高电平
	matrixKey = 0xf0;
	if(!longPress&&matrixKey!=0xf0){
		column = matrixKey;
		delay_Matrix(5); //猜测是仿真bug 加延时 太快导致检测不到 （实物测试再修改）
		//再行赋值为高电平
		matrixKey = 0x0f;
		row = matrixKey;
		delay_Matrix(5)
		position = row|column; //相或获得坐标
		
		switch(position){
			case 0xee:key=0;break;
			case 0xde:key=1;break;
			case 0xbe:key=2;break;
			case 0x7e:key=3;break;
			case 0xed:key=4;break;
			case 0xdd:key=5;break;
			case 0xbd:key=6;break;
			case 0x7d:key=7;break;
			//这里有两个左右键 为了key值就是数字值 所以改变返回值
			case 0xeb:key=10;break; //左
			case 0xdb:key=8;break;
			case 0xbb:key=9;break;
			case 0x7b:key=11;break; //右
			case 0xe7:key=12;break; //设置
			case 0xd7:key=13;break; //波形
			case 0xb7:key=14;break; //参数
			case 0x77:key=15;break; //确认
		}
		longPress = 1;
		return key;
	}else if(matrixKey==0xf0){
		longPress = 0;
	}
	return 16;
}*/


