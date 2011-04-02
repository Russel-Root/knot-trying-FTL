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

	void RangeFaces(void);						//将所有的点排序以便包围盒的生成
	void UpdateSingleFaceNorm(int face_num);	//根据三个顶点的的位置更新平面的法向量
	void UpdateSingleVertexNorm(int point_num);	//更新单个顶点的法向量

	void UpdatePoints();						//根据当前的物理模型重新获得顶点的位置

protected:
	void UpdateFaceNorms(void);					//刷新三角面片法向量
	void UpdateVertexNorms(void);				//刷新顶点法向量

	void UpdateNormals(void);					//根据面片顶点位置获取模型的面片及顶点法向量
	void CreateTopoStruct(void);				//创建模型的拓扑结构

	void swap(int* a, int* b, float* c, float* d);			//交换数组中的两个数，用于排序算法
	int partition(int* A, float* B, int p, int r);			//其中AB为数组，pr为要排序的范围


public:
	void quickSort(int* A, float* B, int p, int r);

public:
	int point_num;				//顶点数
	int face_num;				//面片数

	float* orig_points;			//初始点的位置，物体完全恢复的状态
	float* points;				//顶点位置信息

	Vector3f** v_speed;				//每个顶点的速度
	Vector3f** v_force;			//每个点的总的受力情况

	vector<float>* edges;		//边的信息，每条边占三位，分别是两个顶点序号及边的长度
	int* faces;					//多边形所包含的顶点信息,顶点是顶点数组中的第几个

	float* f_normals;			//面片法向量
	float* v_normals;			//顶点的法向量数组，由面片的向量求出

	//this is for test
	int* highlight_face;		//被碰撞后高光显示的面片
	float* highlight_edge;		//将切割过程中新生成的边高光显示

public:
	float* face_Center;										//储存面的中心点
	float min_x, min_y, min_z, max_x, max_y, max_z;			//最大包围盒的边界

	float* face_range_x;									//所有点的位置在x轴上的排序
	int* range_x;

	float* face_range_y;									//所有点的位置在y轴上的排序
	int* range_y;

	float* face_range_z;									//所有点的位置在在z轴上的排序
	int* range_z;

	//以下数组储存了模型的拓扑结构
	int** conns_vertex_face;							//点与面的关系
	int** conns_vertex_edge;							//点与边的连接关系
	bool flag;											//模型是否被读入

private:
	float k1, k2;					//分别表示点之间与核的两个弹性系数
	float time_step;					//时间步长
	float v_m;						//单个质点的质量
};

struct MeshStruct{
	int numPoints;
	int numFaces;

	float*	points; // 顶点位置信息
	int*	faces; // 面片（包含）对应的顶点信息

	float*	normals; // 面片法向量
	float*	normalsPoints; // 顶点的法向量信息，由面片的向量求出

	// texture
	int		numTexturePoints; // the quantity of texture points
	int		numTextureFaces; // the quantity of texture faces
	
	float*	texturePoints; // the coordinates of texture纹理的坐标
	int*	textureFaces; // 纹理的三角面片

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
