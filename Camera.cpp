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
/* ��ת�����������View - Position��ת                          		*/
/* ����Ϊ�ӽǵ�����ת��,��ֵ����ת����ֵ����ת							*/
/************************************************************************/
void Camera::RotateX(GLfloat angle)
{
	Vector3f base = (view - position).Normalize();
	upVector = upVector.Rotate(base, angle);
}

/************************************************************************/
/* ��ת�����������Up-Vector(Y�ᣩ��ת������x-zƽ����ת��				*/
/* ����Ϊ�ӽǵ������ƶ�,��ֵ���ң���ֵ����								*/
/************************************************************************/
void Camera::RotateY(GLfloat angle)
{
	Vector3f rotate = (view - position).Normalize();
	rotate = rotate.Rotate(upVector,angle);
	view = position + rotate;
}

/************************************************************************/
/* ��ת�����������(UpVector)* (View - Position)(Z��)��ת				*/
/* ����Ϊ�ӽǵ������ƶ�����ֵ���£���ֵ����								*/
/************************************************************************/
void Camera::RotateZ(GLfloat angle)
{
	Vector3f base = (view - position).Cross(upVector).Normalize();
	view = position + (view - position).Rotate(base, angle);
	upVector = upVector.Rotate(base, angle);
}

/************************************************************************/
/* ǰ���ƶ��������stepΪÿ���ƶ��Ĳ�������ֵǰ������ֵ����             */
/************************************************************************/
void Camera::MoveCamera(GLfloat step)
{	

	Vector3f temp = (view - position).Normalize();
	position = position + temp * step;
	view     = view     + temp * step;
}

/************************************************************************/
/* �������ԭ����ת��angle_yΪ��(UPVector)*(View - Position)ת���ĽǶ�	*/
/************************************************************************/
void Camera::RotateCamera_Y(GLfloat angle_y)
{
	//��Ŀ��λ��������ԭ��
	view.setVector(0.0f, 0.0f, 0.0f);
	GLfloat length = (view - position).getLength();
	//ת��ֱ����
	Vector3f temp = (view - position).Normalize();
	Vector3f base = temp.Cross(upVector).Normalize();
	temp = temp.Rotate(base, - angle_y);
	//�õ��µ���������
	upVector = upVector.Rotate(base, - angle_y);
	//�õ��µ����λ��
	position.setVector(- length * temp.x, - length * temp.y, - length * temp.z);
}

/************************************************************************/
/* �������ԭ����ת��angle_xΪ��UpVector����ת���ĽǶ�					*/
/************************************************************************/
void Camera::RotateCamera_X(GLfloat angle_x)
{
	//��Ŀ��λ��������ԭ��
	view.setVector(0.0f, 0.0f, 0.0f);
	//תˮƽ����
	GLfloat length = (view - position).getLength();
	Vector3f temp = (view - position).Normalize().Rotate(upVector.Normalize(), - angle_x);
	//��Ȼ����ԭ�㣬upVector���򲻱䣬�õ��µ������λ��
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