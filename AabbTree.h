#pragma once
#include "ABNode.h"

//this is for test
#include "MyTimer.h"

//��������ײ����Aabb Tree
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
	//void UpdateNodeBox(ABNode* node);				//���½ڵ�İ�Χ��
	//float getMin(float a, float b, float c);		//�õ�����ֵ�е���Сֵ
	//float getMax(float a, float b, float c);		//�õ�����ֵ�е����ֵ
	void checkNode(ABNode* node);
	void splitNode(ABNode* node);					//���ڵ���ѳ������ӽڵ㣬���Χ��С��MINSIZE,�򲻷���
	void RootLeaf(ABNode* node);					//���϶��µı����ڵ�
	void LeafRoot(ABNode* node);					//��Ҷ�ӽڵ����ϱ����ڵ�
	bool CheckFaceAndNode(int face_num);
	void updateSingleBox(ABNode* node);

private:
	ABNode* root;									//���ĸ��ڵ�
	ABNode** temp_collide_node;				
	float t_x1, t_x2, t_y1, t_y2, t_z1, t_z2;		//��ײ����߶ε�λ��

	//MyTimer* Timer;
	//MyTimer* timer1;
	//MyTimer* timer2;

public:
	BasicStruct* objectStruct;						//�������ģ������
	int* Collision_face;							//��ײ���Ľ����Ƭ����û����ײ�Ļ���������Ϊ0

	float* Collision_position;						//��ײ���õ�����е����Ƭ���ཻλ��
	ABNode** last_collide_node;						//��һ������⵽��ײ���λ��
};
