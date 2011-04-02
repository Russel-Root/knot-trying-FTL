#include "Lighting.h"

#define AMBIENT_LIGHT	0
#define POINT_LIGHT		1
#define PARALLEL_LIGHT	2

Lighting::Lighting(){
	type = 0;
	ambientR = ambientG = ambientB = ambientA = 0.0f;
	diffuseR = diffuseG = diffuseB = diffuseA = 0.0f;
	specularR = specularG = specularB = specularA = 0.0f;
	positionX = positionY = positionZ  = 0.0f;
	enable = false;
}

Lighting::~Lighting(){}

void Lighting::Enable(){
	enable = true;
	glEnable(setNum);
}

void Lighting::Disable(){
	enable = false;
	glDisable(setNum);
}

void Lighting::setPosition(GLfloat x, GLfloat y, GLfloat z){
	positionX = x;
	positionY = y;
	positionZ = z;
}

void Lighting::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
	ambientR = r;
	ambientG = g;
	ambientB = b;
	ambientA = a;
}

void Lighting::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
	diffuseR = r;
	diffuseG = g;
	diffuseB = b;
	diffuseA = a;
}

void Lighting::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
	specularR = r;
	specularG = g;
	specularB = b;
	specularA = a;
}

void Lighting::setLight(){
	if (enable){
		GLfloat light_ambient[4];
		GLfloat light_diffuse[4];
		GLfloat light_specular[4];
		GLfloat light_position[4];

		light_ambient[0] = ambientR;
		light_ambient[1] = ambientG;
		light_ambient[2] = ambientB;
		light_ambient[3] = ambientA;

		light_diffuse[0] = diffuseR;
		light_diffuse[1] = diffuseG;
		light_diffuse[2] = diffuseB;
		light_diffuse[3] = diffuseA;

		light_specular[0] = specularR;
		light_specular[1] = specularG;
		light_specular[2] = specularB;
		light_specular[3] = specularA;

		light_position[0] = positionX;
		light_position[1] = positionY;
		light_position[2] = positionZ;

		switch(type){
			case(AMBIENT_LIGHT):
				glLightfv(setNum, GL_AMBIENT, light_ambient);
				glLightfv(setNum, GL_DIFFUSE, light_diffuse);
				glLightfv(setNum, GL_SPECULAR, light_specular);
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
				break;
			case(POINT_LIGHT):
				light_position[3] = 0.0f;
				glLightfv(setNum, GL_AMBIENT, light_ambient);
				glLightfv(setNum, GL_DIFFUSE, light_diffuse);
				glLightfv(setNum, GL_SPECULAR, light_specular);

				//glPushMatrix();
				//glLoadIdentity();
				glLightfv(setNum, GL_POSITION, light_position);
				//glPopMatrix();
				break;
			case(PARALLEL_LIGHT):
				light_position[3] = 1.0f;
				glLightfv(setNum, GL_AMBIENT, light_ambient);
				glLightfv(setNum, GL_DIFFUSE, light_diffuse);
				glLightfv(setNum, GL_SPECULAR, light_specular);

				//glPushMatrix();
				//glLoadIdentity();
				glLightfv(setNum, GL_POSITION, light_position);
				//glPopMatrix();
				break;
			default:
				break;
		}
	}
}