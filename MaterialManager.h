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
	Material*	mats; // ��������������ͼ������
	int			current; // ���ڴ��ڼ��������ļ�
};

#endif