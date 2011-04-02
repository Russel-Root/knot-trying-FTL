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
	void setNum(int n);			// ����������ͼ��
	void enableTexture(int k);	// ����ͼ��������
	void disableTexture();

public:
	// the basic material property
	GLfloat ambientR, ambientG, ambientB, ambientA; // RGBA(alpha)
	GLfloat diffuseR, diffuseG, diffuseB, diffuseA;
	GLfloat specularR, specularG, specularB, specularA;
	GLfloat shininess;

	// the texture property
	int num;			// ��ͼ������
	int current;		// ������ͼ��
	GLuint* g_texture;	// ����������ͼ������
};
#endif