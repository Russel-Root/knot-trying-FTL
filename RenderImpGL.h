#ifndef __RenderImpGL_h__
#define __RenderImpGL_h__

#include "SceneRenderManager.h"

class RenderImpGL{
public:
	RenderImpGL();
	~RenderImpGL();

	void init();
public:
	SceneRenderManager	renderManager;
};

#endif