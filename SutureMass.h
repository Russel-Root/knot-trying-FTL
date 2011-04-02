#ifndef __SutureMass_h__
#define __SutureMass_h__

// SutureMass�Ƿ���ߵĸ������Ƶ㣬���ʵ�

#include "Vector3f.h"
//using namespace std;

class SutureMass{
public:
	int iId; // iId��int��SutureMass��index
	float fMass; // ����������ͣ�fMass��float��SutureMass��ֵ
	
	Vector3f vecOldPos;
	Vector3f vecPos;
	Vector3f vecVelocity;

	// force
	Vector3f vecForce;

	static double statdoubDelta; // $$$
	bool dirty; // $$$ �����dtʱ�����ʵ��˶�������󣨳���statdoubDelta��������Ҫ������������dirty��ʾ�Ƿ���Ҫ����
	
	// RGB
	double dColorR;
	double dColorG;
	double dColorB;

	// force
	Vector3f vecBendingForce;
	Vector3f vecTwistForce;
	Vector3f vecDissipativeFrictionForce; // ��ɢĦ����
	Vector3f vecConstForce;

public:
	~SutureMass();

	void init();
	void applyForce(Vector3f force);
	void simulate(float dt); // ��dtʱ���ڵ��˶�ģ�⣬dt��delta t����dtʱ������
};

#endif
