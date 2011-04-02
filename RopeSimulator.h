#ifndef __RopeSimulator_h__
#define __RopeSimulator_h__

#include "Vector3f.h"
#include "RopeStruct.h"
#include "BvhTree.h"

class RopeSimulator{
public:
	RopeSimulator();
	~RopeSimulator();

	void controlMovInput(Vector3f move3f, int ctrlPointIndex, int ctlNum); // �ⲿ����rope���˶������£�������������3D���� int ctrlPointIndex���ⲿ�����Ŀ��Ƶ���ţ���ʶ����
    void controlMovInputAll(Vector3f move3f);
	void buildTree();
	void selfCollision();
	void setCollisonColor(bool flag);
public:
	RopeStruct*	geoModel;
	BVHTree*	tree;
};

#endif