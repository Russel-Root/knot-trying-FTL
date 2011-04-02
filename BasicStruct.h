#pragma once
#include "Vector3f.h"

//#include <stdio.h>
//#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define LINE_MODE	0
#define FILL_MODE	1

// forward declarations
struct MeshStruct;
struct TopoInfo3d;
struct AuxMeshStruct;
//////////////////////////////////////////////////////////////////////////
class BasicStruct
{
public:
	BasicStruct(void);
	~BasicStruct(void);

public:	
	bool CreateMeshFromFile(char* filename, float scale, float x, float y, float z);
	//void DrawMesh(int draw_mode);
	void DrawMesh();

	//this method is for test
	void setHighLightFace(int* i){ this->highlight_face = i;}
	void setHighLightEdge(float* e){ this->highlight_edge = e;}

	void RangeFaces(void);						//�����еĵ������Ա��Χ�е�����
	void UpdateSingleFaceNorm(int face_num);	//������������ĵ�λ�ø���ƽ��ķ�����
	void UpdateSingleVertexNorm(int point_num);	//���µ�������ķ�����

	void UpdatePoints();						//���ݵ�ǰ������ģ�����»�ö����λ��

protected:
	void UpdateFaceNorms(void);					//ˢ��������Ƭ������
	void UpdateVertexNorms(void);				//ˢ�¶��㷨����

	void UpdateNormals(void);					//������Ƭ����λ�û�ȡģ�͵���Ƭ�����㷨����
	void CreateTopoStruct(void);				//����ģ�͵����˽ṹ

	void swap(int* a, int* b, float* c, float* d);			//���������е������������������㷨
	int partition(int* A, float* B, int p, int r);			//����ABΪ���飬prΪҪ����ķ�Χ


public:
	void quickSort(int* A, float* B, int p, int r);

public:
	int point_num;				//������
	int face_num;				//��Ƭ��

	float* orig_points;			//��ʼ���λ�ã�������ȫ�ָ���״̬
	float* points;				//����λ����Ϣ

	Vector3f** v_speed;				//ÿ��������ٶ�
	Vector3f** v_force;			//ÿ������ܵ��������

	vector<float>* edges;		//�ߵ���Ϣ��ÿ����ռ��λ���ֱ�������������ż��ߵĳ���
	int* faces;					//������������Ķ�����Ϣ,�����Ƕ��������еĵڼ���

	float* f_normals;			//��Ƭ������
	float* v_normals;			//����ķ��������飬����Ƭ���������

	//this is for test
	int* highlight_face;		//����ײ��߹���ʾ����Ƭ
	float* highlight_edge;		//���и�����������ɵı߸߹���ʾ

public:
	float* face_Center;										//����������ĵ�
	float min_x, min_y, min_z, max_x, max_y, max_z;			//����Χ�еı߽�

	float* face_range_x;									//���е��λ����x���ϵ�����
	int* range_x;

	float* face_range_y;									//���е��λ����y���ϵ�����
	int* range_y;

	float* face_range_z;									//���е��λ������z���ϵ�����
	int* range_z;

	//�������鴢����ģ�͵����˽ṹ
	int** conns_vertex_face;							//������Ĺ�ϵ
	int** conns_vertex_edge;							//����ߵ����ӹ�ϵ
	bool flag;											//ģ���Ƿ񱻶���

private:
	float k1, k2;					//�ֱ��ʾ��֮����˵���������ϵ��
	float time_step;					//ʱ�䲽��
	float v_m;						//�����ʵ������
};

struct MeshStruct{
	int numPoints;
	int numFaces;

	float*	points; // ����λ����Ϣ
	int*	faces; // ��Ƭ����������Ӧ�Ķ�����Ϣ

	float*	normals; // ��Ƭ������
	float*	normalsPoints; // ����ķ�������Ϣ������Ƭ���������

	// texture
	int		numTexturePoints; // the quantity of texture points
	int		numTextureFaces; // the quantity of texture faces
	
	float*	texturePoints; // the coordinates of texture���������
	int*	textureFaces; // �����������Ƭ

	int		tagTopo;
	int		tagAux;
	int		tagTexture;

	TopoInfo3d*		topo;
	AuxMeshStruct*	aux;

	MeshStruct(){
		numPoints = numFaces = 0;
		faces = NULL;
		points = normals = normalsPoints = NULL;

		numTexturePoints = numTextureFaces = 0;
		texturePoints = NULL;
		textureFaces = NULL;

		topo = NULL;
		aux = NULL;

		tagTopo = tagAux = tagTexture = 0;
	}
	~MeshStruct(){}
};

struct TopoInfo3d{
	int**	connsVertexVectex;
	int**	connsVertexFace;
	int**	connsEdgeFace;
	int**	connsFaceFace;

	MeshStruct* gem;
};

struct AuxMeshStruct{
	int	numVertex;
	int faceCenterVertex;

	float*	vertexs;
	float*	faceCenterVertexs;

	float*	faceCenterVertexDistance;

	MeshStruct* gem;
};
