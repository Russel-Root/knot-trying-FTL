#ifndef __MaterialManager_h__
#define __MaterialManager_h__

#include <stdlib.h>
#include "Material.h"
#include "PropertyReader.h"

class MaterialManager{
public:
	MaterialManager();
	~MaterialManager();

	void setSize(int n);
	Material getMatAt(int n);
	bool addMat(const char* filename);

private:
	Material*	mats; // 储存所有纹理贴图的链表
	int			current; // 现在存在几个材质文件
};

#endif