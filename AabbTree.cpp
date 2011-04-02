#include "AabbTree.h"



AabbTree::AabbTree(void)
{
	root = NULL;

	//����⵽����Ƭ�ŵ��������У�һ�������ͬʱ�и����Ƭ�������10
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

//ͨ���������ģ�����ݹ�����
AabbTree::AabbTree(BasicStruct* related)
{
	root = NULL;

	//����⵽����Ƭ�ŵ��������У�һ�������ͬʱ�и����Ƭ�������10
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

	//@	this->objectStruct = related; //@ ����Aabb�ṹ�Ķ���ṹ�����յ���Ķ�������

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
/* ͨ����֪��ģ�����ݹ��������ײ�����                                 */
/************************************************************************/
void AabbTree::setRelatedMesh(BasicStruct* related)
{
	//���������ģ��ָ���µ�ģ��,��ԭ�ȵ���ײ�����ɾ��
	this->objectStruct = related;
	if(root!=NULL)
		delete this->root;

	related->RangeFaces();
	//����Aabb��
	root = new ABNode();
	int num = objectStruct->face_num;
	root->face_num = num;

	//�������ڵ�İ�Χ��
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

	//���Ѹ��ڵ�
	this->splitNode(root);
	//cout<<"�����������������ѵ���ʱ�䣺 " << Timer->costTime << "s" << endl;
	//cout<< "�ҵ��߽�ֵ�����ѵ���ʱ�䣺"<< timer1->costTime <<"s"<<endl;
	//cout<<"������һ�����������ѵ�ʱ�䣺" << timer2->costTime << "s" <<endl;
	//Timer->Reset();
	//timer1->Reset();
	//timer2->Reset();
}

/************************************************************************/
/* ���ڵ���ѳ����������ӽڵ㣬����Χ�еı䳤��С��һ��ֵ������ϸ��   */
/************************************************************************/
void AabbTree::splitNode(ABNode* node)
{
	if(node->face_num == 1)
	{
		//����ýڵ�ֻ��Χ��һ����Ƭ
		node->setLeaf(true);
		//����Ҫ����Ҷ�ӽڵ�
		return;
	}

	//���������Ϊ�������ڵ㣬����������֮��Ĺ�ϵ
	ABNode* left = new ABNode();
	ABNode* right = new ABNode();
	node->left = left;
	node->right = right;
	left->parent = node;
	right->parent = node;

	//Ϊ����һ��ƽ�������ҳ����ڵ��бȽϳ���һ���߽��з���
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

	//�������������ڵ�ĸ��ֲ���
	left->faces_x = left_faces_x;
	left->faces_y = left_faces_y;
	left->faces_z = left_faces_z;

	right->faces_x = right_faces_x;
	right->faces_y = right_faces_y;
	right->faces_z = right_faces_z;

	//���x���ϵ�span���������������ϵ�ʱ�򣬷���x��
	if(x >= y && x >= z)
	{
		//�������з���ļ����ڲ�����
		for(int i = 0; i < left->face_num; i++)
			left_faces_x[i] = node->faces_x[i];

		for(int i = 0; i < right->face_num; i++)
			right_faces_x[i] = node->faces_x[i + left->face_num];

		//������������һ�����Ʊ�ʶ��
		int n1, n2;
		bool inLeft;
		float max_x = objectStruct->face_Center[3 * left_faces_x[left->face_num - 1]];
		float min_x = objectStruct->face_Center[3 * right_faces_x[0]];

		//����y�������
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_y[i];
			float temp_x = objectStruct->face_Center[3 * temp_face];
			//�������Ƭ���м�λ�õ�x���������߽ڵ����Сֵ���������߽ڵ��y����
			if(min_x == max_x && temp_x == max_x)
			{						
				inLeft = false;
				//����left_face_x����temp_face�ڲ������棬���ڣ������left_face_y��
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

				//�������������ұߵ�x�����������ŵ���ߵ���������
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

			//�����ֵС���ұ߽ڵ�����ֵ����ŵ��б߽����
			else
			{
				right_faces_y[n2] = temp_face;
				n2++;
			}
		}

		//����z�������
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_z[i];
			float temp_x = objectStruct->face_Center[3 * temp_face];
			//�������Ƭ���м�λ�õ�x���������߽ڵ����Сֵ���������߽ڵ��y����
			if(min_x == max_x && temp_x == max_x)
			{
				inLeft = false;
				//����left_face_x����temp_face�ڲ������棬���ڣ������left_face_y��
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

				//�������������ұߵ�x�����������ŵ���ߵ���������
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

			//�����ֵС���ұ߽ڵ�����ֵ����ŵ��б߽����
			else
			{
				right_faces_z[n2] = node->faces_z[i];
				n2++;
			}
		}
	}

	//����y��
	else if ( y >= x && y >= z)
	{
		//�������з���ļ����ڲ�����
		for(int i = 0; i < left->face_num; i++)
			left_faces_y[i] = node->faces_y[i];

		for(int i = 0; i < right->face_num; i++)
			right_faces_y[i] = node->faces_y[i + left->face_num];

		//������������һ�����Ʊ�ʶ��
		int n1, n2;
		bool inLeft;
		float max_y = objectStruct->face_Center[3 * left_faces_y[left->face_num - 1] + 1];
		float min_y = objectStruct->face_Center[3 * right_faces_y[0] + 1];

		//����x�������
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_x[i];
			float temp_y = objectStruct->face_Center[3 * temp_face + 1];
			//�������Ƭ���м�λ�õ�x���������߽ڵ����Сֵ���������߽ڵ��y����
			if(min_y == max_y && temp_y == max_y)
			{						
				inLeft = false;
				//����left_face_x����temp_face�ڲ������棬���ڣ������left_face_y��
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

				//�������������ұߵ�x�����������ŵ���ߵ���������
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

			//�����ֵС���ұ߽ڵ�����ֵ����ŵ��б߽����
			else
			{
				right_faces_x[n2] = temp_face;
				n2++;
			}
		}

		//����z�������
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_z[i];
			float temp_y = objectStruct->face_Center[3 * temp_face + 1];
			//�������Ƭ���м�λ�õ�x���������߽ڵ����Сֵ���������߽ڵ��y����
			if(min_y == max_y && temp_y == max_y)
			{
				inLeft = false;
				//����left_face_x����temp_face�ڲ������棬���ڣ������left_face_y��
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

				//�������������ұߵ�x�����������ŵ���ߵ���������
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

			//�����ֵС���ұ߽ڵ�����ֵ����ŵ��б߽����
			else
			{
				right_faces_z[n2] = temp_face;
				n2++;
			}
		}
	}

	//����z��
	else
	{
		//�������з���ļ����ڲ�����
		for(int i = 0; i < left->face_num; i++)
			left_faces_z[i] = node->faces_z[i];

		for(int i = 0; i < right->face_num; i++)
			right_faces_z[i] = node->faces_z[i + left->face_num];

		//������������һ�����Ʊ�ʶ��
		int n1, n2;
		bool inLeft;
		float max_z = objectStruct->face_Center[3 * left_faces_z[left->face_num - 1] + 2];
		float min_z = objectStruct->face_Center[3 * right_faces_z[0] + 2];

		//����y�������
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_y[i];
			float temp_z = objectStruct->face_Center[3 * temp_face + 2];
			//�������Ƭ���м�λ�õ�x���������߽ڵ����Сֵ���������߽ڵ��y����
			if(min_z == max_z && temp_z == max_z)
			{						
				inLeft = false;
				//����left_face_x����temp_face�ڲ������棬���ڣ������left_face_y��
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

				//�������������ұߵ�x�����������ŵ���ߵ���������
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

			//�����ֵС���ұ߽ڵ�����ֵ����ŵ��б߽����
			else
			{
				right_faces_y[n2] = temp_face;
				n2++;
			}
		}

		//����x�������
		n1 = n2 = 0;
		for(int i = 0; i < num; i++)
		{
			int temp_face = node->faces_x[i];
			float temp_z = objectStruct->face_Center[3 * temp_face + 2];
			//�������Ƭ���м�λ�õ�x���������߽ڵ����Сֵ���������߽ڵ��y����
			if(min_z == max_z && temp_z == max_z)
			{
				inLeft = false;
				//����left_face_z����temp_face�ڲ������棬���ڣ������left_face_x��
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

				//�������������ұߵ�x�����������ŵ���ߵ���������
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

			//�����ֵС���ұ߽ڵ�����ֵ����ŵ��б߽����
			else
			{
				right_faces_x[n2] = temp_face;
				n2++;
			}
		}
	}

	//������С��Χ�еĴ�С
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
/* ������ײ�����λ��(x1,y1,z1),��(x2,y2,z2)�ھ������ʱ����õ�    */
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
/* ��������Aabb���������һ��ʱ���Ѿ�������ĳ����Ƭ��������Ҷ�ӽڵ㿪 */
/* ʼ������������Ӹ��ڵ㿪ʼ����                                       */
/************************************************************************/
void AabbTree::CollisionDetect()
{
	//������ײ�����Ƭ������	
	int collide_num = this->Collision_face[0];
	Collision_face[0] = 0;

	//this is for test
	//timer1->Start();
	//����һ��״̬�м�⵽��ײ��ʱ�򣬴���һ��λ�õ�Ҷ�ӽڵ㿪ʼ����
	//if(collide_num != 0)
	//{
	//	for(int i = 1; i<= collide_num; i++)
	//		LeafRoot(temp_collide_node[i]);
	//}
	////�Ӹ��ڵ㿪ʼ����
	//else
	RootLeaf(root);

	//�������ײ�Ľڵ�����ָ��temp_collide_node
	for(int i = 1; i <= this->Collision_face[0]; i++)
	{
		last_collide_node[i] = temp_collide_node[i];


	}

	if(this->Collision_face[0]>0)
	{
		int i = 1;
		//this is a trial
		//�����з�����ײ������Ƭȫ�������෽���ƽ�
		int tmp_face = Collision_face[i];
		Vector3f* collide_p1 = new Vector3f(t_x1 - Collision_position[3*i-3], t_y1 - Collision_position[3*i-2], t_z1 - Collision_position[3*i-1]);
		Vector3f* collide_p2 = new Vector3f(t_x2 - Collision_position[3*i-3], t_y2 - Collision_position[3*i-2], t_z2 - Collision_position[3*i-1]);
		Vector3f* up_Vector = new Vector3f(this->objectStruct->f_normals[3 * tmp_face],this->objectStruct->f_normals[3 * tmp_face+1],this->objectStruct->f_normals[3 * tmp_face+2]);
		//�̵���ͷ��Ϊ����Ƭ��ƽ�Ʒ���
		Vector3f* move = collide_p1;
		if((*collide_p2) * (*up_Vector) < 0)
			move = collide_p2;

		int tmp_point;
		//����Ƭ��ص���������
		for(int j = 0; j < 3; j++)
		{
			tmp_point = this->objectStruct->faces[tmp_face * 3 + j];
			this->objectStruct->points[tmp_point * 3] += move.x;
			this->objectStruct->points[tmp_point * 3 + 1] += move.y;
			this->objectStruct->points[tmp_point * 3 + 2] += move.z;

			//this->objectStruct->points[tmp_point * 3] = (t_x2 + t_x1)/2;
			//this->objectStruct->points[tmp_point * 3 + 1] = (t_y2 + t_y1)/2;
			//this->objectStruct->points[tmp_point * 3 + 2] = (t_z2 + t_z1)/2;

			//�˵���ٶ�����Ϊ0
			(*this->objectStruct->v_speed[tmp_point]).setVector(0, 0, 0);

		}		
	}


}

/************************************************************************/
/* ����������Ҷ�ӽڵ�İ�Χ�����ʱ�򣬼��ڵ��ڵ�������Ƭ���и���е�� */
/* �ϵ���ײ��⣬���ߺ������ײ���										*/
/************************************************************************/
void AabbTree::checkNode(ABNode* node)
{
	//����˽ڵ㲻��Ҷ�ӽڵ㣬���˳���飬��ײ���Ϊ��
	if(!node->isLeafNode())
		return;

	//���Ҷ�ӽڵ��Ψһһ����Ƭ�Ƿ���ֱ���ཻ
	int checkFace = node->faces_x[0];
	int size = 0;
	if(CheckFaceAndNode(checkFace))
	{
		//���ཻ����ӵ���ײ������Ƭ����ȥ
		Collision_face[0]++;
		size = Collision_face[0];
		Collision_face[size] = checkFace;
		temp_collide_node[size] = node;
	}
}

/************************************************************************/
/* ����һ���ݹ��㷨���������±���Aabb��                                 */
/************************************************************************/
void AabbTree::RootLeaf(ABNode* node)
{
	//���˽ڵ�Ϊ�գ���������
	if(node == NULL)
		return;

	//����˽ڵ��Ѿ���Ҷ�ӽڵ�
	if(node->isLeafNode())
	{
		//����ײ���ڰ�Χ���У�ֱ�Ӽ�����������Ƭ����ײ
		if(node->InBox(t_x1, t_y1, t_z1, t_x2, t_y2, t_z2))
			checkNode(node);
	}
	else
	{
		//�����ײ��������ڵ�İ�Χ���ڣ����������������ҽڵ�
		if(node->InBox(t_x1, t_y1, t_z1, t_x2, t_y2, t_z2))
		{
			RootLeaf(node->left);
			RootLeaf(node->right);
		}
	}
}

/************************************************************************/
/* ����һ���ݹ��㷨���������ϱ���Aabb��                                 */
/************************************************************************/
void AabbTree::LeafRoot(ABNode *node)
{
	//���˽ڵ�Ϊ�գ���������
	if(node == NULL)
		return;

	//�����ײ����߲��ڴ˽ڵ�İ�Χ���ڲ��Ļ������ϱ���
	if(node->InBox(t_x1, t_y1, t_z1, t_x2, t_y2, t_z2))
		RootLeaf(node);
	else
		LeafRoot(node->parent);
}

/************************************************************************/
/* �ж���Ƭ����ײ����߶��Ƿ��ཻ������Ϊ��Ƭ�����		                */
/************************************************************************/
bool AabbTree::CheckFaceAndNode(int face_num)
{
	//������ķ�����ȷ��ƽ��ķ���
	float vx = objectStruct->f_normals[face_num * 3];
	float vy = objectStruct->f_normals[face_num * 3 + 1];
	float vz = objectStruct->f_normals[face_num * 3 + 2];

	//�õ�������Ƭ�����е��λ������
	float ax, ay, az;
	int point_num = objectStruct->faces[face_num * 3];
	ax = objectStruct->points[point_num * 3];
	ay = objectStruct->points[point_num * 3 + 1];
	az = objectStruct->points[point_num * 3 + 2];


	//��ʱ��֪��������Ƭ��ƽ�淽��Ϊvx*x + vy*y + vz*z + d = 0;
	float d = -vx * ax - vy * ay - vz * az;

	//������˵㵽ƽ��ľ��룬���������ͬ����ʾλ��ͬ�⣬��Ȼ���ཻ
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

		//����ڼ�����ײ��
		int collision_num = Collision_face[0];
		//���ֱ����ƽ���ཻ�ĵ��λ��
		float cpx = (d1 * t_x2 - d2 * t_x1)/(d1 - d2);
		float cpy = (d1 * t_y2 - d2 * t_y1)/(d1 - d2);
		float cpz = (d1 * t_z2 - d2 * t_z1)/(d1 - d2);

		//�жϴ˵��Ƿ���������֮��
		//���˵����������ε��������㣬�õ�����ʸ��
		//pa(ax - cpx, ay - cpy, az - cpz);
		//pb(bx - cpx, by - cpy, bz - cpz);
		//pc(cx - cpx, cy - cpy, cz - cpz);
		//������������˵õ��Ľ��������ʸ��������ͬʱ���������������ڲ���
		//����˺�Ľ�����淨�����ֱ���ˣ��������Ƿ����
		Vector3f pa, pb, pc, v;
		pa.setVector(ax - cpx, ay - cpy, az - cpz);
		pb.setVector(bx - cpx, by - cpy, bz - cpz);
		pc.setVector(cx - cpx, cy - cpy, cz - cpz);
		v.setVector(vx, vy, vz);

		if ( pa.Cross(pb)*v >= 0 && pb.Cross(pc) * v >= 0 && pc.Cross(pa) * v >= 0)
		{
			//������ײ����
			Collision_position[3 * collision_num] = cpx;
			Collision_position[3 * collision_num + 1] = cpy;
			Collision_position[3 * collision_num + 2] = cpz;

			return true;
		}

		return false;
	}
}

/************************************************************************/
/* �����´�������Ƭ��������ģ�͵����˽ṹ�����ع�                     */
/************************************************************************/
void AabbTree::refreshTopoStruct(int* list1, int* list2)
{
	int size = list1[0];
	//����������Ƭ�ķ�����
	for(int i = 0; i < size; i++)
		objectStruct->UpdateSingleFaceNorm(list1[i + 1]);

	int size2 = list2[0];
	for(int i = 0; i < size2; i++)
		objectStruct->UpdateSingleVertexNorm(list2[i + 1]);
}

/************************************************************************/
/* ����Ƭ���Ѻ�ԭ����Ҷ�ӽڵ���ѳ��˺ü�����Ƭ�����˽ڵ㰴�������Ƭ */
/* ��ŷ��ѣ�����listΪ�����ɵ���Ƭ��Ŷ���								*/
/************************************************************************/
void AabbTree::refreshCollisionTree(ABNode* node, int* list)
{
	node->setLeaf(false);
	int num = list[0];
	node->face_num = num;

	//�������ýڵ��и��������ϵ���Ƭ˳��
	delete node->faces_x, node->faces_y, node->faces_z;

	//�½����������ϵ�����
	float* range_x = new float[num];
	float* range_y = new float[num];
	float* range_z = new float[num];

	node->faces_x = new int[num];
	node->faces_y = new int[num];
	node->faces_z = new int[num];

	//��ǰ����Ƭ��źͶ������
	int temp_face_num, temp_point_num;										
	float x, y, z, temp_x, temp_y, temp_z;

	for(int i = 0; i < num; i++)
	{
		x = y = z = 0.0f;
		temp_face_num = list[i + 1];
		for(int j = 0; j < 3; j++)
		{
			temp_point_num = objectStruct->faces[temp_face_num * 3 + j];		//�ö�������
			temp_x = objectStruct->points[3 * temp_point_num];					//�ö����xֵ
			temp_y = objectStruct->points[3 * temp_point_num + 1];
			temp_z = objectStruct->points[3 * temp_point_num + 2];

			//�ҳ���Χ����x�᷽���ϵı߽�ֵ
			if( temp_x < node->x1)
				node->x1 = temp_x;
			else if ( temp_x > node->x2)
				node->x2 = temp_x;

			//�ҳ���Χ����y�᷽���ϵı߽�ֵ
			if( temp_y < node->y1)
				node->y1 = temp_y;
			else if ( temp_y > node->y2)
				node->y2 = temp_y;

			//�ҳ���Χ����z�᷽���ϵı߽�ֵ
			if( temp_z < node->z1)
				node->z1 = temp_z;
			else if ( temp_z > node->z2)
				node->z2 = temp_z;

			//�������Ķ����
			x += temp_x;
			y += temp_y;
			z += temp_z;
		}

		//��ƽ��ֵ�������飬��Ϊ��Ƭ��λ��
		range_x[i] = objectStruct->face_Center[3 * temp_face_num]     = x / 3 ;
		range_y[i] = objectStruct->face_Center[3 * temp_face_num + 1] = y / 3;
		range_z[i] = objectStruct->face_Center[3 * temp_face_num + 2] = z / 3;
		node->faces_x[i] = node->faces_y[i] = node->faces_z[i] = temp_face_num;
	}

	//���������������ֱ�����
	objectStruct->quickSort(node->faces_x, range_x, 0, num - 1);
	objectStruct->quickSort(node->faces_y, range_y, 0, num - 1);
	objectStruct->quickSort(node->faces_z, range_z, 0, num - 1);

	splitNode(node);

}

/************************************************************************/
/* �����µĶ����λ�ø���������ײ�����İ�Χ�нṹ                     */
/************************************************************************/
void AabbTree::UpdateBoundaries()
{
	//���ú��������ķ�������������
	updateSingleBox(this->root);

}

/************************************************************************/
/* ���������ӽڵ�İ�Χ�л����Լ���������Ƭ���µ�ǰ����İ�Χ�з�Χ     */
/************************************************************************/
void AabbTree::updateSingleBox(ABNode* node)
{
	//�����ǰ�ڵ�Ϊ�գ�����ִ���
	if(node == NULL)
		return;

	//�����ǰ�ڵ���һ��Ҷ�ӽڵ㣬�����������Ƭ����������ȷ�����Χ�з�Χ
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
	//����ֱ���������ҽڵ�İ�Χ�У��ٸ������ҽڵ�İ�Χ����ȷ����ǰ��Χ�еĴ�С
	else //@ ����������� ���������ڵ�
	{
		updateSingleBox(node->left);
		updateSingleBox(node->right);
		//�������Ұ�Χ�и��µ�ǰ��Χ�еķ�Χ
		node->x1 = node->left->x1 < node->right->x1 ? node->left->x1:node->right->x1;
		node->x2 = node->left->x2 > node->right->x2 ? node->left->x2:node->right->x2;
		node->y1 = node->left->y1 < node->right->y1 ? node->left->y1:node->right->y1;
		node->y2 = node->left->y2 > node->right->y2 ? node->left->y2:node->right->y2;
		node->z1 = node->left->z1 < node->right->z1 ? node->left->z1:node->right->z1;
		node->z2 = node->left->z2 > node->right->z2 ? node->left->z2:node->right->z2;
	}

}