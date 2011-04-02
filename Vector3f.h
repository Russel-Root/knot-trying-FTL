#pragma once 
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#define M_PI               3.14159265359

class Vector3f{
public:

	//default constructor
	Vector3f(){
		x = y = z = 0.0f;
	}

	//Constructor2
	Vector3f(float a, float b, float c){
		x = a;
		y = b;
		z = c;
	}

	void setVector(Vector3f v){ x = v.x; y = v.y; z = v.z;}

	void setVector(float x1, float y1, float z1){ x = x1; y = y1; z = z1;}

	void ToFloat(float* vec){
		vec[0] = this->x;
		vec[1] = this->y;
		vec[2] = this->z;
	}
	//override the operators: + - * / ><
	Vector3f operator+(Vector3f v){
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	void operator+=(Vector3f v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator-=(Vector3f v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	Vector3f operator-(Vector3f v){
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	//两向量点乘
	Vector3f operator*(float n){
		return Vector3f(n * x, n * y, n * z);
	}

	void operator *=(float n){
		x *= n;
		y *= n;
		z *= n;
	}
	//Vector3f operator*=(float n){
	//	return Vector3f(n*x, n*y, n*z);
	//}
	
	// dot product
	float operator*(Vector3f b){
		return b.x * x + b.y * y + b.z * z;
	}

	Vector3f operator/(float n){
		return Vector3f(x / n, y / n, z / n);
	}

	//两向量叉乘
	Vector3f Cross(Vector3f v){
		return Vector3f(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
	}

	float getLength(){
		return (float)sqrt(x*x + y*y + z*z);
	}

	//设置向量的长度
	Vector3f setLength(float length){
		float l = ((float)sqrt(x*x + y*y + z*z )) / length;			
		return Vector3f(x / l , y / l, z / l);
	}

	//单位化向量
	Vector3f Normalize(){
		float l = (float)sqrt(x*x + y*y + z*z );	
		//如果长度为0
		if(l == 0)
			return Vector3f(0, 0, 0);
		return Vector3f(x / l, y / l, z / l);										
	}

	//绕着与其垂直的向量b为轴，旋转angle角度
	Vector3f Rotate(Vector3f b, float angle)
	{
		float cosA = (float)cos(angle/180*M_PI);
		float sinA = (float)sin(angle/180*M_PI);

		float cy = cosA*y - z*b.x*sinA + x*b.z*sinA;
		float cx = cosA*x - y*b.z*sinA + z*b.y*sinA;
		float cz = cosA*z - x*b.y*sinA + y*b.x*sinA;

		return Vector3f(cx, cy, cz);
	}

	void Clear(){ x = y = z = 0.0; }

	Vector3f Negative(){
		return Vector3f( -x, -y, -z);
	}

public:
	float x, y, z;
};