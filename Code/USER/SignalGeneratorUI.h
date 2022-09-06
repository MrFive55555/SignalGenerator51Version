#ifndef SGU_H
#define SGU_H
#include "matrixKeyboard.h"
#include "nixieTube.h"
#include "pcf8591.h"
#include "at24c02.h"
#define u8 unsigned char
#define u16 unsigned int
/*
*业务部分
*/
//键盘
void keyboard();
//设置
void set();
//选择波形
void selectWave();
//选择参数
void selectParameter();
//确认
void confirm();
//切换波形和参数时，显示各自的数字
void recoverNumber();
//移动键盘
void moveKey(u8 key);
//数码管显示 放到中断了
void showTube();
//输入数字
void inputNumber(u8 key);
//闪烁中断初始化
void TimerInit();
//闪烁中断函数
void interruptFlicker();
//写入数据
void writeData();
//错误检测
u8 isError();
//延时
void Delay5ms();
//保存数据
void saveData();
//恢复数据
void recoverData();
//延时5ms
void Delay5ms();
/*
*算法部分
*/
//频率中断函数
void interruptFreq();
//调节频率
void adjustFreq();
//调节电压
void adjustVoltage();
//调节占空比
void adjustDutyCycle();
#endif
