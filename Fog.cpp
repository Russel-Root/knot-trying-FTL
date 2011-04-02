#include "Fog.h"

Fog::Fog(){
	density = colorG = colorR = colorB = colorA = start = end = 0.0f;
	enable = false;
}

Fog::~Fog(){}

void Fog::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
	colorR = r;
	colorG = g;
	colorB = b;
	colorA = a;
}

void Fog::setRange(GLfloat s, GLfloat e){
	start = s;
	end = e;
}

void Fog::Enable(){
	enable = true;
	glEnable(GL_FOG);
}

void Fog::Disable(){
	enable = false;
	glDisable(GL_FOG);
}

void Fog::setFog(){
	if (enable){
		float color[4] = { colorR, colorG, colorB, colorA };
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogfv(GL_FOG_COLOR, color);
		glFogf(GL_FOG_DENSITY, density);
		glFogf(GL_FOG_START, start);
		glFogf(GL_FOG_END, end);
		glHint(GL_FOG_HINT, GL_NICEST);
	}
}