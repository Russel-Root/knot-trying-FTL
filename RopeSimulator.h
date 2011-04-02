#ifndef __RopeSimulator_h__
#define __RopeSimulator_h__

#include "Vector3f.h"
#include "RopeStruct.h"
#include "BvhTree.h"

class RopeSimulator{
public:
	RopeSimulator();
	~RopeSimulator();

	void controlMovInput(Vector3f move3f, int ctrlPointIndex, int ctlNum); // 外部触发rope的运动（更新），外界输入的是3D向量 int ctrlPointIndex是外部触发的控制点序号（标识符）
    void controlMovInputAll(Vector3f move3f);
	void buildTree();
	void selfCollision();
	void setCollisonColor(bool flag);
public:
	RopeStruct*	geoModel;
	BVHTree*	tree;
};

#endif