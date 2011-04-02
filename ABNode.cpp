#include "ABNode.h"

ABNode::ABNode(void)
{
	face_num = 0;
	left = right = parent = NULL;
	x1 = y1 = z1 = 100;
	x2 = y2 = z2 = -100;
	isLeaf = false;							//默认的节点不是叶子节点
}

ABNode::~ABNode(void)
{
	delete left, right, parent;
	delete faces_x, faces_y, faces_z;
}

/************************************************************************/
/* 设置碰撞检测包围盒的范围                                             */
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
/* 根据点的位置来修正包围盒的大小                                       */
/************************************************************************/
void ABNode::FixBox(float x, float y, float z)
{
	//修正x轴上包围盒的范围
	if ( x < x1 )
		x1 = x;
	if ( x > x2 )
		x2 = x;

	//修正y轴上包围盒的范围
	if ( y < y1 )
		y1 = y;
	if ( y > y2 )
		y2 = y;

	//修正z轴上包围盒的范围
	if ( z < z1)
		z1 = z;
	if ( z > z2)
		z2 = z;
}

/************************************************************************/
/* 判断一条线段是否经过该节点的包围盒内部						        */
/************************************************************************/
bool ABNode::InBox(float p_x1, float p_y1, float p_z1, float p_x2, float p_y2, float p_z2)
{  
	float min_t = 0; 
	float max_t = 1;

	//先检查x轴方向
	float temp1, temp2;

	if(p_x2 > p_x1)
	{
		temp1 = (x1 - p_x1)/(p_x2 - p_x1);
		if( temp1 > min_t)
			min_t = temp1;

		temp2 = (x2 - p_x1)/(p_x2 - p_x1);
		if( temp2 < max_t)
			max_t = temp2;

		//如果t的范围中的最小值大于最大值，则返回false
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

		//如果t的范围中的最小值大于最大值，则返回false
		if(min_t > max_t)
			return false;
	}

	//当p_x1 == p_x2的时候
	else
	{
		//如果过x轴的平面不经过包围盒，则不可能会有相交，返回false
		if(!(p_x1 <= x2 && p_x1 >= x1))
			return false;
	}

	//再检查y轴方向
	if(p_y2 > p_y1)
	{
		temp1 = (y1 - p_y1)/(p_y2 - p_y1);
		if( temp1 > min_t)
			min_t = temp1;

		temp2 = (y2 - p_y1)/(p_y2 - p_y1);
		if( temp2 < max_t)
			max_t = temp2;

		//如果t的范围中的最小值大于最大值，则返回false
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

		//如果t的范围中的最小值大于最大值，则返回false
		if(min_t > max_t)
			return false;
	}

	//当p_y1 == p_y2的时候
	else
	{
		//如果过y轴的平面不经过包围盒，则不可能会有相交，返回false
		if(!(p_y1 <= y2 && p_y1 >= y1))
			return false;
	}

	//再检查z轴方向
	if(p_z2 > p_z1)
	{
		temp1 = (z1 - p_z1)/(p_z2 - p_z1);
		if( temp1 > min_t)
			min_t = temp1;

		temp2 = (z2 - p_z1)/(p_z2 - p_z1);
		if( temp2 < max_t)
			max_t = temp2;

		//如果t的范围中的最小值大于最大值，则返回false
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

		//如果t的范围中的最小值大于最大值，则返回false
		if(min_t > max_t)
			return false;
	}

	//当p_z1 == p_z2的时候
	else
	{
		//如果过z轴的平面不经过包围盒，则不可能会有相交，返回false
		if(!(p_z1 <= z2 && p_z1 >= z1))
			return false;
	}

	return true;
}