#ifndef __Fog_h__
#define __Fog_h__

#include <gl/glut.h>

class Fog{
public:
	Fog();
	~Fog();

	void Enable();
	void Disable();
	void setDensity(GLfloat d) { density = d; }
	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void setRange(GLfloat s, GLfloat e);
	void setFog();
private:
	GLfloat density;
	GLfloat colorR, colorG, colorB, colorA;
	GLfloat start, end;
	bool	enable;				
};
#endif