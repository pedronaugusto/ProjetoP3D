#include "Camera.h"


Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up){
	this->camera_pos = cameraPos;
	this->camera_target = cameraTarget;
	this->up = up;
}

Camera::~Camera(){

}

void Camera::Init() {
	speed = 1.5f;
	vertical_speed = 5.0f;
	radius = 8.0f;

	last_frame = 0.0f;
	delta_time = 0.0f;

	angle = 0.0f;

	view_matrix = glm::lookAt(
		camera_pos,
		camera_target,
		up
	);
	//ProjectionMatrix//
	float fov = 45; //field of view
	float aspect_ratio = (float)WIDTH / (float)HEIGHT; //aspectRatio
	float near = 0.1f; //near
	float far = 100; //far
	projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
}

void Camera::Update(GLFWwindow *window) {

	current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_pos.y += vertical_speed * delta_time;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_pos.y -= vertical_speed * delta_time;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		angle -= speed * delta_time;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		angle += speed * delta_time;
	}

	camera_pos.x = sin(angle) * radius;
	camera_pos.z = cos(angle) * radius;

	if (camera_pos.y < -7.0f) camera_pos.y = -7.0f;
	else if (camera_pos.y > 10.0f) camera_pos.y = 10.0f;

	view_matrix = glm::lookAt(
		camera_pos,
		camera_target,
		up
	);
}
