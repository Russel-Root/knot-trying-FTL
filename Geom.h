#ifndef __Geom_h__
#define __Geom_h__

// this headerfile contains the virtual class Geom, and its subclasses derived from class Geom

#include "RenderImp.h"
class RenderImp;

//////////////////////////////////////////////////////////////////////////
class Geom{
public:
	Geom();
	Geom(const Geom& copy_obj);
	~Geom();

	virtual void AcceptDrawer(RenderImp* imp);
};

//////////////////////////////////////////////////////////////////////////
class SphereGeom : public Geom{
public:
	SphereGeom();
	SphereGeom(const SphereGeom& copy_obj);
	~SphereGeom();

	void AcceptDrawer(RenderImp* imp);
	
	// 上面的是继承父类的函数，下面的是新增函数
	SphereGeom(float x, float y, float z, float _r);
	
	void SetCenter(float x, float y, float z){ center[0] = x; center[1] = y; center[2] = z; }
	void GetCenter(float* c){ c[0] = center[0]; c[1] = center[1]; c[2] = center[2]; }

	void SetRadius(float _r){ r = _r; }
	float GetRadius(){ return r; }
private:
	float center[3];
	float r;
};

//////////////////////////////////////////////////////////////////////////
class MeshGeom : public Geom{
public:
	MeshGeom();
	MeshGeom(const MeshGeom& copy_obj);
	~MeshGeom();

	void AcceptDrawer(RenderImp* imp);

	// next is the new classes, not derived classes
	MeshGeom(MeshStruct* _m);
	void SetMesh(MeshStruct* _m);
	MeshStruct* GetMesh(){ return mesh; }
private:
	MeshStruct* mesh;
	int tag_mesh;
};

#endif