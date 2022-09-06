#ifndef PCF8591_H
#define PCF8591_H
#include "IIC.h" //这里不要延时的IIC 否则会影响速度

//数模转换
void digitalToAnlog(u8 value);

#endif
