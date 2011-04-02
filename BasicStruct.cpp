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
/* ͨ����ȡ�ļ�����ģ�ͣ��������true�����ļ���ȡ�ɹ���false�򲻳ɹ�    */
/************************************************************************/
bool BasicStruct::CreateMeshFromFile(char* filename, float scale, float x, float y, float z)
{
	//�õ��ļ���׺��
	string type = strrchr(filename, '.');
	//����ļ���ʽ�����������֣����ؿ�
	if(!(type == ".3dsa" || type == ".vtka" || type == ".plya" || type == ".bod" || type == ".bd" || type == ".roda"))
		return false;

	ifstream readin( filename, ios::in );
	if( !readin )
		return false;

	//��ȡ����������Ƭ��
	int pn, fn;
	readin >> pn >> fn;

	point_num = pn;
	face_num = fn;

	//��ʼ���������鼰����������
	orig_points = new float[point_num * 3 * 6]; //@ Ϊ��Ҫ*6�أ�ɾȥ����Ȼ������������
	points = new float[ point_num * 3     * 6];
	faces = new int[ face_num * 3         * 6];
	f_normals = new float[ face_num * 3   * 6];
	v_normals = new float[ point_num * 3  * 6];

	//��ʼ�����ж�����ٶȳ�����ʼ�ٶ�Ϊ0
	v_speed = new Vector3f*[point_num * 3     * 6]; //@ Vector3f��default constructor��ʼ��x,y,zΪ0
	v_force = new Vector3f*[point_num * 3 * 6];

	for(int i = 0; i < point_num*3; i++)
	{
		v_speed[i] = new Vector3f();
		v_force[i] = new Vector3f();
	}


	int i, idx;
	char title[3];
	float temp1, temp2, temp3;


	//��ȡ��׺��Ϊ".3dsa"��ʽ���ļ�
	if( type == ".3dsa" )
	{
		//�������ж�����Ϣ
		for( i = 0; i < point_num; i++ )
		{
			readin >> idx >> temp1 >> temp2 >> temp3;
			orig_points[i*3] = points[i*3] = temp1 * scale + x;
			orig_points[i*3+1] = points[i*3 + 1] = temp2 * scale + y;
			orig_points[i*3+2] = points[i*3 + 2] = temp3 * scale + z;
		}

		//����������Ƭ��Ϣ
		for( i = 0; i < face_num; i++ )
		{
			readin >> idx >> title >> faces[ i*3 ] >> title >> faces[ i*3+1 ] >> title >> faces[ i*3+2 ];
			readin >> title >> idx >> title >> idx >> title >> idx;
		}	
	}

	//��ȡ��׺��Ϊ".vtka"���ļ�
	else if( type == ".vtka" )
	{	
		//��ȡ���ж���
		for( i = 0; i < point_num; i++ )
		{
			readin >> temp1 >> temp2 >> temp3;
			orig_points[i*3] = points[i*3] = temp1 * scale + x;
			orig_points[i*3+1] = points[i*3 + 1] = temp2 * scale + y;
			orig_points[i*3+2] = points[i*3 + 2] = temp3 * scale + z;
		}

		//��ȡ������Ƭ
		for( i = 0; i < face_num; i++ )
			readin >> idx >> faces[ i*3 ] >> faces[ i*3+1 ] >> faces[ i*3+2 ];
	}

	//��ȡ��׺��Ϊ".plya"���ļ�
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

	//��ȡ��׺��Ϊ".bd"���ļ�
	else if( type == ".bd" || type == ".bod")
	{
		//�������ж�����Ϣ
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


	//����ģ�͵����˽ṹ
	this->CreateTopoStruct();
	//�õ����з�����
	this->UpdateNormals();
	//�����е���Ƭ��λ������
	this->RangeFaces();

	flag = true;
	return true;
}

/************************************************************************/
/* ����ģ�͵ĵ��淨����											        */
/************************************************************************/
void BasicStruct::UpdateNormals(void)
{
	UpdateFaceNorms();			//������Ƭ������
	UpdateVertexNorms();		//���¶��㷨����
}

/************************************************************************/
/* ������Ƭ��������ģ��                                                 */
/************************************************************************/
void BasicStruct::DrawMesh()//(int draw_mode)
{
	if(!flag)
		return;	

	//ȷ������ε���������ξ����ġ��ڴ���Ϊ��ʱ�뷽��
	glFrontFace(GL_CCW);

	//����һ����������Ƭ��ģ��
	int n = 0;

	////ѡ���߿�ģʽ
	//if(draw_mode == LINE_MODE)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	////ѡ��ʵ��ģʽ
	//else if(draw_mode == FILL_MODE)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	glLineWidth(1.0f);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i< face_num; i++)
	{
		//����i��������Ƭ����������
		n = (faces[i*3]) * 3;				//��һ������ĵ�һ��λ������
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[i*3 + 1]) * 3;			//�ڶ�������ĵ�һ��λ������
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[i*3 + 2]) * 3;			//��һ������ĵ�һ��λ������
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);
	}
	glEnd();


	//glPopMatrix();

	//////////////////////////////////////////////////////////////////////////
	//this is for test
	//������Щ��ײ����ཻ����
	//////////////////////////////////////////////////////////////////////////
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	if(highlight_face == NULL)
		return;
	for(int i = 0; i< this->highlight_face[0]; i++)
	{
		//����i��������Ƭ����������
		n = (faces[highlight_face[i+1]*3]) * 3;				//��һ������ĵ�һ��λ������
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[highlight_face[i+1]*3 + 1]) * 3;			//�ڶ�������ĵ�һ��λ������
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);

		n = (faces[highlight_face[i+1]*3 + 2]) * 3;			//��һ������ĵ�һ��λ������
		glNormal3f(v_normals[n], v_normals[n+1], v_normals[n+2]);
		glVertex3f(points[n], points[n+1], points[n+2]);
	}
	glEnd();
	glPopMatrix();
	//////////////////////////////////////////////////////////////////////////

}

/************************************************************************/
/* �������������λ�ü��㵥����Ƭ�ķ�����                               */
/************************************************************************/
void BasicStruct::UpdateSingleFaceNorm(int faceNum)
{
	Vector3f v1, v2, v3, norm;
	int n1, n2, n3;

	n1 = faces[faceNum * 3];		//��һ������Ĵ���
	n2 = faces[faceNum * 3 + 1];	//�ڶ�������Ĵ���
	n3 = faces[faceNum * 3 + 2];	//����������Ĵ���

	v1.setVector(points[3*n1], points[3*n1 + 1],points[3*n1 + 2]);
	v2.setVector(points[3*n2], points[3*n2 + 1],points[3*n2 + 2]);
	v3.setVector(points[3*n3], points[3*n3 + 1],points[3*n3 + 2]);
	norm = (v2 - v1).Cross(v3 - v1).Normalize();

	f_normals[3 * faceNum] = norm.x;
	f_normals[3 * faceNum + 1] = norm.y;
	f_normals[3 * faceNum + 2] = norm.z;
}

/************************************************************************/
/* �������ڵ���ķ��������µ�������ķ�����                             */
/************************************************************************/
void BasicStruct::UpdateSingleVertexNorm(int pointNum)
{
	Vector3f norm;
	float x, y, z;
	int n, size;

	size = conns_vertex_face[pointNum][0];		//�õ���˶�����ص����ж��ٸ�
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
/* ����ÿ����Ƭ�Ķ���λ�ü���ÿ����Ƭ�ķ�����                           */
/************************************************************************/
void BasicStruct::UpdateFaceNorms(void)
{
	for(int i = 0; i< this->face_num; i++)
		UpdateSingleFaceNorm(i);
}

/************************************************************************/
/* ����������ڵ�������Ƭ�õ���ķ�����                                 */
/************************************************************************/
void BasicStruct::UpdateVertexNorms(void)
{
	for(int i = 0; i < this->point_num; i++)
		UpdateSingleVertexNorm(i);
}

/************************************************************************/
/* ����ÿ������ĵ�ǰ�ٶ��Լ���������������ж����λ���Լ��ٶ�         */
/************************************************************************/
void BasicStruct::UpdatePoints()
{	
	//�Ӹ���������	
	Vector3f tmp_vector;
	float plus_result;			//���������˶�����ĵ��

	int times = 0;

	while(times < 20) //@ ����20�Σ������ȶ���
	{

		for(int i = 0; i < point_num; i++)
		{
			//��������
			(*v_force[i]).setVector(0, v_m * 9.8, 0);

			//����ţ�ٵڶ����ɼ���˽ṹ�����ϵĵ���,����k2Ϊ�˵��ɵĵ���ϵ��
			tmp_vector.setVector((orig_points[3*i]-points[3*i])*k2,(orig_points[3*i+1]-points[3*i+1])*k2,(orig_points[3*i+2]-points[3*i+2])*k2);

			//�����ʱ�ĵ������ʵ���˶�����С��90��ǣ���Ϊ������ͬ����������֮�෴����֮��������֮��ͬ��
			plus_result = tmp_vector * (*v_speed[i]);

			//plus_result = (tmp_vector.Normalize()) * ((*v_speed[i]).Normalize());
			//������ͬ
			if(plus_result > 0.1)
				tmp_vector = tmp_vector*0.8;		//������ϵ���ĵ�����Ϊ�ؼ�
			//����ͬʱ
			else if(plus_result < -0.1)
				tmp_vector = tmp_vector*1.2;

			(*v_force[i]) += tmp_vector;

		}

		//�������еıߣ��������������صĵ���������
		int edge_num = edges->size()/3;
		Vector3f current_edge, edge_force;
		int v1,v2;		//���ӵ�ǰ�ߵ������ڵ�����
		float dl;		//�ߵı仯��
		for(int i = 0; i < edge_num; i++)
		{
			v1 = (*edges)[3 * i];
			v2 = (*edges)[3 * i + 1];
			current_edge.setVector(points[3*v1]-points[3*v2],points[3*v1+1]-points[3*v2+1],points[3*v1+2]-points[3*v2+2]);
			dl = current_edge.getLength() - (*edges)[3 * i + 2];
			//���仯����С��������еĶ��㲻Ӱ��
			if(dl < 0.001 && dl > -0.001)
				continue;

			//������v1v2�Ϸֱ�ʩ������������ϵ���
			edge_force = current_edge.setLength(dl * k1);

			//ͬ������������ϣ�ͬ����Ҫ����һ��������
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


		//�������ж��㣬���ݵ���������������λ�ü��ٶ�
		for(int i = 0; i < point_num; i++)
		{
			////��һ�����ٶȺ�С����ԭʼλ���ǻ�ʱ����������Ϊ0
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

	//���ݵ�ǰ����Ƭ���·�����������ûʲô�ı䣬�ǾͲ�������
	this->UpdateNormals();

}

/************************************************************************/
/* ����ģ�͵����˽ṹ                                                   */
/************************************************************************/
void BasicStruct::CreateTopoStruct(void)
{
	//����������Ĺ�ϵ
	conns_vertex_face = new int*[point_num * 6];
	//���������Ĺ�ϵ��ÿ������������λ���棬��һλ��ʾ���������ţ��ڶ�λ��ʾ�������ǵıߵ����
	conns_vertex_edge = new int*[point_num * 12];
	//����ŷ������һ����˵e=f+v-2�͹��ˣ�
	edges = new vector<float>();

	//�����е�����������������Ϊ0
	for(int i = 0; i < point_num; i++)
	{
		conns_vertex_face[i] = new int[1];
		conns_vertex_face[i][0] = 0;		//������ĵ�һλΪ�����ĸ���

		conns_vertex_edge[i] = new int[1];
		conns_vertex_edge[i][0] = 0;
	}

	int size = 0, v1,v2;

	//�������е���
	int i,j,k;
	int* temp;
	for(i = 0; i< face_num; i++)
	{
		//�������������뵱ǰ��Ƭ���ӵ����˹�ϵ
		for(j = 0; j< 3; j++)
		{
			v1 = faces[3*i + j];
			size = conns_vertex_face[v1][0] + 1;
			temp = new int[size + 1];
			temp[0] = size;					//�б�Ĵ�С����1
			for(k = 1; k<size; k++)
				temp[k] = conns_vertex_face[v1][k];

			temp[size] = i;					//���������������������б�����
			delete conns_vertex_face[v1];
			conns_vertex_face[v1] = temp;

			//����µıߣ��������������붥������˹�ϵ
			v2 = faces[3*i + (j + 1)%3]; 

			//�����ǰ���Ѿ������ˣ������
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


			//��ӱ�
			edges->push_back(v1);
			edges->push_back(v2);
			//����v1v2��λ�ü��㵱ǰ�ߵĳ���
			Vector3f* tmp_edge = new Vector3f(points[3*v1]-points[3*v2], points[3*v1+1]-points[3*v2+1], points[3*v1+2]-points[3*v2+2]);
			edges->push_back(tmp_edge->getLength());

			//����v1,v2�뵱ǰ�ߵ����˹�ϵ
			size = conns_vertex_edge[v1][0] + 1;
			temp = new int[2*size + 1];
			temp[0] = size;					//�б�Ĵ�С����1
			for(k = 1; k<2*size-1; k++)
				temp[k] = conns_vertex_edge[v1][k];

			temp[2 * size-1] = v2;								//��������Ķ���
			temp[2 * size] = edges->size()/3-1;					//�����һ���߼���
			delete conns_vertex_edge[v1];
			conns_vertex_edge[v1] = temp;

			size = conns_vertex_edge[v2][0] + 1;
			temp = new int[2 * size + 1];
			temp[0] = size;					//�б�Ĵ�С����1
			for(k = 1; k<2 *size-1; k++)
				temp[k] = conns_vertex_edge[v2][k];

			temp[2 * size - 1] = v1;
			temp[2 * size] = edges->size()/3-1;					//�����һ���߼���
			delete conns_vertex_edge[v2];
			conns_vertex_edge[v2] = temp;
		}
	}

}

//��������Ƭ��λ�ð����ĵ������Է����������AABB���İ�Χ��
void BasicStruct::RangeFaces()
{
	face_Center = new float[3 * face_num * 6];			//Ϊ�����������࣬��������鿪���
	//ͬ�ϵ�ȡ������
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
			n = faces[i * 3 + j];						//����Ƭ�����
			temp_x = points[3 * n];						//�ö����xֵ
			temp_y = points[3 * n + 1];
			temp_z = points[3 * n + 2];

			//�ҳ���Χ����x�᷽���ϵı߽�ֵ
			if( temp_x < min_x)
				min_x = temp_x;
			else if ( temp_x > max_x)
				max_x = temp_x;

			//�ҳ���Χ����y�᷽���ϵı߽�ֵ
			if( temp_y < min_y)
				min_y = temp_y;
			else if ( temp_y > max_y)
				max_y = temp_y;

			//�ҳ���Χ����z�᷽���ϵı߽�ֵ
			if( temp_z < min_z)
				min_z = temp_z;
			else if ( temp_z > max_z)
				max_z = temp_z;

			//�������Ķ����
			x += temp_x;
			y += temp_y;
			z += temp_z;
		}

		//��ƽ��ֵ�������飬��Ϊ��Ƭ��λ��
		face_range_x[i] = face_Center[3 * i] = x / 3 ;
		face_range_y[i] = face_Center[3 * i + 1] = y / 3;
		face_range_z[i] = face_Center[3 * i + 2] = z / 3;
		range_x[i] = range_y[i] = range_z[i] = i;
	}

	//�����ĵ㰴��x,y,z��ֱ�����
	quickSort(range_x, face_range_x, 0, face_num - 1);
	quickSort(range_y, face_range_y, 0, face_num - 1);
	quickSort(range_z, face_range_z, 0, face_num - 1);

}

/************************************************************************/
/* ��a��b��c��d�ֱ𽻻�                                                 */
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
/* ����λ��i����������ŵ������һ�ߣ�С�ķŵ�����һ��                  */
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
/* ���õݹ��˼����ɿ��������㷨                                       */
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