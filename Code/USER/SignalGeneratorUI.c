#include "SignalGeneratorUI.h"
/**
*信号发生器功能汇总
*/

//业务部分参数:
idata u8 setValue = 0;//set键值
idata u8 flickerChange = 1; //闪烁切换
idata leftRightValue = 1; //左右移动键值 0左 1右
idata u8 moveKeyOn = 0; //是否在参数的选择下按了移动键
idata u8 errorFlag = 0; //错误标志
idata char tubeOff = -1; //数码管显示灭的位置
idata u8 dotOn = 0; //开启显示小数点
//二维数组储存数值 idata间接寻址 直接寻址只有128byte 不够用
idata u16 number[4][4]={0}; ///0TF 1TA 2RF 3RD qian shi bai ge
//由此引发一个想法：step==当前所选择的波形和参数
idata u8 step = 2; //操作步数 0TF 1TA 2RF 3RD
data u8 index = 0; //nixieTubeFormat的数组下标
data u8 nixieTubeFormat[8]={0,0,0,10,10,10,0,0};//数码管格式化显示信息 控制段选信息 *业务的核心*
data u16 errorTimeCount = 0; //错误显示时间
data u16 flickerTimeCount = 0;//闪烁计时值
data u8 tubeIndex = 0;
data u16 showCount = 0;

//算法部分参数:
data u16 voltage = 0; //电压
data int changeVoltage = 0;//变化的电压
data u16 period = 0;//一个采样点的周期
data u8 samplingVoltage = 0; //每一份采样电压
data u8 rise = 0; //上升或者下降
data u16 highPeriod = 0;//高电平占用时间
data u16 lowPeriod = 0;//低电平占用时间
/**
*信号发生器 业务代码部分:
*/
//键盘扫描
void keyboard(){
	u8 key;
	key = getMatrixKey();
	if(key==12){ //设置
		//关闭波形输出
		IIC_Stop();
		TR0 = 0;
		set();
	}else if((setValue&&(key>=13&&key<=15))||setValue==0){ //设置之下的功能
		switch(key){
			case 13:IIC_Stop();TR0 = 0;selectWave();break;
			case 14:IIC_Stop();TR0 = 0;selectParameter();break;
			case 15:IIC_Stop();TR0 = 0;confirm();break;
		}
	}else if((key==10||key==11)&&moveKeyOn&&setValue){ //左右键
		moveKey(key);
	}else if((index>=4&&index<=7)&&(key>=0&&key<=9)){ //输入数字
		//直接将键盘值传入inputnumberRF函数当数字使用
		inputNumber(key);
	}
	//1s错误显示
	if(errorTimeCount>=5000){
		errorTimeCount = 0;
		errorFlag = 0; //清除错误标志
	}
}

//移动键盘 
void moveKey(u8 key){
	//三角波和矩形波的占空比部分两格移动
	if(step==0||step==1||step==3){
		//前两位显示清零
		nixieTubeFormat[4] = 10;
		nixieTubeFormat[5] = 10;
		//移动键值
		if(key==10) leftRightValue--; //左移
		else leftRightValue++; //右移
		//左边再按 回到右边
		if(leftRightValue<=-1) leftRightValue = 1;
		//右边再按 回到左边
		else if(leftRightValue>=2) leftRightValue = 0;
		//数组下标更改
		index = leftRightValue+6;
	}else{
		//矩形波频率四格移动
		if(key==10) leftRightValue--;//左移
		else leftRightValue++; //右移 
		//左边再按 回到右边
		if(leftRightValue<=-1) leftRightValue = 3;
		//右边再按 回到左边
		else if(leftRightValue>=4) leftRightValue = 0;
		//数组下标更改
		index = leftRightValue+4;
	}
}

//设置 
void set(){
	//按下设置键的键值反转
	setValue = ~setValue;
	index = 0;
	tubeOff = -1;
	if(setValue){
		//显示“S”
		nixieTubeFormat[0]=5;
	}else{
		//显示“O”
		nixieTubeFormat[0]=0;
	}
}

//选择波形
void selectWave(){
	//按下选择波形后 step切换
	if(step>=2) step = 0;
	else step = 2;
	//小数点显示默认清零
	dotOn = 0;
	//数组下标赋值为1
	index = 1;
	//每次按下选择换波形后 参数始终显示“F”开始
	nixieTubeFormat[index+1] = 11;
	//开启移动
	moveKeyOn = 1;
	//修改数码管显示文本
	//选择矩形波 显示2表示
	if(step) nixieTubeFormat[index]=step;
	//选择三角波 显示1表示
	else nixieTubeFormat[index]=step+1;
	recoverNumber();
}

//选择参数
void selectParameter(){
	//小数点显示默认清零
	dotOn = 0;
	//数组下标赋值为2
	index = 2;
	//移动键开启激活
	moveKeyOn = 1;
	//修改数码管显示文本
	if(step>=2){
		step++;
		if(step==4) step = 2;
		//显示“d”
		if(step==3) nixieTubeFormat[index]=step+10;
		//显示“F”
		else nixieTubeFormat[index]=step+9;
	}else{
		//显示“A”
		step++;
		step%=2;
		if(step){ 
			nixieTubeFormat[index]=step+11; 
			dotOn = 1;
		}
		//显示“F”	
		else nixieTubeFormat[index]=step+11;
		
	}
	recoverNumber();
}

//切换波形和参数时，显示各自的数字
void recoverNumber(){
	u8 i;
	switch(step){
		case 0:
		case 1:
		case 3:
		nixieTubeFormat[4] = 10;
		nixieTubeFormat[5] = 10;
		nixieTubeFormat[6] = number[step][0];
		nixieTubeFormat[7] = number[step][1];break;
		case 2:for(i=0;i<4;i++) nixieTubeFormat[i+4] = number[step][i];break;
	}
}

//输入数字
void inputNumber(u8 key){
	//step 0TF 1TR 2RF 3RD
	number[step][leftRightValue] = key;
	nixieTubeFormat[index] = key;
}

//确认
void confirm(){
	//错误检测
	u8 checkPass;
	checkPass = isError();
	if(checkPass){ //没错
		//数组下标置为0
		index = 0;
		tubeOff = -1;
		//清零设置键值和关闭移动键激活
		if(setValue) setValue = 0;
		if(moveKeyOn) moveKeyOn = 0;
		//显示“O”
		nixieTubeFormat[0]=0;
		//最后写入数据
		saveData();
		writeData();
	}else{ //错误 显示1s的错误提示信息
		errorFlag = 1;
	}
}

//显示数码管
void showTube(){
	weixuan(tubeIndex);
	if(tubeIndex==tubeOff&&!errorFlag) duanxuan(14,0); //不显示 (实际上,传入全灭的参数)
	else if(dotOn&&tubeIndex==6&&!errorFlag) duanxuan(nixieTubeFormat[tubeIndex],1); //幅值挡 显示小数点
	else if(errorFlag) duanxuan(tubeIndex,2); //错误显示
	else duanxuan(nixieTubeFormat[tubeIndex],0); //正常显示
	tubeIndex++;
	tubeIndex%=8;
}

//检查是否错误输入
u8 isError(){
	u8 checkPass1 = 0; //0出错 1无错
	u8 checkPass2 = 0; 
	//分三角波和矩形波来检查
	if(step<=1){
		//三角波频率 范围 f = 0-20hz
		if(number[0][0]>=0&&number[0][0]<5) checkPass1 = 1;
		else if(number[0][0]==5&&number[0][1]==0) checkPass1 = 1;
		else checkPass1 = 0;
		//幅值 vpp = 1-5v
		if(number[1][0]>=1&&number[1][0]<5) checkPass2 = 1;
		else if(number[1][0]==5&&number[1][1]==0) checkPass2 = 1;
		else checkPass2 = 0;
	}else{
		//矩形波频率 f = 20-1000hz
		if(number[2][0]<1) checkPass1 = 1;
		else if(number[2][0]==1&&number[2][1]==0&&number[2][2]==0&&number[2][3]==0) checkPass1 = 1;
		else if(number[2][0]==0&&number[2][1]==0&&number[2][2]<2) checkPass1 = 0;
		else checkPass1 = 0;
		//占空比
		if(number[3][0]>=2&&number[3][0]<8) checkPass2 = 1;
		else if(number[3][0]==8&&number[3][1]==0) checkPass2 = 1;
		else checkPass2 = 0;
	}
	return checkPass1&checkPass2;
}

//定时器初始化
void TimerInit(){
	//定时器1 16位自动重载 定时器0 8位
	TMOD = 0x21;
	//200us溢出
	TH1 = 56;
	TL1 = 56;
	EA = 1;
	ET1 = 1;
	TR1 = 1;
	ET0 = 1;
	TR0 = 0;
}

//暂时不要加using 会导致程序错误 如中断无法使用
//闪烁中断函数
void interruptFlicker() interrupt 3 {
	showCount++;
	flickerTimeCount++;
	if(showCount>=12){
		showCount = 0;
		showTube();
	}
	//300ms溢出
	if(flickerTimeCount>=1500&&index!=0){
		flickerTimeCount = 0;
		//决定哪一位显示灭 不再更改数组下标 原值得以保留
		if(flickerChange) tubeOff=index;
		//清零显示灭的位
		else tubeOff = -1;
		flickerChange = !flickerChange; //仿真bug 这里不能用~取反 要用!
	}
	if(errorFlag) errorTimeCount++;
}

//写入数据
void writeData(){
	adjustFreq();
}
//延时5ms
void Delay5ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 10;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
}

//保存数据
void saveData(){
	//写入数据需要至少5ms延时 确保写入完毕
	//保存数码管显示的前三位信息
	u8 i,j,k;
	for(i=0;i<4;i++){
		if(i==3) AT24C02WriteByte(i,step); //3
		else AT24C02WriteByte(i,nixieTubeFormat[i]); //0 1 2
			//保存当前step信息
		Delay5ms();
	}
	//保存数值信息
	k = 4;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			/*4 5 6 7
			* 8 9 10 11
			* 12 13 14 15
			* 16 17 18 19
			*/
			AT24C02WriteByte(k,number[i][j]);
			Delay5ms();
			k++;
		}
	}
}

//开机恢复数据
void recoverData(){
	//恢复数码管数据
	u8 i,j,k;
	for(i=0;i<4;i++){
		if(i==3) step = AT24C02ReadByte(i); //3
		else nixieTubeFormat[i] = AT24C02ReadByte(i); //0 1 2
	}
	//恢复数值数据
	k = 4;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			/*4 5 6 7
			* 8 9 10 11
			* 12 13 14 15
			* 16 17 18 19
			*/
			number[i][j] = AT24C02ReadByte(k);
			k++;
		}
	}
}
/**
*算法部分:
*/
void interruptFreq() interrupt 1{
	/*调用此中断时 发现数码管无法显示 猜测如下*/
	//写入数字量
	//digitalToAnlog(255);
	//猜测:period计算值有问题 digitalToAnlog也有问题 最终发现period有问题 period可能计算为0 导致无限卡在中断里
	//最后发现以上猜想都错了 真正的错误原因 在于中断使用了using x导致程序卡死
	//但最后发现 频率快了 中断全部用于写入数据时 没时间去执行数码管显示
	//只能在中断函数中写入数据了 因为只有它在循环
	//上半周期 单调递增
	EA = 0;
	if(step>=2){
		rise = !rise;
		if(rise){
			TH0 = highPeriod>>8;
			TL0 = highPeriod;
			P1_6 = 1;
		}else{
			TH0 = lowPeriod>>8;
			TL0 = lowPeriod;
			P1_6 = 0;
		}
	}else{
		//最多44hz三角波了 因为PCF8591和单片机的运行速度极限了 没那么快了
		//并且我使用了较多的函数封装 对性能有一定损失 但函数封装总归是好的
		TH0 = period>>8;
		TL0 = period;
		if(rise){
			changeVoltage+=samplingVoltage;
			if(changeVoltage>=voltage){ 
				changeVoltage = voltage;
				rise = !rise;
			}
		}else{
			changeVoltage-=samplingVoltage; 
			//以下语句就没执行 所以导致上升部分执行不了 changeVoltage 减去0没值了 很奇怪
			//将changeVoltage改成有符号的数字即可解决问题
			if(changeVoltage<=0){
				changeVoltage = 0;
				rise = !rise;
			}				
		}
		//中断发送数据信号即可 不再多次连接 浪费时间
		IIC_SendByte(changeVoltage);
		IIC_SendAck(1);
	}
	EA = 1;

}

//调节频率
void adjustFreq(){ 
		/**F=10hz-50hz T=100000us-20000us
		*三角波 采样点16个一周期 所以T/16; T=6250us-1250us; 1hz周期12500us
		*电压也要分成32份
		*/
	//三角波
	if(step<=1){
		float freq;
		freq = (float)(number[0][0]*10+number[0][1]);
		freq = 31250/freq;
		//四舍五入
		if((freq-(u16)freq)>=0.5) period = (u16)freq+1;
		else period = (u16)freq; //得到一个采样点的周期
		//赋值并启动定时器1
		adjustVoltage(); //得到电压数字量
	}else{ //矩形波
		/**
		*F=20HZ-1000HZ T=50000us-1000us
		*/
		float freq;
		freq = (float)(number[2][0]*1000+number[2][1]*100+number[2][2]*10+number[2][3]);
		freq = (float)1000000/freq;
		//四舍五入
		if((freq-(u16)freq)>=0.5) period = (u16)freq+1;
		else period = (u16)freq; //一个矩形波的周期 高低电平为一周期
		adjustDutyCycle();
	}
}
//调节电压
void adjustVoltage(){ //1.0v-5.0v
	//数据处理 
	float temp;
	temp =(float)(number[1][0]*10+number[1][1])/10;
	temp = temp*255/5;
	//四舍五入
	if((temp-(u16)temp)>=0.5) voltage = (u16)temp+1;
	else voltage = (u16)temp;
	temp = temp/16;  //每一份采样数字量 
	if((temp-(u16)temp)>=0.5) samplingVoltage = (u16)temp+1;
	else samplingVoltage = (u16)temp;
	period = 65536-period;
	TH0 = period>>8;
	TL0 = period;
	rise = 1;//上升
	/**
	*问题解决的关键：不要多次连接 连接设备所耗费的时间比较长
	*所以只发送一次连接信号 其余只要发送数据信号即可
	*究其原因对IIC不够了解 灵感来源：管琼老师
	*/
	//主机向从机写入地址
	IIC_Start();            
	IIC_SendByte(0x90);
	IIC_ReceiveAck();
	//选择模拟输出      
	IIC_SendByte(0x40);
	IIC_ReceiveAck();
	//中断里发送数据
	TR0 = 1;
}

//调节占空比
void adjustDutyCycle(){
	//20%-80%
	float temp;
	//u16类型转换位float参与浮点运算
	temp = (float)(number[3][0]*10+number[3][1])/100;
	//求出高电平占用时间
	temp = temp*(float)period;
	if((temp-(u16)temp)>=0.5)  highPeriod = (u16)temp+1;
	else highPeriod = (u16)temp;
	lowPeriod = period - highPeriod;
	//频率补偿
	highPeriod = 65536-highPeriod+60;
	lowPeriod = 65536-lowPeriod+60;
	//赋值别加上算术运算 否则会导致定时器时间不准确
	TH0 = highPeriod>>8;
	TL0 = highPeriod;
	rise = 1;
	TR0 = 1;
}