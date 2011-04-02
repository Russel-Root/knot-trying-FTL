#ifndef __RopeStruct_h__
#define __RopeStruct_h__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Vector3f.h"
//#include "Vector4f.h"

using namespace std;

static const GLfloat	red[4]	 = {0.5, 0.2, 0.3, 1.0};
static const GLfloat	green[4] = {0.05f, 1.0f, 0.05f, 1.0f};
static const GLfloat	blue[4]  = {0.0, 0.0, 1.0, 1.0};

class ParticlesBuffer{ // buffer for the self collision handling
public:
	ParticlesBuffer(int serialNumber, Vector3f direct): index(serialNumber), direction(direct){};
public:
	int index;
	Vector3f direction;
};

class RopeStruct{
public:
	// constructor and unconstructor
	RopeStruct();
	~RopeStruct();

	bool CreateMeshFromFile(char* _file_name, float rank);
	void SphereDraw(Vector3f p, float rad); // draw spheres with the position(Vector3f p), and radius(float rad)
	void CylinderDraw(Vector3f p1, Vector3f p2, float rad); // draw cylinders
	void PointCatmullRom(int n, Vector3f p0, Vector3f p1, Vector3f p2, Vector3f p3); //interpolation draw the Catmull-Rom Spline between the given 2 points(Vector3f p1, Vector3f p2)
	void Draw();
	void drawAboutTailHead(int targetHead, Vector3f dist_vec); // as catmull-rom interpolation need 4 points to interpolate between 2 points, so the points_vec[0] & poins_vec[num_points - 1] should get more attention

	void notLittleEndian(int target, Vector3f move3f);
	void littleEndian(int target, Vector3f move3f);
	void Move(Vector3f dist_vec, int target); // 外部施加作用于序号为target的控制点，该控制点在外力的作用下移动的距离为dist_vec
	void MoveConsecutive(int target, int n, Vector3f dis_vec); // target is not 1 point, but consecutive n points, that's [target], [target+1],..., [target+n-1]
	void MoveAll(Vector3f dist_vec);

	void Update();
	//void ActionBuffer(Vector3f direc, int particleIndex); // buffer of self collision handling
	//void MoveAction();

	void CopyPoints(); // backup the points_vec[0~num_points]

	void Axis();
public:
	int			num_points;
	Vector3f*   points_vec; // particles position
	Vector3f*   points_vec_copy;

	float		length_points;
	float		wid; // width = length_points / 2.0; 这里wid的设置对于碰撞检测有影响

	// if it is the target points, then points_vec_1 = points_vec[0] + dist_vec * 2,
	// if it is the tail points, then points_vec_1 = points_vec[0], that's the original points before the movement
	Vector3f	points_vec_1; 
	Vector3f	points_vec_tail; 

	vector<ParticlesBuffer> bufferSelfcolHandling;

	//int		    redFlag;
	//GLfloat		red[4];		
	//GLfloat		gray[4];
};

#endif