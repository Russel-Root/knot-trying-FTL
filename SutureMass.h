#ifndef __SutureMass_h__
#define __SutureMass_h__

// SutureMass是缝合线的各个控制点，即质点

#include "Vector3f.h"
//using namespace std;

class SutureMass{
public:
	int iId; // iId是int，SutureMass的index
	float fMass; // 命名规则解释：fMass是float，SutureMass的值
	
	Vector3f vecOldPos;
	Vector3f vecPos;
	Vector3f vecVelocity;

	// force
	Vector3f vecForce;

	static double statdoubDelta; // $$$
	bool dirty; // $$$ 如果在dt时间内质点运动距离过大（超过statdoubDelta），则需要纠正，这里用dirty表示是否需要纠正
	
	// RGB
	double dColorR;
	double dColorG;
	double dColorB;

	// force
	Vector3f vecBendingForce;
	Vector3f vecTwistForce;
	Vector3f vecDissipativeFrictionForce; // 耗散摩擦力
	Vector3f vecConstForce;

public:
	~SutureMass();

	void init();
	void applyForce(Vector3f force);
	void simulate(float dt); // 在dt时间内的运动模拟，dt是delta t，即dt时间间隔内
};

#endif
