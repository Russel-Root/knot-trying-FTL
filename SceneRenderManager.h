#ifndef __SceneRenderManager_h__
#define __SceneRenderManager_h__

#include <stdlib.h>
#include <iostream>
#include "PropertyReader.h"
#include "Fog.h"
#include "Lighting.h"
#include "LinkedList.h"
#include "MaterialManager.h"

using namespace std;

class SceneRenderManager{
public:
	SceneRenderManager();
	~SceneRenderManager();

	void setupProperties(char* filename);
	
	void init(char* filename);
	void setMat(int n);
	void Rendering();


public:
	PropertyReader	pReader;
	Fog				fog;
	Lighting*		light;

	Material		mat;
	MaterialManager matManager;
};
#endif