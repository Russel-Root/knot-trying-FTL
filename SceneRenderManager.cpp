#include "SceneRenderManager.h"

SceneRenderManager::SceneRenderManager(void)
{
}

SceneRenderManager::~SceneRenderManager(void)
{
}

/*************************************************
 *���������ļ��е��������ԣ������������Լ����趨
 *************************************************/
void SceneRenderManager::setupProperties(char* filename)
{
	if(!pReader.readConfigFile(filename))
		return;

	//set the fog properties
	fog.setDensity(atof(pReader.getProperty("fogDensity")));
	fog.setColor(atof(pReader.getProperty("fogColorR")), atof(pReader.getProperty("fogColorG")), 
				 atof(pReader.getProperty("fogColorB")), atof(pReader.getProperty("fogColorA")));
	fog.setRange(atof(pReader.getProperty("fogStart")), atof(pReader.getProperty("fogEnd")));

	//get the light number
	int num = atoi(pReader.getProperty("lightNum"));
	light = new Lighting[num];
	//set each light
	int type;
	int setNum;
	float ambientR, ambientG, ambientB, ambientA;
	float diffuseR, diffuseG, diffuseB, diffuseA;
	float speculaR, speculaG, speculaB, speculaA;
	float position_x, position_y, position_z;
	char string[20];
	for(int i = 0; i<num; i++)
	{
		//set light type
		sprintf_s(string,"type%d", i);
		type = atoi(pReader.getProperty(string));
		light[i].setType(type);
		//set light num
		sprintf_s(string, "light%d", i);
		sscanf_s(pReader.getProperty(string),"%x", &setNum);
		light[i].setLightNum(setNum);
		//set position
		sprintf_s(string, "light%dPosX", i);
		position_x = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dPosY", i);
		position_y = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dPosZ", i);
		position_z = atof(pReader.getProperty(string));
		light[i].setPosition(position_x, position_y, position_z);
		//set the ambient light		
		sprintf_s(string, "light%dAmbientR", i);
		ambientR = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dAmbientG", i);
		ambientG = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dAmbientB", i);
		ambientB = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dAmbientA", i);
		ambientA = atof(pReader.getProperty(string));
		light[i].setAmbient(ambientR, ambientG, ambientB, ambientA);
		//set the diffuse		
		sprintf_s(string, "light%dDiffuseR", i);
		diffuseR = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dDiffuseG", i);
		diffuseG = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dDiffuseB", i);
		diffuseB = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dDiffuseA", i);
		diffuseA = atof(pReader.getProperty(string));
		light[i].setDiffuse(diffuseR, diffuseG, diffuseB, diffuseA);
		//set the specular light			
		sprintf_s(string, "light%dSpecularR", i);
		speculaR = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dSpecularG", i);
		speculaG = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dSpecularB", i);
		speculaB = atof(pReader.getProperty(string));
		sprintf_s(string, "light%dSpecularA", i);
		speculaA = atof(pReader.getProperty(string));
		light[i].setSpecular(speculaR, speculaG, speculaB, speculaA);
	}

	//set the material and texture file
	int matNum = atoi(pReader.getProperty("matNum"));
	matManager.setSize(matNum);
	for(int i = 0; i< matNum; i++)
	{
		sprintf_s(string, "material%d", i);
		matManager.addMat(pReader.getProperty(string));
	}
}
/********************************************
 ����������������еĵڼ�����������Ϊ��ǰ����
 ********************************************/
void SceneRenderManager::setMat(int n)
{
	mat = matManager.getMatAt(n);
	//����Ĭ������
	mat.setMaterial();
	mat.enableTexture(0);
}

/***********
 *��ʼ������
 ***********/
void SceneRenderManager::init(char* filename)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_SMOOTH);

	//��ʼ��ʱ��������ȾЧ������
	setupProperties(filename);
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);								//��յ�ɫΪǳ��ɫ
	fog.Enable();														//������Ч��

	//glEnable(GL_LIGHTING);
	//for(int i = 0; i< atoi(pReader.getProperty("lightNum")); i++)
	//	light[i].Enable();												//�������й���

	//���õ�һ�����ص�������ͼ������ΪĬ�ϵ���������
	mat = matManager.getMatAt(0);
}

void SceneRenderManager::Rendering()
{	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//������Ч��
	fog.setFog();
	////�������й���
	//for(int i = 0; i<atoi(pReader.getProperty("lightNum")); i++)
	//	light[i].setLight();
}