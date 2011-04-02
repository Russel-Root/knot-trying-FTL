#include "RenderImpGL.h"

RenderImpGL::RenderImpGL(){}

RenderImpGL::~RenderImpGL(){}

void RenderImpGL::init(){
	cout << "start to init RenderImpGL..." << endl;
	//::glClearColor(0.8, 0.8, 0.8, 0.0);
	::glShadeModel(GL_FLAT);

	renderManager.init("../Data/Render/configure.properties");
	renderManager.fog.Disable();
}