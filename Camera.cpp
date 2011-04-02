//#include "stdafx.h"
#include "Camera.h"

Camera::Camera(){
	position = Vector3f(0.0f, 0.0f, -1.0f);	//initiate the camera position
	view	 = Vector3f(0.0f, 0.0f, 0.0f);		//view position
	upVector = Vector3f(0.0f, 1.0f, 0.0f);		//view vector
}

void Camera::setCamera(GLfloat positionX, GLfloat positionY, GLfloat positionZ,
					   GLfloat viewX,     GLfloat viewY,     GLfloat viewZ,
					   GLfloat upVectorX, GLfloat upVectorY, GLfloat upVectorZ)
{
	position = Vector3f(positionX, positionY, positionZ);
	view	 = Vector3f(viewX, viewY, viewZ);
	upVector = Vector3f(upVectorX, upVectorY, upVectorZ);
}


/************************************************************************/
/* 旋转摄像机，沿着View - Position旋转                          		*/
/* 表现为视角的左右转动,负值向左转，正值向右转							*/
/************************************************************************/
void Camera::RotateX(GLfloat angle)
{
	Vector3f base = (view - position).Normalize();
	upVector = upVector.Rotate(base, angle);
}

/************************************************************************/
/* 旋转摄像机，沿着Up-Vector(Y轴）旋转，即在x-z平面内转动				*/
/* 表现为视角的左右移动,负值向右，正值向左								*/
/************************************************************************/
void Camera::RotateY(GLfloat angle)
{
	Vector3f rotate = (view - position).Normalize();
	rotate = rotate.Rotate(upVector,angle);
	view = position + rotate;
}

/************************************************************************/
/* 旋转摄像机，沿着(UpVector)* (View - Position)(Z轴)旋转				*/
/* 表现为视角的上下移动，负值向下，正值向上								*/
/************************************************************************/
void Camera::RotateZ(GLfloat angle)
{
	Vector3f base = (view - position).Cross(upVector).Normalize();
	view = position + (view - position).Rotate(base, angle);
	upVector = upVector.Rotate(base, angle);
}

/************************************************************************/
/* 前后移动摄像机，step为每次移动的步长，正值前进，负值后退             */
/************************************************************************/
void Camera::MoveCamera(GLfloat step)
{	

	Vector3f temp = (view - position).Normalize();
	position = position + temp * step;
	view     = view     + temp * step;
}

/************************************************************************/
/* 摄像机绕原点旋转，angle_y为绕(UPVector)*(View - Position)转动的角度	*/
/************************************************************************/
void Camera::RotateCamera_Y(GLfloat angle_y)
{
	//将目标位置锁定在原点
	view.setVector(0.0f, 0.0f, 0.0f);
	GLfloat length = (view - position).getLength();
	//转竖直方向
	Vector3f temp = (view - position).Normalize();
	Vector3f base = temp.Cross(upVector).Normalize();
	temp = temp.Rotate(base, - angle_y);
	//得到新的向上向量
	upVector = upVector.Rotate(base, - angle_y);
	//得到新的相机位置
	position.setVector(- length * temp.x, - length * temp.y, - length * temp.z);
}

/************************************************************************/
/* 摄像机绕原点旋转，angle_x为绕UpVector方向转动的角度					*/
/************************************************************************/
void Camera::RotateCamera_X(GLfloat angle_x)
{
	//将目标位置锁定在原点
	view.setVector(0.0f, 0.0f, 0.0f);
	//转水平方向
	GLfloat length = (view - position).getLength();
	Vector3f temp = (view - position).Normalize().Rotate(upVector.Normalize(), - angle_x);
	//仍然看向原点，upVector方向不变，得到新的摄像机位置
	position.setVector(- length * temp.x, - length * temp.y, - length * temp.z);
}

void Camera::Look()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position.x, position.y, position.z,	
		view.x,	   view.y,     view.z,	
		upVector.x, upVector.y, upVector.z);
}