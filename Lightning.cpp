#include "Lightning.h"



Lightning::Lightning()
{
}


Lightning::~Lightning()
{
}

void Lightning::Init(GLuint program) {

	glUniform3fv(glGetUniformLocation(program, "ambientLight.ambient"), 1, &(glm::vec3(0, 0, 0))[0]);

	glUniform3fv(glGetUniformLocation(program, "directionalLight.direction"), 1, &(glm::vec3(1.0, 1.0, 0.0))[0]);
	glUniform3fv(glGetUniformLocation(program, "directionalLight.ambient"), 1, &(glm::vec3(0.0, 0.0, 0.0))[0]);
	glUniform3fv(glGetUniformLocation(program, "directionalLight.diffuse"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform3fv(glGetUniformLocation(program, "directionalLight.specular"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);

	glUniform3fv(glGetUniformLocation(program, "pointLight.position"), 1, &point_pos[0]);
	glUniform3fv(glGetUniformLocation(program, "pointLight.ambient"), 1, &(glm::vec3(0, 0, 0))[0]);
	glUniform3fv(glGetUniformLocation(program, "pointLight.diffuse"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform3fv(glGetUniformLocation(program, "pointLight.specular"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform1f(glGetUniformLocation(program, "pointLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "pointLight.linear"), 0.05f);
	glUniform1f(glGetUniformLocation(program, "pointLight.quadratic"), 0.02f);

	glUniform3fv(glGetUniformLocation(program, "spotLight.position"), 1, &(glm::vec3(0.0, 2.0, -1.0))[0]);
	glUniform1f(glGetUniformLocation(program, "spotLight.Cutoff"), 0.8f);
	glUniform1f(glGetUniformLocation(program, "spotLight.Exponent"), 0.8f);
	glUniform3fv(glGetUniformLocation(program, "spotLight.ambient"), 1, &(glm::vec3(0.1, 0.1, 0.1))[0]);
	glUniform3fv(glGetUniformLocation(program, "spotLightt.diffuse"), 1, &(glm::vec3(1.0, 0.0, 1.0))[0]);
	glUniform3fv(glGetUniformLocation(program, "spotLight.specular"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform3fv(glGetUniformLocation(program, "spotLight.spotDirection"), 1, &(glm::vec3(1.0, 1.0, 0.0))[0]);
	glUniform1f(glGetUniformLocation(program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "spotLight.linear"), 0.05f);
	glUniform1f(glGetUniformLocation(program, "spotLight.quadratic"), 0.02f);




}

void Lightning::Update(GLuint program, GLFWwindow* window) {



	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		GLuint ambient_ambient = glGetUniformLocation(program, "ambientLight.ambient");
		switch (directional_activ)
		{
		case true: {
			ambient_activ = false;
			glUniform3f(ambient_ambient, 0.0, 0.0, 0.0);
			break;
		}

		case false: {
			ambient_activ = true;
			glUniform3f(ambient_ambient, 1, 1, 1);
			break;
		}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		//glUniform3fv(glGetUniformLocation(program, "directionalLight.direction"), 1, &(glm::vec3(1.0, 0.0, 0.0))[0]);
		GLint directional_ambient = glGetUniformLocation(program, "directionalLight.ambient");
		GLint directional_diffuse = glGetUniformLocation(program, "directionalLight.diffuse");
		GLint directional_specular = glGetUniformLocation(program, "directionalLight.specular");

		switch (directional_activ)
		{
		case true:
			directional_activ = false;
			glUniform3f(directional_ambient, 0.0, 0.0, 0.0);
			glUniform3f(directional_diffuse, 0.0, 0.0, 0.0);
			glUniform3f(directional_specular, 0.0, 0.0, 0.0);
			break;

		case false:
			directional_activ = true;
			glUniform3f(directional_ambient, 0.2, 0.2, 0.2);
			glUniform3f(directional_diffuse, 1.0, 1.0, 1.0);
			glUniform3f(directional_specular, 1.0, 1.0, 1.0);
			break;
		}

	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		GLint point_ambient = glGetUniformLocation(program, "pointLight.ambient");
		GLint point_diffuse = glGetUniformLocation(program, "pointLight.diffuse");
		GLint point_specular = glGetUniformLocation(program, "pointLight.specular");

		switch (point_activ)
		{
		case true:
			point_activ = false;
			glUniform3f(point_ambient, 0.0, 0.0, 0.0);
			glUniform3f(point_diffuse, 0.0, 0.0, 0.0);
			glUniform3f(point_specular, 0.0, 0.0, 0.0);
			break;


		case false:
			point_activ = true;
			glUniform3f(point_ambient, 0.2, 0.2, 0.2);
			glUniform3f(point_diffuse, 1.0, 1.0, 1.0);
			glUniform3f(point_specular, 1.0, 1.0, 1.0);
			break;

		}
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		GLint spot_ambient = glGetUniformLocation(program, "spotLight.ambient");
		GLint spot_diffuse = glGetUniformLocation(program, "spotLight.diffuse");
		GLint spot_specular = glGetUniformLocation(program, "spotLight.specular");

		switch (spot_activ)
		{
		case true:
			spot_activ = false;
			glUniform3f(spot_ambient, 0.0, 0.0, 0.0);
			glUniform3f(spot_diffuse, 0.0, 0.0, 0.0);
			glUniform3f(spot_specular, 0.0, 0.0, 0.0);
			break;

		case false:
			spot_activ = true;
			glUniform3f(spot_ambient, 1.0, 1.0, 1.0);
			glUniform3f(spot_diffuse, 1.0, 1.0, 1.0);
			glUniform3f(spot_specular, 1.0, 1.0, 1.0);
			break;
		}

	}
}
