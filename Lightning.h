using namespace std;
#include <iostream>

#define GLEW_STATICw
#include "include/GL/glew.h"

#define GLFW_USE_DWM_SWAP_INTERVAL
#include "include/GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "include/glm/glm.hpp"
#include "include/glm/gtc/type_ptr.hpp"

#ifndef PROJETOP3D_LIGHTNING_H
#define PROJETOP3D_LIGHTNING_H

class Lightning
{
public:
	Lightning();
	~Lightning();

	void Init(GLuint program);
	void Update(GLuint program, GLFWwindow *window);

private:
	glm::vec3 point_pos = glm::vec3(0.0f, 2.0f, -2.5f);
	bool ambientLight = false;
	bool directLight = false;
	bool ponctualLight = false;
	bool spotLight; //tochange
};

#endif
