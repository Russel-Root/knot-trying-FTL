#include "MyTimer.h"


MyTimer::MyTimer()
{
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	_freq = tmp.QuadPart;
	costTime = 0;
}

void MyTimer::Start()            // 开始计时
{
	QueryPerformanceCounter(&_begin);
}

void MyTimer::End()                // 结束计时
{
	QueryPerformanceCounter(&_end);
	costTime += ((float)(_end.QuadPart - _begin.QuadPart) / _freq);
}

void MyTimer::Reset()            // 计时清0
{
	costTime = 0;
}

void MyTimer::showDuration()
{
	cout<< "Total Time: " << costTime << endl;
}

//如果要把结果输出到文件，则需要调用此方法
void MyTimer::SetOutputFile(char *filename)
{
	file = new ofstream(filename);
}

void MyTimer::OutPutDuration()
{
	(*file)<< costTime<< endl;
}
