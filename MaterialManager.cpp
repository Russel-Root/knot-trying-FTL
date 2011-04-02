#include "MaterialManager.h"

MaterialManager::MaterialManager(){
	current = 0;
}

MaterialManager::~MaterialManager(){}

Material MaterialManager::getMatAt(int n){
	return mats[n];
}

void MaterialManager::setSize(int n){
	mats = new Material[n];
}

bool MaterialManager::addMat(const char* filename){
	PropertyReader pReader;
	if ( !pReader.readConfigFile(filename) )
		return false;

	Material mat;
	mat.setAmbient(atof(pReader.getProperty("matAmbientR")), atof(pReader.getProperty("matAmbientG")), 
				   atof(pReader.getProperty("matAmbientB")), atof(pReader.getProperty("matAmbientA")));
	mat.setDiffuse(atof(pReader.getProperty("matDiffuseR")), atof(pReader.getProperty("matDiffuseG")),
				   atof(pReader.getProperty("matDiffuseB")), atof(pReader.getProperty("matDiffuseA")));
	mat.setSpecular(atof(pReader.getProperty("matSpecularR")), atof(pReader.getProperty("matSpecularG")),
				    atof(pReader.getProperty("matSpecularB")), atof(pReader.getProperty("matSpecularA")));
	mat.setShininess(atof(pReader.getProperty("matShininess")));
	mat.setNum(atof(pReader.getProperty("num")));

	// set the textures of the mat
	char string[20];
	for ( int i = 0; i < atoi(pReader.getProperty("num")); i++ ){
		sprintf_s(string, "texture%d", i);
		mat.addTexture(pReader.getProperty(string));
	}

	mats[current] = mat;
	current++;
	return true;
}