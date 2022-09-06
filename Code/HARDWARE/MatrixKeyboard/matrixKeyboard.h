#ifndef MATRIXKEYBOARD_H
#define MATRIXKEYBOARD_H
#include <regx52.h>

#define u8 unsigned char
#define u16 unsigned int
#define matrixKey P2

//获取矩阵键盘号
u8 getMatrixKey();
void delay_Matrix(u16 z);
#endif
