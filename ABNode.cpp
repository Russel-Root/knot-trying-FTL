#include "ABNode.h"

ABNode::ABNode(void)
{
	face_num = 0;
	left = right = parent = NULL;
	x1 = y1 = z1 = 100;
	x2 = y2 = z2 = -100;
	isLeaf = false;							//Ĭ�ϵĽڵ㲻��Ҷ�ӽڵ�
}

ABNode::~ABNode(void)
{
	delete left, right, parent;
	delete faces_x, faces_y, faces_z;
}

/************************************************************************/
/* ������ײ����Χ�еķ�Χ                                             */
/************************************************************************/
void ABNode::setBox(float x1, float x2, float y1, float y2, float z1, float z2)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->z1 = z1;
	this->z2 = z2;
}

/************************************************************************/
/* ���ݵ��λ����������Χ�еĴ�С                                       */
/************************************************************************/
void ABNode::FixBox(float x, float y, float z)
{
	//����x���ϰ�Χ�еķ�Χ
	if ( x < x1 )
		x1 = x;
	if ( x > x2 )
		x2 = x;

	//����y���ϰ�Χ�еķ�Χ
	if ( y < y1 )
		y1 = y;
	if ( y > y2 )
		y2 = y;

	//����z���ϰ�Χ�еķ�Χ
	if ( z < z1)
		z1 = z;
	if ( z > z2)
		z2 = z;
}

/************************************************************************/
/* �ж�һ���߶��Ƿ񾭹��ýڵ�İ�Χ���ڲ�						        */
/************************************************************************/
bool ABNode::InBox(float p_x1, float p_y1, float p_z1, float p_x2, float p_y2, float p_z2)
{  
	float min_t = 0; 
	float max_t = 1;

	//�ȼ��x�᷽��
	float temp1, temp2;

	if(p_x2 > p_x1)
	{
		temp1 = (x1 - p_x1)/(p_x2 - p_x1);
		if( temp1 > min_t)
			min_t = temp1;

		temp2 = (x2 - p_x1)/(p_x2 - p_x1);
		if( temp2 < max_t)
			max_t = temp2;

		//���t�ķ�Χ�е���Сֵ�������ֵ���򷵻�false
		if(min_t > max_t)
			return false;

	}

	else if ( p_x2 < p_x1)
	{
		temp1 = (x1 - p_x1)/(p_x2 - p_x1);
		if( temp1 < max_t)
			max_t = temp1;

		temp2 = (x2 - p_x1)/(p_x2 - p_x1);
		if( temp2 > min_t)
			min_t = temp2;

		//���t�ķ�Χ�е���Сֵ�������ֵ���򷵻�false
		if(min_t > max_t)
			return false;
	}

	//��p_x1 == p_x2��ʱ��
	else
	{
		//�����x���ƽ�治������Χ�У��򲻿��ܻ����ཻ������false
		if(!(p_x1 <= x2 && p_x1 >= x1))
			return false;
	}

	//�ټ��y�᷽��
	if(p_y2 > p_y1)
	{
		temp1 = (y1 - p_y1)/(p_y2 - p_y1);
		if( temp1 > min_t)
			min_t = temp1;

		temp2 = (y2 - p_y1)/(p_y2 - p_y1);
		if( temp2 < max_t)
			max_t = temp2;

		//���t�ķ�Χ�е���Сֵ�������ֵ���򷵻�false
		if(min_t > max_t)
			return false;

	}

	else if ( p_y2 < p_y1)
	{
		temp1 = (y1 - p_y1)/(p_y2 - p_y1);
		if( temp1 < max_t)
			max_t = temp1;

		temp2 = (y2 - p_y1)/(p_y2 - p_y1);
		if( temp2 > min_t)
			min_t = temp2;

		//���t�ķ�Χ�е���Сֵ�������ֵ���򷵻�false
		if(min_t > max_t)
			return false;
	}

	//��p_y1 == p_y2��ʱ��
	else
	{
		//�����y���ƽ�治������Χ�У��򲻿��ܻ����ཻ������false
		if(!(p_y1 <= y2 && p_y1 >= y1))
			return false;
	}

	//�ټ��z�᷽��
	if(p_z2 > p_z1)
	{
		temp1 = (z1 - p_z1)/(p_z2 - p_z1);
		if( temp1 > min_t)
			min_t = temp1;

		temp2 = (z2 - p_z1)/(p_z2 - p_z1);
		if( temp2 < max_t)
			max_t = temp2;

		//���t�ķ�Χ�е���Сֵ�������ֵ���򷵻�false
		if(min_t > max_t)
			return false;

	}

	else if ( p_z2 < p_z1)
	{
		temp1 = (z1 - p_z1)/(p_z2 - p_z1);
		if( temp1 < max_t)
			max_t = temp1;

		temp2 = (z2 - p_z1)/(p_z2 - p_z1);
		if( temp2 > min_t)
			min_t = temp2;

		//���t�ķ�Χ�е���Сֵ�������ֵ���򷵻�false
		if(min_t > max_t)
			return false;
	}

	//��p_z1 == p_z2��ʱ��
	else
	{
		//�����z���ƽ�治������Χ�У��򲻿��ܻ����ཻ������false
		if(!(p_z1 <= z2 && p_z1 >= z1))
			return false;
	}

	return true;
}