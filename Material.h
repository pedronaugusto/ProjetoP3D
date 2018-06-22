using namespace std;
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#define GLEW_STATIC
#include "include/GL/glew.h"

#define GLFW_USE_DWM_SWAP_INTERVAL
#include "include/GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "include/glm/glm.hpp"


#ifndef PROJETOP3D_MATERIAL_H
#define PROJETOP3D_MATERIAL_H

class Material{
public:
	Material(const char *path);
	~Material();

	void Init();
private:
	float ns;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	float ni;
	float d;
	int illum;
	char path_to_tga[32];

	GLuint texture;

	void LoadMtl(const char * path);
	void LoadTexture(const char * path);
};

#endif
