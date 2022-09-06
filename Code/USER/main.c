#include "SignalGeneratorUI.h"
void main(){
	//恢复数据
	recoverData();
	//刷新数值显示
	recoverNumber();
	//进入设置状态
	set();
	selectWave();
	//定时器初始化
	TimerInit();
	while(1){
		//键盘扫描
		keyboard();
	}
}