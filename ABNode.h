#pragma once
#include "BasicStruct.h"

//��һ���ڵ����Ƭ��С�ڴ�ֵʱ�����ٽ��з���
#define BOX_MIN_NODE_NUM	3

//Aabb���Ľڵ�
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
	int face_num;										//���ڸýڵ��ڲ��Ķ������
	//int* points_x;									//���ڸýڵ��Χ���ڵĶ������
	//int* points_y;
	//int* points_z;
	int* faces_x;
	int* faces_y;
	int* faces_z;

	ABNode* left;									//�ýڵ�����ӽڵ㣬��ΪҶ�ӽڵ㣬��ΪNULL
	ABNode* right;									//�ýڵ�����ӽڵ㣬��ΪҶ�ӽڵ㣬��ΪNULL
	ABNode* parent;									//�ýڵ�ĸ��ڵ㣬��Ϊ��ʼ�ڵ㣬��ΪNULL
	//ABNode* brother;								//��ΪAabb����һ�������������нڵ㶼�����һ���ֵܽڵ�

	float x1, x2, y1, y2, z1, z2;					//��Χ����x,y,z���ϵ�λ��

private:
	bool isLeaf;									//�ýڵ��ǲ���Ҷ�ӽڵ�
};
