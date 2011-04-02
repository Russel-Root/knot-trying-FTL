#ifndef __Material_h__
#define __Material_h__

#include <stdlib.h>
#include <gl/glut.h>
#include "ImageLoader.h"

class Material{
public:
	Material();
	~Material();

	void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setShininess(GLfloat s);

	void setMaterial();
	void setTransparent(bool transp);

	bool addTexture(const char* filename);
	void setNum(int n);			// 设置纹理贴图数
	void enableTexture(int k);	// 打开贴图纹理属性
	void disableTexture();

public:
	// the basic material property
	GLfloat ambientR, ambientG, ambientB, ambientA; // RGBA(alpha)
	GLfloat diffuseR, diffuseG, diffuseB, diffuseA;
	GLfloat specularR, specularG, specularB, specularA;
	GLfloat shininess;

	// the texture property
	int num;			// 贴图的数量
	int current;		// 现有贴图数
	GLuint* g_texture;	// 储存纹理贴图的数组
};
#endif