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
	float costTime;		        // ���ѵ�ʱ��(��ȷ��΢��)
	ofstream* file;					// ����ļ�

public:
	MyTimer();
	void Start();				    // ��ʼ��ʱ
	void End();		                // ������ʱ

	void Reset();					// ��ʱ��0
	void showDuration();			// ��cmd�д�ӡʱ���

	void SetOutputFile(char* filename);		//��������ļ�;
	void OutPutDuration();			// �Ѵ�ӡֵȫ��д���ļ�
};
#endif