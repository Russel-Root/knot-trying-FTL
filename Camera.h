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

	//��ת������� ��Y����ת,��ת�Ƕ�Ϊangle
	void RotateY(GLfloat angle);

	//��ת������� ��X����ת,��ת�Ƕ�Ϊangle
	void RotateX(GLfloat angle);

	//��ת������� ��Z����ת,��ת�Ƕ�Ϊangle
	void RotateZ(GLfloat angle);

	//move the camera
	void MoveCamera(GLfloat step);

	//������ָ��ԭ����ת,��ˮƽ����
	void RotateCamera_X(GLfloat angle_x);

	void RotateCamera_Y(GLfloat angle_y);

	//���û��Ƶ��ӽǺ���������ӽ���ͬ
	void Look();

private:
	Vector3f position;		
	Vector3f view;			
	Vector3f upVector;
};


