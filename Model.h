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

#include "Material.h"


#ifndef PROJETOP3D_MODEL_H
#define PROJETOP3D_MODEL_H

class Model{
public:
	Model(const char *path);
	~Model();

	void Init(GLuint program);
	void Update();
	void Draw(GLuint program);
	
	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint uv_buffer;
	GLuint normal_buffer;

private:

	vector < glm::vec3 > vertices;
	vector < glm::vec2 > uvs;
	vector < glm::vec3 > normals;
	char path_to_mtl[32];

	vector < glm::vec3 > temp_vertices;
	vector < glm::vec2 > temp_uvs;
	vector < glm::vec3 > temp_normals;

	vector< unsigned short > vertex_indices, uv_indices, normal_indices;

	void LoadObj(const char * path);
	void GetIndicesFromLine(string first_indices, string second_indices, string third_indices);
	void MakeIndexedVerticesUvsNormals();
};

#endif

