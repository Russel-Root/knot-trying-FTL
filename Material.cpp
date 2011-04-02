#include "Material.h"

Material::Material(){
	ambientR = ambientG = ambientB = ambientA = 0.0f;
	diffuseR = diffuseG = diffuseB = diffuseA = 0.0f;
	specularR = specularG = specularB = specularA = 0.0f;
	shininess = 0.0f;
	current = 0;
}

Material::~Material(){}

void Material::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
	ambientR = r;
	ambientG = g;
	ambientB = b;
	ambientA = a;
}

void Material::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
	diffuseR = r;
	diffuseG = g;
	diffuseB = b;
	diffuseA = a;
}

void Material::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
	specularR = r;
	specularG = g;
	specularB = b;
	specularA = a;
}

void Material::setShininess(GLfloat s){
	shininess = s;
}

void Material::setMaterial(){
	GLfloat mat_ambient[] = {ambientR, ambientG, ambientB, ambientA};
	GLfloat mat_diffuse[] = {diffuseR, diffuseG, diffuseB, diffuseA};
	GLfloat mat_specular[] = {specularR, specularG, specularB, specularA};
	GLfloat mat_shininess[] = {shininess};

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void Material::setTransparent(bool transp){
	if (transp){
		glEnable(GL_BLEND); // 启动纹理混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 基于源像素alpha通道值的半透明混合函数
		glDepthMask(GL_FALSE);
	}
	else{
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

void Material::setNum(int n){
	num = n;
	g_texture = new GLuint[n];
}

bool Material::addTexture(const char* filename){
	if ( !g_texture )
		return false;
	Image* temp = loadBMP(filename);

	// 如果该贴图不存在，返回false
	if ( !temp )
		return false;

	// bind the texture
	glGenTextures(1, &g_texture[current]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, g_texture[current]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, temp->height, temp->width, 0, GL_RGB, GL_UNSIGNED_BYTE, temp->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, temp->height, temp->width, GL_RGB, GL_UNSIGNED_BYTE, temp->pixels);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// deinit the memory
	if (temp){
		if (temp->pixels)
			free(temp->pixels);
		free(temp);
	}
	current++;
	return true;
}

void Material::enableTexture(int k){
	glEnable(GL_TEXTURE_2D);
	if ( k < current )
		glBindTexture(GL_TEXTURE_2D, g_texture[k]);
}