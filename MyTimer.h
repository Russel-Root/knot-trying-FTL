#ifndef __MyTimer_H__
#define __MyTimer_H__
#include <windows.h>

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class MyTimer
{
private:
	int _freq;
	LARGE_INTEGER _begin;
	LARGE_INTEGER _end;

public:
	float costTime;		        // 花费的时间(精确到微秒)
	ofstream* file;					// 输出文件

public:
	MyTimer();
	void Start();				    // 开始计时
	void End();		                // 结束计时

	void Reset();					// 计时清0
	void showDuration();			// 在cmd中打印时间差

	void SetOutputFile(char* filename);		//设置输出文件;
	void OutPutDuration();			// 把打印值全部写入文件
};
#endif