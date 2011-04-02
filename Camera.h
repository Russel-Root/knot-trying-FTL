#pragma once
//#include <gl/glut.h>
#include "Vector3f.h"

class Camera {
public:
	//the Constructor
	Camera();	

	Vector3f getPosition(){return position;}
	Vector3f getView(){return view;}
	Vector3f getUpVector() {return upVector;}

	void setCamera(GLfloat positionX, GLfloat positionY, GLfloat positionZ,
		GLfloat viewX,     GLfloat viewY,     GLfloat viewZ,
		GLfloat upVectorX, GLfloat upVectorY, GLfloat upVectorZ);

	//旋转摄像机， 绕Y轴旋转,旋转角度为angle
	void RotateY(GLfloat angle);

	//旋转摄像机， 绕X轴旋转,旋转角度为angle
	void RotateX(GLfloat angle);

	//旋转摄像机， 绕Z轴旋转,旋转角度为angle
	void RotateZ(GLfloat angle);

	//move the camera
	void MoveCamera(GLfloat step);

	//摄像器指向原点旋转,在水平方向
	void RotateCamera_X(GLfloat angle_x);

	void RotateCamera_Y(GLfloat angle_y);

	//设置绘制的视角和摄像机的视角相同
	void Look();

private:
	Vector3f position;		
	Vector3f view;			
	Vector3f upVector;
};


