#include "BasicStruct.h"

BasicStruct::BasicStruct(void)
{
	point_num = face_num = 0;
	min_x = min_y = min_z = max_x = max_y = max_z = 0.0f;
	points = orig_points = NULL;
	v_speed = v_force = NULL;
	f_normals = v_normals = NULL;
	faces = range_x = range_y = range_z = NULL;
	face_Center = face_range_x = face_range_y = face_range_z = NULL;
	conns_vertex_face = conns_vertex_edge = NULL;
	highlight_face = NULL; //this is for test
	highlight_edge = NULL; //this is for test
	flag = false;
	k1 = 35.0, k2 = 40.0;
	time_step = 0.001;
	v_m = 0.2;
}

BasicStruct::~BasicStruct(void)
{
	delete points, orig_points, v_speed;
	delete faces;
	delete f_normals;
	delete v_normals;
	delete v_force, v_speed;
	delete conns_vertex_face, conns_vertex_edge;
	delete face_Center, face_range_x, face_range_y, face_range_z;
	delete range_x, range_y, range_z;
}

/************************************************************************/
/* 通过读取文件读入模型，如果返回true，则文件读取成功，false则不成功    */
/************************************************************************/
bool BasicStruct::CreateMeshFromFile(char* filename, float scale, float x, float y, float z)
{
	//得到文件后缀名
	string type = strrchr(filename, '.');
	//如果文件格式不是以下四种，返回空
	if(!(type == ".3dsa" || type == ".vtka" || type == ".plya" || type == ".bod" || type == ".bd" || type == ".roda"))
		return false;

	ifstream readin( filename, ios::in );
	if( !readin )
		return false;

	//读取顶点数及面片数
	int pn, fn;
	readin >> pn >> fn;

	point_num = pn;
	face_num = fn;

	//初始化定点数组及法向量数组
	orig_points = new float[point_num * 3 * 6]; //@ 为何要*6呢？删去后依然可以跑起来啊
	points = new float[ point_num * 3     * 6];
	faces = new int[ face_num * 3         * 6];
	f_normals = new float[ face_num * 3   * 6];
	v_normals = new float[ point_num * 3  * 6];

	//初始化所有顶点的速度场，初始速度为0
	v_speed = new Vector3f*[point_num * 3     * 6]; //@ Vector3f的default constructor初始化x,y,z为0
	v_force = new Vector3f*[point_num * 3 * 6];

	for(int i = 0; i < point_num*3; i++)
	{
		v_speed[i] = new Vector3f();
		v_force[i] = new Vector3f();
	}


	int i, idx;
	char title[3];
	float temp1, temp2, temp3;


	//读取后缀名为".3dsa"格式的文件
	if( type == ".3dsa" )
	{
		//读入所有顶点信息
		for( i = 0; i < point_num; i++ )
		{
			readin >> idx >> temp1 >> temp2 >> temp3;
			orig_points[i*3] = points[i*3] = temp1 * scale + x;
			orig_points[i*3+1] = points[i*3 + 1] = temp2 * scale + y;
			orig_points[i*3+2] = points[i*3 + 2] = temp3 * scale + z;
		}

		//读入所有面片信息
		for( i = 0; i < face_num; i++ )
		{
			readin >> idx >> title >> faces[ i*3 ] >> title >> faces[ i*3+1 ] >> title >> faces[ i*3+2 ];
			readin >> title >> idx >> title >> idx >> title >> idx;
		}	
	}

	//读取后缀名为".vtka"的文件
	else if( type == ".vtka" )
	{	
		//读取所有顶点
		for( i = 0; i < point_num; i++ )
		{
			readin >> temp1 >> temp2 >> temp3;
			orig_points[i*3] = points[i*3] = temp1 * scale + x;
			orig_points[i*3+1] = points[i*3 + 1] = temp2 * scale + y;
			orig_points[i*3+2] = points[i*3 + 2] = temp3 * scale + z;
		}

		//读取所有面片
		for( i = 0; i < face_num; i++ )
			readin >> idx >> faces[ i*3 ] >> faces[ i*3+1 ] >> faces[ i*3+2 ];
	}

	//读取后缀名为".plya"的文件
	else if( type == ".plya" )
	{
		float temp4,temp5;
		for( i = 0; i < point_num; i++ )
		{
			readin  >> temp1 >> temp2 >> temp3 >> temp4 >> temp5;
			orig_points[i*3] = points[i*3] = temp1 * scale + x;
			orig_points[i*3+1] = points[i*3 + 1] = temp2 * scale + y;
			orig_points[i*3+2] = points[i*3 + 2] = temp3 * scale + z;
		}

		for( i = 0; i < face_num; i++ )
			readin >> idx >> faces[ i*3 ] >> faces[ i*3+1 ] >> faces[ i*3+2 ];
	}

	//读取后缀名为".bd"的文件
	else if( type == ".bd" || type == ".bod")
	{
		//读入所有顶点信息
		for( i = 0; i < point_num; i++ )
		{
			readin >> temp1 >> temp2 >> temp3;
			orig_points[i*3] = points[i*3] = temp1 * scale + x;
			orig_points[i*3+1] = points[i*3 + 1] = temp2 * scale + y;
			orig_points[i*3+2] = points[i*3 + 2] = temp3 * scale + z;
		}

		for( i = 0; i < face_num; i++ )
			readin >> faces[ i*3 ] >> faces[ i*3+1 ] >> faces[ i*3+2 ];	
	}
	readin.close();


	//计算模型的拓扑结构
	this->CreateTopoStruct();
	//得到所有法向量
	this->UpdateNormals();
	//将所有的面片按位置排序
	this->RangeFaces();

	flag = true;
	return true;
}

/************************************************************************/
/* 更新模型的店面法向量											        */
/************************************************************************/
void BasicStruct::UpdateNormals(void)
{
	UpdateFaceNorms();			//更新面片法向量
	UpdateVertexNorms();		//更新顶点法向量
}

/************************************************************************/
/* 根据面片绘制网格模型                                                 */
/************************************************************************/
void BasicStruct::DrawMesh()//(int draw_mode)
{
	if(!flag)
		return;	

	//确定多边形的正面是如何决定的。在此设为逆时针方向
	glFrontFace(GL_CCW);

	//按照一个个三角面片画模型
	int n = 0;

	////选择线框模式
	//if(draw_mode == LINE_MODE)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	////选择实体模式
	//else if(draw_mode == FILL_MODE)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	glLineWidth(1.0f);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i< face_num; i++)
	{
		//画第i个三角面片的三个顶点
		n = (faces[i*3]) * 3;				//第一个顶点的第一个位置坐标
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[i*3 + 1]) * 3;			//第二个顶点的第一个位置坐标
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[i*3 + 2]) * 3;			//第一个顶点的第一个位置坐标
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);
	}
	glEnd();


	//glPopMatrix();

	//////////////////////////////////////////////////////////////////////////
	//this is for test
	//绘制那些碰撞检测相交的面
	//////////////////////////////////////////////////////////////////////////
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	if(highlight_face == NULL)
		return;
	for(int i = 0; i< this->highlight_face[0]; i++)
	{
		//画第i个三角面片的三个顶点
		n = (faces[highlight_face[i+1]*3]) * 3;				//第一个顶点的第一个位置坐标
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[highlight_face[i+1]*3 + 1]) * 3;			//第二个顶点的第一个位置坐标
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[highlight_face[i+1]*3 + 2]) * 3;			//第一个顶点的第一个位置坐标
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);
	}
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////////////////

}

/************************************************************************/
/* 根据三个顶点的位置计算单个面片的法向量                               */
/************************************************************************/
void BasicStruct::UpdateSingleFaceNorm(int faceNum)
{
	Vector3f v1, v2, v3, norm;
	int n1, n2, n3;

	n1 = faces[faceNum * 3];		//第一个顶点的次序
	n2 = faces[faceNum * 3 + 1];	//第二个顶点的次序
	n3 = faces[faceNum * 3 + 2];	//第三个顶点的次序

	v1.setVector(points[3*n1], points[3*n1 + 1],points[3*n1 + 2]);
	v2.setVector(points[3*n2], points[3*n2 + 1],points[3*n2 + 2]);
	v3.setVector(points[3*n3], points[3*n3 + 1],points[3*n3 + 2]);
	norm = (v2 - v1).Cross(v3 - v1).Normalize();

	f_normals[3 * faceNum] = norm.x;
	f_normals[3 * faceNum + 1] = norm.y;
	f_normals[3 * faceNum + 2] = norm.z;
}

/************************************************************************/
/* 根据相邻的面的法向量更新单个顶点的法向量                             */
/************************************************************************/
void BasicStruct::UpdateSingleVertexNorm(int pointNum)
{
	Vector3f norm;
	float x, y, z;
	int n, size;

	size = conns_vertex_face[pointNum][0];		//得到与此顶点相关的面有多少个
	x = y = z = 0.0f;
	for(int j = 1; j < size + 1; j++)
	{
		n = conns_vertex_face[pointNum][j];
		x += f_normals[n*3];
		y += f_normals[n*3 + 1];
		z += f_normals[n*3 + 2];
	}

	norm.setVector(x, y, z);
	norm = norm.Normalize();

	v_normals[pointNum * 3]		= norm.x;
	v_normals[pointNum * 3 + 1]	= norm.y;
	v_normals[pointNum * 3 + 2]	= norm.z;
}

/************************************************************************/
/* 根据每个面片的顶点位置计算每个面片的法向量                           */
/************************************************************************/
void BasicStruct::UpdateFaceNorms(void)
{
	for(int i = 0; i< this->face_num; i++)
		UpdateSingleFaceNorm(i);
}

/************************************************************************/
/* 根据与点相邻的所有面片得到点的法向量                                 */
/************************************************************************/
void BasicStruct::UpdateVertexNorms(void)
{
	for(int i = 0; i < this->point_num; i++)
		UpdateSingleVertexNorm(i);
}

/************************************************************************/
/* 根据每个顶点的当前速度以及受力情况更新所有顶点的位置以及速度         */
/************************************************************************/
void BasicStruct::UpdatePoints()
{	
	//加个重力试试	
	Vector3f tmp_vector;
	float plus_result;			//弹簧力与运动方向的点乘

	int times = 0;

	while(times < 20) //@ 计算20次，增加稳定性
	{

		for(int i = 0; i < point_num; i++)
		{
			//加上重力
			(*v_force[i]).setVector(0, v_m * 9.8, 0);

			//根据牛顿第二定律计算核结构弹簧上的弹力,其中k2为核弹簧的弹性系数
			tmp_vector.setVector((orig_points[3*i]-points[3*i])*k2,(orig_points[3*i+1]-points[3*i+1])*k2,(orig_points[3*i+2]-points[3*i+2])*k2);

			//如果此时的弹力与质点的运动方向小于90°角，认为方向相同，阻尼力与之相反；反之阻尼力与之相同。
			plus_result = tmp_vector * (*v_speed[i]);

			//plus_result = (tmp_vector.Normalize()) * ((*v_speed[i]).Normalize());
			//方向相同
			if(plus_result > 0.1)
				tmp_vector = tmp_vector*0.8;		//这两个系数的调节至为关键
			//方向不同时
			else if(plus_result < -0.1)
				tmp_vector = tmp_vector*1.2;

			(*v_force[i]) += tmp_vector;

		}

		//遍历所有的边，更新所有与边相关的点的受力情况
		int edge_num = edges->size()/3;
		Vector3f current_edge, edge_force;
		int v1,v2;		//连接当前边的两个节点的序号
		float dl;		//边的变化量
		for(int i = 0; i < edge_num; i++)
		{
			v1 = (*edges)[3 * i];
			v2 = (*edges)[3 * i + 1];
			current_edge.setVector(points[3*v1]-points[3*v2],points[3*v1+1]-points[3*v2+1],points[3*v1+2]-points[3*v2+2]);
			dl = current_edge.getLength() - (*edges)[3 * i + 2];
			//若变化量很小，则对所有的顶点不影响
			if(dl < 0.001 && dl > -0.001)
				continue;

			//否则在v1v2上分别施加力，计算边上的力
			edge_force = current_edge.setLength(dl * k1);

			//同样在这根弹簧上，同样需要加上一个阻尼力
			plus_result = edge_force * (*v_speed[v1]);
			if(plus_result > 0.1)
				(*v_force[v1]) -= (edge_force * 1.2);
			else if (plus_result < -0.1)
				(*v_force[v1]) -= (edge_force * 0.8);
			else
				(*v_force[v1]) -= edge_force;

			plus_result = edge_force * (*v_speed[v2]);
			if(plus_result > 0.1)
				(*v_force[v2]) += (edge_force * 0.8);
			else if (plus_result < -0.1)
				(*v_force[v2]) += (edge_force * 1.2);
			else
				(*v_force[v2]) += edge_force;
		}


		//遍历所有顶点，根据点的受力情况更新其位置及速度
		for(int i = 0; i < point_num; i++)
		{
			////当一个点速度很小的在原始位置徘徊时，将其设置为0
			//if((*v_force[i]).getLength() < 0.001 && (*v_speed[i]).getLength() < 0.01)
			//{
			//	(*v_force[i]).setVector(0, 0, 0);
			//	(*v_speed[i]).setVector(0, 0, 0);
			//	points[3 * i] = orig_points[3 * i];
			//	points[3 * i + 1] = orig_points[3 * i];
			//	points[3 * i + 2] = orig_points[3 * i];

			//}

			(*v_speed[i]) += (*v_force[i]) * (time_step / v_m);
			Vector3f ds = (*v_speed[i]) * time_step;

			points[3 * i] += ds.x;
			points[3 * i + 1] += ds.y;
			points[3 * i + 2] += ds.z;
		}

		times++;

	}

	//根据当前的面片更新法向量，好像没什么改变，那就不更新了
	this->UpdateNormals();

}

/************************************************************************/
/* 建立模型的拓扑结构                                                   */
/************************************************************************/
void BasicStruct::CreateTopoStruct(void)
{
	//建立点与面的关系
	conns_vertex_face = new int*[point_num * 6];
	//建立点与点的关系，每个关联点用两位储存，第一位表示关联点的序号，第二位表示连接它们的边的序号
	conns_vertex_edge = new int*[point_num * 12];
	//根据欧拉定理，一般来说e=f+v-2就够了；
	edges = new vector<float>();

	//把所有点关联的面的数量设置为0
	for(int i = 0; i < point_num; i++)
	{
		conns_vertex_face[i] = new int[1];
		conns_vertex_face[i][0] = 0;		//子数组的第一位为相关面的个数

		conns_vertex_edge[i] = new int[1];
		conns_vertex_edge[i][0] = 0;
	}

	int size = 0, v1,v2;

	//遍历所有的面
	int i,j,k;
	int* temp;
	for(i = 0; i< face_num; i++)
	{
		//建立三个顶点与当前面片连接的拓扑关系
		for(j = 0; j< 3; j++)
		{
			v1 = faces[3*i + j];
			size = conns_vertex_face[v1][0] + 1;
			temp = new int[size + 1];
			temp[0] = size;					//列表的大小增加1
			for(k = 1; k<size; k++)
				temp[k] = conns_vertex_face[v1][k];

			temp[size] = i;					//将现在这个面加入点相关面列表的最后
			delete conns_vertex_face[v1];
			conns_vertex_face[v1] = temp;

			//添加新的边，并建立三条边与顶点的拓扑关系
			v2 = faces[3*i + (j + 1)%3]; 

			//如果当前边已经存在了，则不添加
			bool isExist = false;
			for(k = 1; k <= conns_vertex_edge[v1][0]; k++)
			{
				if(v2 == conns_vertex_edge[v1][2*k-1])
				{
					isExist = true;
					break;
				}
			}
			if(isExist)
				continue;


			//添加边
			edges->push_back(v1);
			edges->push_back(v2);
			//根据v1v2的位置计算当前边的长度
			Vector3f* tmp_edge = new Vector3f(points[3*v1]-points[3*v2], points[3*v1+1]-points[3*v2+1], points[3*v1+2]-points[3*v2+2]);
			edges->push_back(tmp_edge->getLength());

			//建立v1,v2与当前边的拓扑关系
			size = conns_vertex_edge[v1][0] + 1;
			temp = new int[2*size + 1];
			temp[0] = size;					//列表的大小增加1
			for(k = 1; k<2*size-1; k++)
				temp[k] = conns_vertex_edge[v1][k];

			temp[2 * size-1] = v2;								//加入关联的顶点
			temp[2 * size] = edges->size()/3-1;					//将最后一条边加入
			delete conns_vertex_edge[v1];
			conns_vertex_edge[v1] = temp;

			size = conns_vertex_edge[v2][0] + 1;
			temp = new int[2 * size + 1];
			temp[0] = size;					//列表的大小增加1
			for(k = 1; k<2 *size-1; k++)
				temp[k] = conns_vertex_edge[v2][k];

			temp[2 * size - 1] = v1;
			temp[2 * size] = edges->size()/3-1;					//将最后一条边加入
			delete conns_vertex_edge[v2];
			conns_vertex_edge[v2] = temp;
		}
	}

}

//将所有面片的位置按中心点排序，以方便后来建立AABB树的包围盒
void BasicStruct::RangeFaces()
{
	face_Center = new float[3 * face_num * 6];			//为防顶点数增多，把这个数组开大点
	//同上的取巧做法
	face_range_x = new float[face_num];
	face_range_y = new float[face_num];
	face_range_z = new float[face_num];
	range_x = new int[face_num];
	range_y = new int[face_num];
	range_z = new int[face_num];

	int n;										
	float x, y, z, temp_x, temp_y, temp_z;
	for(int i = 0; i < face_num; i++)
	{
		x = y = z = 0.0f;
		for(int j = 0; j < 3; j++)
		{
			n = faces[i * 3 + j];						//该面片的序号
			temp_x = points[3 * n];						//该顶点的x值
			temp_y = points[3 * n + 1];
			temp_z = points[3 * n + 2];

			//找出包围盒在x轴方向上的边界值
			if( temp_x < min_x)
				min_x = temp_x;
			else if ( temp_x > max_x)
				max_x = temp_x;

			//找出包围盒在y轴方向上的边界值
			if( temp_y < min_y)
				min_y = temp_y;
			else if ( temp_y > max_y)
				max_y = temp_y;

			//找出包围盒在z轴方向上的边界值
			if( temp_z < min_z)
				min_z = temp_z;
			else if ( temp_z > max_z)
				max_z = temp_z;

			//求个方向的顶点和
			x += temp_x;
			y += temp_y;
			z += temp_z;
		}

		//将平均值存入数组，作为面片的位置
		face_range_x[i] = face_Center[3 * i] = x / 3 ;
		face_range_y[i] = face_Center[3 * i + 1] = y / 3;
		face_range_z[i] = face_Center[3 * i + 2] = z / 3;
		range_x[i] = range_y[i] = range_z[i] = i;
	}

	//将中心点按照x,y,z轴分别排序
	quickSort(range_x, face_range_x, 0, face_num - 1);
	quickSort(range_y, face_range_y, 0, face_num - 1);
	quickSort(range_z, face_range_z, 0, face_num - 1);

}

/************************************************************************/
/* 将a与b，c与d分别交换                                                 */
/************************************************************************/
void BasicStruct::swap(int *a, int *b, float *c, float *d)
{
	int x;
	x = *a;
	*a = *b;
	*b = x;

	float y;
	y = *c;
	*c = *d;
	*d = y;
}

/************************************************************************/
/* 将比位置i的数大的数放到数组的一边，小的放到另外一边                  */
/************************************************************************/
int BasicStruct::partition(int* A, float * B, int p, int r)
{
	int random = (p + (int)(((float)rand()/RAND_MAX)*(r - p)));
	swap(&A[random], &A[r], &B[random], &B[r]);
	int i = p - 1, j;
	for(j = p; j < r; j++)
	{
		if(B[j] > B[r])
		{
			i++;
			swap(&A[i], &A[j], &B[i], &B[j]);
		}
	}
	swap(&A[i + 1], &A[r], &B[i + 1], &B[r]);
	return i + 1;
}

/************************************************************************/
/* 利用递归的思想完成快速排序算法                                       */
/************************************************************************/
void BasicStruct::quickSort(int *A, float *B, int p, int r)
{
	int i;
	if(p < r)
	{
		i = partition(A, B, p, r);
		quickSort(A, B, p, i - 1);
		quickSort(A, B, i + 1, r);
	}   
}