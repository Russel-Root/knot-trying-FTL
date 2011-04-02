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
	dirty = false; // Ĭ������� dtʱ���� �ʵ��ƶ�������ָ����Χ�ڣ�����Ҫ���������dirtyĬ����false
	vecVelocity += vecForce / fMass * dt; // v = v + f / m * dt, ����t��delta t

	if ( (vecVelocity * dt).getLength() > statdoubDelta ){  // $$$ �����dtʱ�����ʵ��˶����루��v * dt�����󣨳���statdoubDelta��������Ҫ������������dirty��ʾ�Ƿ���Ҫ����
		vecOldPos = vecPos;
		dirty = true;
	}

	vecPos += vecVelocity * dt; // position = position + v * dt 
}