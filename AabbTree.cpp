#include "AabbTree.h"



AabbTree::AabbTree(void)
{
	root = NULL;

	//将检测到的面片放到此数组中，一般情况下同时切割的面片不会大于10
	Collision_face = new int[6];
	last_collide_node = new ABNode*[6];
	temp_collide_node = new ABNode*[6];
	Collision_position = new float[18];
	for(int i = 0; i < 6; i++)
	{
		Collision_face[i] = 0;
		Collision_position[3 * i] = Collision_position[3 * i + 1] = Collision_position[3 * i + 2] = 0.0f;
	}

	//this is for test
	//Timer = new MyTimer();
	//timer1 = new MyTimer();
	//timer2 = new MyTimer();
}

//通过相关联的模型数据构造树
AabbTree::AabbTree(BasicStruct* related)
{
	root = NULL;

	//将检测到的面片放到此数组中，一般情况下同时切割的面片不会大于10
	Collision_face = new int[6];  //@ 3*2
	last_collide_node = new ABNode*[6];
	temp_collide_node = new ABNode*[6];

	Collision_position = new float[18]; //@ 6*3
	for(int i = 0; i < 6; i++)
	{
		Collision_face[i] = 0;
		Collision_position[3 * i] = Collision_position[3 * i + 1] = Collision_position[3 * i + 2] = 0.0f;
	}

	//this is for test
	//Timer = new MyTimer();
	//timer1 = new MyTimer();
	//timer1->SetOutputFile("addLeafToRoot4.txt");
	//timer2 = new MyTimer();
	//timer2->SetOutputFile("RootToLeaf4.txt");

	//@	this->objectStruct = related; //@ 构建Aabb结构的对象结构，即刚导入的对象数据

	setRelatedMesh(related);
}

AabbTree::~AabbTree(void)
{
	delete root;
	delete last_collide_node;
	delete temp_collide_node;
	delete objectStruct;
	delete Collision_face;
	delete Collision_position;
}

/************************************************************************/
/* 通过已知的模型数据构造二叉碰撞检测树                                 */
/************************************************************************/
void AabbTree::setRelatedMesh(BasicStruct* related)
{
	//将相关联的模型指向到新的模型,将原先的碰撞检测树删除
	this->objectStruct = related;
	if(root!=NULL)
		delete this->root;

	related->RangeFaces();
	//建立Aabb树
	root = new ABNode();
	int num = objectStruct->face_num;
	root->face_num = num;

	//建立根节点的包围盒
	root->setBox(objectStruct->min_x, objectStruct->max_x, objectStruct->min_y, objectStruct->max_y, 
		objectStruct->min_z, objectStruct->max_z);
	root->faces_x = new int[num];
	root->faces_y = new int[num];
	root->faces_z = new int[num];

	for(int i = 0; i < num; i++)
	{
		root->faces_x[i] = objectStruct->range_x[i];
		root->faces_y[i] = objectStruct->range_y[i];
		root->faces_z[i] = objectStruct->range_z[i];
	}

	//分裂根节点
	this->splitNode(root);
	//cout<<"排列另外两个所花费的总时间： " << Timer->costTime << "s" << endl;
	//cout<< "找到边界值所花费的总时间："<< timer1->costTime <<"s"<<endl;
	//cout<<"创建第一个数组所花费的时间：" << timer2->costTime << "s" <<endl;
	//Timer->Reset();
	//timer1->Reset();
	//timer2->Reset();
}

/************************************************************************/
/* 将节点分裂出左右两个子节点，若包围盒的变长均小于一定值，则不再细分   */
/************************************************************************/
void AabbTree::splitNode(ABNode* node)
{
	if(node->face_num == 1)
	{
		//如果该节点只包围了一个面片
		node->setLeaf(true);
		//不需要分裂叶子节点
		return;
	}

	//否则将其分裂为左右两节点，并设置他们之间的关系
	ABNode* left = new ABNode();
	ABNode* right = new ABNode();
	node->left = left;
	node->right = right;
	left->parent = node;
	right->parent = node;

	//为构造一棵平衡树，找出父节点中比较长的一条边进行分裂
	float x = node->x2 - node->x1;
	float y = node->y2 - node->y1;
	float z = node->z2 - node->z1;

	int num = node->face_num;	
	left->face_num = (num - 1)/2 + 1;
	right->face_num = num / 2;
	int* left_faces_x = new int[left->face_num];
	int* left_faces_y = new int[left->face_num];
	int* left_faces_z = new int[left->face_num];
	int* right_faces_x = new int[right->face_num];
	int* right_faces_y = new int[right->face_num];
	int* right_faces_z = new int[right->face_num];

	//设置左右两个节点的各种参数
	left->faces_x = left_faces_x;
	left->faces_y = left_faces_y;
	left->faces_z = left_faces_z;

	right->faces_x = right_faces_x;
	right->faces_y = right_faces_y;
	right->faces_z = right_faces_z;

	//如果x轴上的span大于其他两个轴上的时候，分裂x轴
	if(x >= y && x >= z)
	{
		//更新所有方向的集合内部数据
		for(int i = 0; i < left->face_num; i++)
			left_faces_x[i] = node->faces_x[i];

		for(int i = 0; i < right->face_num; i++)
			right_faces_x[i] = node->faces_x[i + left->face_num];

		//两个计数器和一个控制标识符
		int n1, n2;
		bool inLeft;
		float max_x = objectStruct->face_Center[3 * left_faces_x[left->face_num - 1]];
		float min_x = objectStruct->face_Center[3 * right_faces_x[0]];

		//分裂y轴的数组
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_y[i];
			float temp_x = objectStruct->face_Center[3 * temp_face];
			//如果此面片的中间位置的x坐标大于左边节点的最小值，则加入左边节点的y数组
			if(min_x == max_x && temp_x == max_x)
			{						
				inLeft = false;
				//遍历left_face_x看看temp_face在不在里面，若在，则放在left_face_y里
				for(int j = left->face_num - 1; j >= 0; j--)
				{

					if(temp_face == left_faces_x[j])
					{
						left_faces_y[n1] = temp_face;
						n1++;
						inLeft= true;
						break;
					}

					if(objectStruct->face_Center[3 * left_faces_x[j]] > max_x)
						break;
				}

				//如果这个数不在右边的x数组里，则把他放到左边的数组里面
				if(!inLeft)
				{
					right_faces_y[n2] = temp_face;
					n2++;
				}
			}

			else if(temp_x >= max_x)
			{
				left_faces_y[n1] = temp_face;
				n1++;
			}

			//如果此值小于右边节点的最大值，则放到有边界点中
			else
			{
				right_faces_y[n2] = temp_face;
				n2++;
			}
		}

		//分裂z轴的数组
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_z[i];
			float temp_x = objectStruct->face_Center[3 * temp_face];
			//如果此面片的中间位置的x坐标大于左边节点的最小值，则加入左边节点的y数组
			if(min_x == max_x && temp_x == max_x)
			{
				inLeft = false;
				//遍历left_face_x看看temp_face在不在里面，若在，则放在left_face_y里
				for(int j = left->face_num-1; j >= 0; j--)
				{

					if(temp_face == left_faces_x[j])
					{
						left_faces_z[n1] = temp_face;
						n1++;
						inLeft= true;
						break;
					}

					if(objectStruct->face_Center[3 * left_faces_x[j]] > max_x)
						break;
				}

				//如果这个数不在右边的x数组里，则把他放到左边的数组里面
				if(!inLeft)
				{
					right_faces_z[n2] = temp_face;
					n2++;
				}
			}

			else if(temp_x >= max_x)
			{
				left_faces_z[n1] = node->faces_z[i];
				n1++;
			}

			//如果此值小于右边节点的最大值，则放到有边界点中
			else
			{
				right_faces_z[n2] = node->faces_z[i];
				n2++;
			}
		}
	}

	//分裂y轴
	else if ( y >= x && y >= z)
	{
		//更新所有方向的集合内部数据
		for(int i = 0; i < left->face_num; i++)
			left_faces_y[i] = node->faces_y[i];

		for(int i = 0; i < right->face_num; i++)
			right_faces_y[i] = node->faces_y[i + left->face_num];

		//两个计数器和一个控制标识符
		int n1, n2;
		bool inLeft;
		float max_y = objectStruct->face_Center[3 * left_faces_y[left->face_num - 1] + 1];
		float min_y = objectStruct->face_Center[3 * right_faces_y[0] + 1];

		//分裂x轴的数组
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_x[i];
			float temp_y = objectStruct->face_Center[3 * temp_face + 1];
			//如果此面片的中间位置的x坐标大于左边节点的最小值，则加入左边节点的y数组
			if(min_y == max_y && temp_y == max_y)
			{						
				inLeft = false;
				//遍历left_face_x看看temp_face在不在里面，若在，则放在left_face_y里
				for(int j = left->face_num - 1; j >= 0; j--)
				{

					if(temp_face == left_faces_y[j])
					{
						left_faces_x[n1] = temp_face;
						n1++;
						inLeft= true;
						break;
					}

					if(objectStruct->face_Center[3 * left_faces_y[j] + 1] > max_y)
						break;
				}

				//如果这个数不在右边的x数组里，则把他放到左边的数组里面
				if(!inLeft)
				{
					right_faces_x[n2] = temp_face;
					n2++;
				}
			}

			else if(temp_y >= max_y)
			{
				left_faces_x[n1] = temp_face;
				n1++;
			}

			//如果此值小于右边节点的最大值，则放到有边界点中
			else
			{
				right_faces_x[n2] = temp_face;
				n2++;
			}
		}

		//分裂z轴的数组
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_z[i];
			float temp_y = objectStruct->face_Center[3 * temp_face + 1];
			//如果此面片的中间位置的x坐标大于左边节点的最小值，则加入左边节点的y数组
			if(min_y == max_y && temp_y == max_y)
			{
				inLeft = false;
				//遍历left_face_x看看temp_face在不在里面，若在，则放在left_face_y里
				for(int j = left->face_num-1; j >= 0; j--)
				{

					if(temp_face == left_faces_y[j])
					{
						left_faces_z[n1] = temp_face;
						n1++;
						inLeft= true;
						break;
					}

					if(objectStruct->face_Center[3 * left_faces_y[j] + 1] > max_y)
						break;
				}

				//如果这个数不在右边的x数组里，则把他放到左边的数组里面
				if(!inLeft)
				{
					right_faces_z[n2] = temp_face;
					n2++;
				}
			}

			else if(temp_y >= max_y)
			{
				left_faces_z[n1] = temp_face;
				n1++;
			}

			//如果此值小于右边节点的最大值，则放到有边界点中
			else
			{
				right_faces_z[n2] = temp_face;
				n2++;
			}
		}
	}

	//分裂z轴
	else
	{
		//更新所有方向的集合内部数据
		for(int i = 0; i < left->face_num; i++)
			left_faces_z[i] = node->faces_z[i];

		for(int i = 0; i < right->face_num; i++)
			right_faces_z[i] = node->faces_z[i + left->face_num];

		//两个计数器和一个控制标识符
		int n1, n2;
		bool inLeft;
		float max_z = objectStruct->face_Center[3 * left_faces_z[left->face_num - 1] + 2];
		float min_z = objectStruct->face_Center[3 * right_faces_z[0] + 2];

		//分裂y轴的数组
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_y[i];
			float temp_z = objectStruct->face_Center[3 * temp_face + 2];
			//如果此面片的中间位置的x坐标大于左边节点的最小值，则加入左边节点的y数组
			if(min_z == max_z && temp_z == max_z)
			{						
				inLeft = false;
				//遍历left_face_x看看temp_face在不在里面，若在，则放在left_face_y里
				for(int j = left->face_num - 1; j >= 0; j--)
				{

					if(temp_face == left_faces_z[j])
					{
						left_faces_y[n1] = temp_face;
						n1++;
						inLeft= true;
						break;
					}

					if(objectStruct->face_Center[3 * left_faces_z[j] + 2] > max_z)
						break;
				}

				//如果这个数不在右边的x数组里，则把他放到左边的数组里面
				if(!inLeft)
				{
					right_faces_y[n2] = temp_face;
					n2++;
				}
			}

			else if(temp_z >= max_z)
			{
				left_faces_y[n1] = temp_face;
				n1++;
			}

			//如果此值小于右边节点的最大值，则放到有边界点中
			else
			{
				right_faces_y[n2] = temp_face;
				n2++;
			}
		}

		//分裂x轴的数组
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_x[i];
			float temp_z = objectStruct->face_Center[3 * temp_face + 2];
			//如果此面片的中间位置的x坐标大于左边节点的最小值，则加入左边节点的y数组
			if(min_z == max_z && temp_z == max_z)
			{
				inLeft = false;
				//遍历left_face_z看看temp_face在不在里面，若在，则放在left_face_x里
				for(int j = left->face_num-1; j >= 0; j--)
				{

					if(temp_face == left_faces_z[j])
					{
						left_faces_x[n1] = temp_face;
						n1++;
						inLeft= true;
						break;
					}

					if(objectStruct->face_Center[3 * left_faces_z[j] + 2] > max_z)
						break;
				}

				//如果这个数不在右边的x数组里，则把他放到左边的数组里面
				if(!inLeft)
				{
					right_faces_x[n2] = temp_face;
					n2++;
				}
			}

			else if(temp_z >= max_z)
			{
				left_faces_x[n1] = temp_face;
				n1++;
			}

			//如果此值小于右边节点的最大值，则放到有边界点中
			else
			{
				right_faces_x[n2] = temp_face;
				n2++;
			}
		}
	}

	//更新最小包围盒的大小
	int tmp_face, tmp_pt;
	for(int i = 0; i < (num - 1)/2 + 1; i++)
	{
		tmp_face = left->faces_x[i];
		for(int j = 0; j < 3; j++)
		{
			tmp_pt = objectStruct->faces[3 * tmp_face + j];
			left->FixBox(objectStruct->points[3 * tmp_pt], objectStruct->points[3 * tmp_pt + 1], objectStruct->points[3 * tmp_pt + 2]);
		}
	}

	for(int i = 0; i < num / 2; i++)
	{
		tmp_face = right->faces_x[i];
		for(int j = 0; j < 3; j++)
		{
			tmp_pt = objectStruct->faces[3 * tmp_face + j];
			right->FixBox(objectStruct->points[3 * tmp_pt], objectStruct->points[3 * tmp_pt + 1], objectStruct->points[3 * tmp_pt + 2]);
		}
	}

	splitNode(left);
	splitNode(right);
}

/************************************************************************/
/* 设置碰撞检测点的位置(x1,y1,z1),而(x2,y2,z2)在具体检测的时候才用到    */
/************************************************************************/
void AabbTree::setToolPosition(float x1, float y1, float z1, float x2, float y2, float z2)
{
	this->t_x1 = x1;
	this->t_x2 = x2;
	this->t_y1 = y1;
	this->t_y2 = y2;
	this->t_z1 = z1;
	this->t_z2 = z2;
}

/************************************************************************/
/* 遍历整棵Aabb树，如果上一个时刻已经碰到了某个面片，则从这个叶子节点开 */
/* 始遍历，否则则从根节点开始遍历                                       */
/************************************************************************/
void AabbTree::CollisionDetect()
{
	//重置碰撞检测面片的数组	
	int collide_num = this->Collision_face[0];
	Collision_face[0] = 0;

	//this is for test
	//timer1->Start();
	//当上一个状态有检测到碰撞的时候，从上一个位置的叶子节点开始遍历
	//if(collide_num != 0)
	//{
	//	for(int i = 1; i<= collide_num; i++)
	//		LeafRoot(temp_collide_node[i]);
	//}
	////从根节点开始遍历
	//else
	RootLeaf(root);

	//将这次碰撞的节点数组指向temp_collide_node
	for(int i = 1; i <= this->Collision_face[0]; i++)
	{
		last_collide_node[i] = temp_collide_node[i];


	}

	if(this->Collision_face[0]>0)
	{
		int i = 1;
		//this is a trial
		//把所有发现碰撞检测的面片全部往法相方向推进
		int tmp_face = Collision_face[i];
		Vector3f* collide_p1 = new Vector3f(t_x1 - Collision_position[3*i-3], t_y1 - Collision_position[3*i-2], t_z1 - Collision_position[3*i-1]);
		Vector3f* collide_p2 = new Vector3f(t_x2 - Collision_position[3*i-3], t_y2 - Collision_position[3*i-2], t_z2 - Collision_position[3*i-1]);
		Vector3f* up_Vector = new Vector3f(this->objectStruct->f_normals[3 * tmp_face],this->objectStruct->f_normals[3 * tmp_face+1],this->objectStruct->f_normals[3 * tmp_face+2]);
		//短的那头认为是面片的平移方向
		Vector3f* move = collide_p1;
		if((*collide_p2) * (*up_Vector) < 0)
			move = collide_p2;

		int tmp_point;
		//与面片相关的三个顶点
		for(int j = 0; j < 3; j++)
		{
			tmp_point = this->objectStruct->faces[tmp_face * 3 + j];
			this->objectStruct->points[tmp_point * 3] += move.x;
			this->objectStruct->points[tmp_point * 3 + 1] += move.y;
			this->objectStruct->points[tmp_point * 3 + 2] += move.z;

			//this->objectStruct->points[tmp_point * 3] = (t_x2 + t_x1)/2;
			//this->objectStruct->points[tmp_point * 3 + 1] = (t_y2 + t_y1)/2;
			//this->objectStruct->points[tmp_point * 3 + 2] = (t_z2 + t_z1)/2;

			//此点的速度设置为0
			(*this->objectStruct->v_speed[tmp_point]).setVector(0, 0, 0);

		}		
	}


}

/************************************************************************/
/* 当顶点落于叶子节点的包围盒里的时候，检查节点内的所有面片与切割器械线 */
/* 断的碰撞检测，做线和面的碰撞检测										*/
/************************************************************************/
void AabbTree::checkNode(ABNode* node)
{
	//如果此节点不是叶子节点，则退出检查，碰撞检测为空
	if(!node->isLeafNode())
		return;

	//检查叶子节点的唯一一个面片是否与直线相交
	int checkFace = node->faces_x[0];
	int size = 0;
	if(CheckFaceAndNode(checkFace))
	{
		//若相交，添加到碰撞检测的面片里面去
		Collision_face[0]++;
		size = Collision_face[0];
		Collision_face[size] = checkFace;
		temp_collide_node[size] = node;
	}
}

/************************************************************************/
/* 这是一个递归算法，从上至下遍历Aabb树                                 */
/************************************************************************/
void AabbTree::RootLeaf(ABNode* node)
{
	//当此节点为空，不予与检测
	if(node == NULL)
		return;

	//如果此节点已经是叶子节点
	if(node->isLeafNode())
	{
		//若碰撞点在包围盒中，直接检测点与其中面片的碰撞
		if(node->InBox(t_x1, t_y1, t_z1, t_x2, t_y2, t_z2))
			checkNode(node);
	}
	else
	{
		//如果碰撞点在这个节点的包围盒内，则继续检测它的左右节点
		if(node->InBox(t_x1, t_y1, t_z1, t_x2, t_y2, t_z2))
		{
			RootLeaf(node->left);
			RootLeaf(node->right);
		}
	}
}

/************************************************************************/
/* 这是一个递归算法，从下至上遍历Aabb树                                 */
/************************************************************************/
void AabbTree::LeafRoot(ABNode *node)
{
	//当此节点为空，不予与检测
	if(node == NULL)
		return;

	//如果碰撞检测线不在此节点的包围盒内部的话，向上遍历
	if(node->InBox(t_x1, t_y1, t_z1, t_x2, t_y2, t_z2))
		RootLeaf(node);
	else
		LeafRoot(node->parent);
}

/************************************************************************/
/* 判断面片与碰撞检测线段是否相交，输入为面片的序号		                */
/************************************************************************/
bool AabbTree::CheckFaceAndNode(int face_num)
{
	//根据面的法向量确定平面的方程
	float vx = objectStruct->f_normals[face_num * 3];
	float vy = objectStruct->f_normals[face_num * 3 + 1];
	float vz = objectStruct->f_normals[face_num * 3 + 2];

	//得到三角面片中所有点的位置坐标
	float ax, ay, az;
	int point_num = objectStruct->faces[face_num * 3];
	ax = objectStruct->points[point_num * 3];
	ay = objectStruct->points[point_num * 3 + 1];
	az = objectStruct->points[point_num * 3 + 2];


	//此时可知此三角面片的平面方程为vx*x + vy*y + vz*z + d = 0;
	float d = -vx * ax - vy * ay - vz * az;

	//算出两端点到平面的距离，如果符号相同，表示位于同测，必然不相交
	float d1 = vx * t_x1 + vy * t_y1 + vz * t_z1 + d;
	float d2 = vx * t_x2 + vy * t_y2 + vz * t_z2 + d;

	if(d1 * d2 > 0)
		return false;

	else
	{
		float bx, by, bz, cx, cy, cz;
		point_num = objectStruct->faces[face_num * 3 + 1];
		bx = objectStruct->points[point_num * 3];
		by = objectStruct->points[point_num * 3 + 1];
		bz = objectStruct->points[point_num * 3 + 2];
		point_num = objectStruct->faces[face_num * 3 + 2];
		cx = objectStruct->points[point_num * 3];
		cy = objectStruct->points[point_num * 3 + 1];
		cz = objectStruct->points[point_num * 3 + 2];

		//代表第几个碰撞面
		int collision_num = Collision_face[0];
		//算出直线与平面相交的点的位置
		float cpx = (d1 * t_x2 - d2 * t_x1)/(d1 - d2);
		float cpy = (d1 * t_y2 - d2 * t_y1)/(d1 - d2);
		float cpz = (d1 * t_z2 - d2 * t_z1)/(d1 - d2);

		//判断此点是否在三角形之内
		//过此点连接三角形的三个顶点，得到三个矢量
		//pa(ax - cpx, ay - cpy, az - cpz);
		//pb(bx - cpx, by - cpy, bz - cpz);
		//pc(cx - cpx, cy - cpy, cz - cpz);
		//当他们两两叉乘得到的结果均与面矢量方向相同时，点是在三角形内部的
		//将叉乘后的结果与面法向量分别相乘，看方向是否相符
		Vector3f pa, pb, pc, v;
		pa.setVector(ax - cpx, ay - cpy, az - cpz);
		pb.setVector(bx - cpx, by - cpy, bz - cpz);
		pc.setVector(cx - cpx, cy - cpy, cz - cpz);
		v.setVector(vx, vy, vz);

		if ( pa.Cross(pb)*v >= 0 && pb.Cross(pc) * v >= 0 && pc.Cross(pa) * v >= 0)
		{
			//设置碰撞顶点
			Collision_position[3 * collision_num] = cpx;
			Collision_position[3 * collision_num + 1] = cpy;
			Collision_position[3 * collision_num + 2] = cpz;

			return true;
		}

		return false;
	}
}

/************************************************************************/
/* 根据新创建的面片序号数组对模型的拓扑结构进行重构                     */
/************************************************************************/
void AabbTree::refreshTopoStruct(int* list1, int* list2)
{
	int size = list1[0];
	//更新所有面片的法向量
	for(int i = 0; i < size; i++)
		objectStruct->UpdateSingleFaceNorm(list1[i + 1]);

	int size2 = list2[0];
	for(int i = 0; i < size2; i++)
		objectStruct->UpdateSingleVertexNorm(list2[i + 1]);
}

/************************************************************************/
/* 在面片分裂后，原本的叶子节点分裂成了好几个面片，将此节点按里面的面片 */
/* 序号分裂，其中list为新生成的面片序号队列								*/
/************************************************************************/
void AabbTree::refreshCollisionTree(ABNode* node, int* list)
{
	node->setLeaf(false);
	int num = list[0];
	node->face_num = num;

	//重新设置节点中各个方向上的面片顺序
	delete node->faces_x, node->faces_y, node->faces_z;

	//新建三个方向上的数组
	float* range_x = new float[num];
	float* range_y = new float[num];
	float* range_z = new float[num];

	node->faces_x = new int[num];
	node->faces_y = new int[num];
	node->faces_z = new int[num];

	//当前的面片序号和顶点序号
	int temp_face_num, temp_point_num;										
	float x, y, z, temp_x, temp_y, temp_z;

	for(int i = 0; i < num; i++)
	{
		x = y = z = 0.0f;
		temp_face_num = list[i + 1];
		for(int j = 0; j < 3; j++)
		{
			temp_point_num = objectStruct->faces[temp_face_num * 3 + j];		//该顶点的序号
			temp_x = objectStruct->points[3 * temp_point_num];					//该顶点的x值
			temp_y = objectStruct->points[3 * temp_point_num + 1];
			temp_z = objectStruct->points[3 * temp_point_num + 2];

			//找出包围盒在x轴方向上的边界值
			if( temp_x < node->x1)
				node->x1 = temp_x;
			else if ( temp_x > node->x2)
				node->x2 = temp_x;

			//找出包围盒在y轴方向上的边界值
			if( temp_y < node->y1)
				node->y1 = temp_y;
			else if ( temp_y > node->y2)
				node->y2 = temp_y;

			//找出包围盒在z轴方向上的边界值
			if( temp_z < node->z1)
				node->z1 = temp_z;
			else if ( temp_z > node->z2)
				node->z2 = temp_z;

			//求个方向的顶点和
			x += temp_x;
			y += temp_y;
			z += temp_z;
		}

		//将平均值存入数组，作为面片的位置
		range_x[i] = objectStruct->face_Center[3 * temp_face_num]     = x / 3 ;
		range_y[i] = objectStruct->face_Center[3 * temp_face_num + 1] = y / 3;
		range_z[i] = objectStruct->face_Center[3 * temp_face_num + 2] = z / 3;
		node->faces_x[i] = node->faces_y[i] = node->faces_z[i] = temp_face_num;
	}

	//对三个方向的数组分别排序
	objectStruct->quickSort(node->faces_x, range_x, 0, num - 1);
	objectStruct->quickSort(node->faces_y, range_y, 0, num - 1);
	objectStruct->quickSort(node->faces_z, range_z, 0, num - 1);

	splitNode(node);

}

/************************************************************************/
/* 根据新的顶点的位置更新整棵碰撞检测书的包围盒结构                     */
/************************************************************************/
void AabbTree::UpdateBoundaries()
{
	//采用后续遍历的方法遍历整棵树
	updateSingleBox(this->root);

}

/************************************************************************/
/* 根据左右子节点的包围盒或者自己包含的面片更新当前顶点的包围盒范围     */
/************************************************************************/
void AabbTree::updateSingleBox(ABNode* node)
{
	//如果当前节点为空，则出现错误
	if(node == NULL)
		return;

	//如果当前节点是一个叶子节点，根据其包含面片的三个顶点确定其包围盒范围
	if(node->isLeafNode())
	{
		int tmp_face = node->faces_x[0];
		int tmp_pt;
		for(int j = 0; j < 3; j++)
		{
			tmp_pt = objectStruct->faces[3 * tmp_face + j];
			node->FixBox(objectStruct->points[3 * tmp_pt], objectStruct->points[3 * tmp_pt + 1], objectStruct->points[3 * tmp_pt + 2]);
		}
	}
	//否则分别更新其左右节点的包围盒，再根据左右节点的包围盒来确定当前包围盒的大小
	else //@ 后序遍历，即 最后遍历根节点
	{
		updateSingleBox(node->left);
		updateSingleBox(node->right);
		//根据左右包围盒更新当前包围盒的范围
		node->x1 = node->left->x1 < node->right->x1 ? node->left->x1:node->right->x1;
		node->x2 = node->left->x2 > node->right->x2 ? node->left->x2:node->right->x2;
		node->y1 = node->left->y1 < node->right->y1 ? node->left->y1:node->right->y1;
		node->y2 = node->left->y2 > node->right->y2 ? node->left->y2:node->right->y2;
		node->z1 = node->left->z1 < node->right->z1 ? node->left->z1:node->right->z1;
		node->z2 = node->left->z2 > node->right->z2 ? node->left->z2:node->right->z2;
	}

}