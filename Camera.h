using namespace std;
#include <vector>

#define GLEW_STATIC
#include "include/GL/glew.h"

#define GLFW_USE_DWM_SWAP_INTERVAL
#include "include/GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

#define WIDTH 1920
#define HEIGHT 1080


#ifndef PROJETOP3D_CAMERA_H
#define PROJETOP3D_CAMERA_H

class Camera
{
public:
	Camera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up);
	~Camera();

	void Init();
	void Update(GLFWwindow *window);

	glm::mat4 view_matrix;	
	glm::mat4 projection_matrix;
	glm::vec3 camera_pos;
private:

	glm::vec3 camera_target;
	glm::vec3 up;

	float speed;
	float vertical_speed;
	float radius;

	double current_frame;
	float last_frame = 0.0f;
	float delta_time = 0.0f;

	float angle;
};

#endif
