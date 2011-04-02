#include "MyTimer.h"


MyTimer::MyTimer()
{
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	_freq = tmp.QuadPart;
	costTime = 0;
}

void MyTimer::Start()            // ��ʼ��ʱ
{
	QueryPerformanceCounter(&_begin);
}

void MyTimer::End()                // ������ʱ
{
	QueryPerformanceCounter(&_end);
	costTime += ((float)(_end.QuadPart - _begin.QuadPart) / _freq);
}

void MyTimer::Reset()            // ��ʱ��0
{
	costTime = 0;
}

void MyTimer::showDuration()
{
	cout<< "Total Time: " << costTime << endl;
}

//���Ҫ�ѽ��������ļ�������Ҫ���ô˷���
void MyTimer::SetOutputFile(char *filename)
{
	file = new ofstream(filename);
}

void MyTimer::OutPutDuration()
{
	(*file)<< costTime<< endl;
}
