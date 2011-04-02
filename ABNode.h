#pragma once
#include "BasicStruct.h"

//当一个节点的面片数小于此值时，不再进行分裂
#define BOX_MIN_NODE_NUM	3

//Aabb树的节点
class ABNode
{
public:
	ABNode(void);
public:
	~ABNode(void);

public:
	bool isLeafNode(void){ return this->isLeaf; }
	void setLeaf(bool leaf){ this->isLeaf = leaf; }
	void setBox(float x1, float x2, float y1, float y2, float z1, float z2);
	void FixBox(float x, float y, float z);

public:
	bool InBox(float x1, float y1, float z1, float x2, float y2, float z2);

public:
	int face_num;										//处于该节点内部的顶点个数
	//int* points_x;									//处于该节点包围盒内的顶点序号
	//int* points_y;
	//int* points_z;
	int* faces_x;
	int* faces_y;
	int* faces_z;

	ABNode* left;									//该节点的右子节点，如为叶子节点，则为NULL
	ABNode* right;									//该节点的右子节点，如为叶子节点，则为NULL
	ABNode* parent;									//该节点的父节点，如为起始节点，则为NULL
	//ABNode* brother;								//因为Aabb树是一个二叉树，所有节点都最多有一个兄弟节点

	float x1, x2, y1, y2, z1, z2;					//包围盒在x,y,z轴上的位置

private:
	bool isLeaf;									//该节点是不是叶子节点
};
