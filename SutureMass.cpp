#include "SutureMass.h"

double SutureMass::statdoubDelta = 0.001; // $$$

SutureMass::~SutureMass(){}

void SutureMass::init(){
	this->vecForce.x = 0.0;
	this->vecForce.y = 0.0;
	this->vecForce.z = 0.0;
}

void SutureMass::applyForce(Vector3f force){
	this->vecForce += force;
}

void SutureMass::simulate(float dt){
	dirty = false; // 默认情况是 dt时间内 质点移动距离在指定范围内，不需要纠正，因此dirty默认是false
	vecVelocity += vecForce / fMass * dt; // v = v + f / m * dt, 这里t是delta t

	if ( (vecVelocity * dt).getLength() > statdoubDelta ){  // $$$ 如果在dt时间内质点运动距离（即v * dt）过大（超过statdoubDelta），则需要纠正，这里用dirty表示是否需要纠正
		vecOldPos = vecPos;
		dirty = true;
	}

	vecPos += vecVelocity * dt; // position = position + v * dt 
}