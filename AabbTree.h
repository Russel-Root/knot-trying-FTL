#pragma once
#include "ABNode.h"

//this is for test
#include "MyTimer.h"

//用来做碰撞检测的Aabb Tree
class AabbTree
{
public:
	AabbTree(void);
	AabbTree(BasicStruct* related);
public:
	~AabbTree(void);

public:
	void setRelatedMesh(BasicStruct* related);
public:
	void refreshTopoStruct(int* list1, int* list2);
	void refreshCollisionTree(ABNode* node, int* list);
	void UpdateBoundaries();
public:
	void setToolPosition(float x1, float y1, float z1, float x2, float y2, float z2);
public:
	void CollisionDetect();

private:
	//void UpdateNodeBox(ABNode* node);				//更新节点的包围盒
	//float getMin(float a, float b, float c);		//得到三个值中的最小值
	//float getMax(float a, float b, float c);		//得到三个值中的最大值
	void checkNode(ABNode* node);
	void splitNode(ABNode* node);					//将节点分裂成两个子节点，如包围盒小于MINSIZE,则不分裂
	void RootLeaf(ABNode* node);					//至上而下的遍历节点
	void LeafRoot(ABNode* node);					//从叶子节点向上遍历节点
	bool CheckFaceAndNode(int face_num);
	void updateSingleBox(ABNode* node);

private:
	ABNode* root;									//树的根节点
	ABNode** temp_collide_node;				
	float t_x1, t_x2, t_y1, t_y2, t_z1, t_z2;		//碰撞检测线段的位置

	//MyTimer* Timer;
	//MyTimer* timer1;
	//MyTimer* timer2;

public:
	BasicStruct* objectStruct;						//相关联的模型数据
	int* Collision_face;							//碰撞检测的结果面片，如没有碰撞的话，该数组为0

	float* Collision_position;						//碰撞检测得到的器械与面片的相交位置
	ABNode** last_collide_node;						//上一个被检测到碰撞点的位置
};
