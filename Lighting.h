#ifndef __Lighting_h__
#define __Lighting_h__

#include <gl/glut.h>

class Lighting{
public:
	Lighting();
	~Lighting();

	void Enable();
	void Disable();
	void setType(int t){type = t;}
	void setLightNum(int num) { setNum = num; }
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setLight();
private:
	int type;
	int setNum;
	GLfloat ambientR, ambientG, ambientB, ambientA;
	GLfloat diffuseR, diffuseG, diffuseB, diffuseA;
	GLfloat specularR, specularG, specularB, specularA;
	GLfloat positionX, positionY, positionZ;
	bool enable;
};
#endif