#include "Lightning.h"



Lightning::Lightning()
{
}


Lightning::~Lightning()
{
}

void Lightning::Init(GLuint program) {

	// Fonte de luz ambiente global
	glUniform3fv(glGetUniformLocation( program, "ambientLight.ambient"), 1, &(glm::vec3(0.1, 0.1, 0.1))[0]);

	// Fonte de luz direcional
	glUniform3fv( glGetUniformLocation( program, "directionalLight.direction"), 1, &(glm::vec3(1.0, 0.0, 0.0))[0]);
	glUniform3fv( glGetUniformLocation( program, "directionalLight.ambient"), 1, &(glm::vec3(0.2, 0.2, 0.2))[0]);
	glUniform3fv( glGetUniformLocation( program, "directionalLight.diffuse"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform3fv( glGetUniformLocation( program, "directionalLight.specular"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);

	// Fonte de luz pontual
	glUniform3fv( glGetUniformLocation( program, "pointLight.position"), 1, &point_pos[0]);
	glUniform3fv( glGetUniformLocation( program, "pointLight.ambient"), 1, &(glm::vec3(0.1, 0.1, 0.1))[0]);
	glUniform3fv( glGetUniformLocation( program, "pointLight.diffuse"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform3fv( glGetUniformLocation( program, "pointLight.specular"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform1f( glGetUniformLocation( program, "pointLight.constant"), 1.0f);
	glUniform1f( glGetUniformLocation( program, "pointLight.linear"), 0.06f);
	glUniform1f( glGetUniformLocation( program, "pointLight.quadratic"), 0.02f);

	/*
	// Fonte de luz Conica
	glUniform3fv( glGetUniformLocation( GL_UNIFORM, "spotLight.position"), 1, &(glm::vec3(0.0, 2.0, -1.0))[0]);
	glUniform1f( glGetUniformLocation( GL_UNIFORM, "spotLight.cutOff"), 0.8f);
	glUniform1f( glGetUniformLocation( GL_UNIFORM, "spotLight.outerCutOff"), 0.8f);
	glUniform3fv( glGetUniformLocation( GL_UNIFORM, "spotLight.ambient"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform3fv( glGetUniformLocation( GL_UNIFORM, "spotLightt.diffuse"), 1, &(glm::vec3(1.0, 0.0, 1.0))[0]);
	glUniform3fv( glGetUniformLocation( GL_UNIFORM, "spotLight.specular"), 1, &(glm::vec3(1.0, 1.0, 1.0))[0]);
	glUniform3fv( glGetUniformLocation( GL_UNIFORM, "spotLight.direction"), 1, &(glm::vec3(0.0, -1.0, 0.0))[0]);
	glUniform1f( glGetUniformLocation( GL_UNIFORM, "spotLight.constant"), 1.0f);
	glUniform1f( glGetUniformLocation( GL_UNIFORM, "spotLight.linear"), 0.06f);
	glUniform1f( glGetUniformLocation( GL_UNIFORM, "spotLight.quadratic"), 0.02f);
	*/

}

void Lightning::Update(GLuint program, GLFWwindow* window) {
	
	int AmbientLightB = glfwGetKey(window, GLFW_KEY_1);
	int DirectionaltLightB = glfwGetKey(window, GLFW_KEY_2);
	int PonctualLightB = glfwGetKey(window, GLFW_KEY_3);



	if (AmbientLightB == GLFW_PRESS) {
		GLuint ambientLightU = glGetUniformLocation(program, "ambientLight.ambient");
		switch (ambientLight)
		{
			case true:
				ambientLight = false;
				glUniform3f(ambientLightU, 0.0, 0.0, 0.0);
				break;
			case false:
				ambientLight = true;
				glUniform3f(ambientLightU, 1.0, 1.0, 1.0);
				break;
		
		}

	}

	else if (DirectionaltLightB == GLFW_PRESS) {
		GLint directLightUA = glGetUniformLocation(program, "directionalLight.ambient");
		GLint directLightUD = glGetUniformLocation(program, "directionalLight.diffuse");
		GLint directLightUS = glGetUniformLocation(program, "directionalLight.specular");

		switch (directLight)
		{
			case true: 
				directLight = false;
				glUniform3f(directLightUA, 0.0, 0.0, 0.0);
				glUniform3f(directLightUD, 0.0, 0.0, 0.0);
				glUniform3f(directLightUS, 0.0, 0.0, 0.0);
				break;

			case false: 
				directLight = true;
				glUniform3f(directLightUA, 1.0, 1.0, 1.0);
				glUniform3f(directLightUD, 2.0, 2.0, 2.0);
				glUniform3f(directLightUS, 1.0, 1.0, 1.0);
				break;
		}

	}

	else if (PonctualLightB == GLFW_PRESS) {
		GLint ponctualLightUA = glGetUniformLocation(program, "pointLight.ambient");
		GLint ponctualLightUD = glGetUniformLocation(program, "pointLight.diffuse");
		GLint ponctualLightUS = glGetUniformLocation(program, "pointLight.specular");

		switch (ponctualLight)
		{
			case true: 
				ponctualLight = false;
				glUniform3f(ponctualLightUA, 0.0, 0.0, 0.0);
				glUniform3f(ponctualLightUD, 0.0, 0.0, 0.0);
				glUniform3f(ponctualLightUS, 0.0, 0.0, 0.0);
				break;
		

			case false: 
				ponctualLight = true;
				glUniform3f(ponctualLightUA, 1.0, 1.0, 1.0);
				glUniform3f(ponctualLightUD, 2.0, 2.0, 2.0);
				glUniform3f(ponctualLightUS, 1.0, 1.0, 1.0);
			break;
		
		}
	}
	/*
	else if (SpotLightB == GLFW_PRESS) {
		GLint spotLightUA = glGetUniformLocation(program, "spotLight.ambient");
		GLint spotLightUD = glGetUniformLocation(program, "spotLight.diffuse");
		GLint spotLightUS = glGetUniformLocation(program, "spotLight.specular");

		switch (spotLight)
		{
			case true: 
				spotLight = false;
				glUniform3f(spotLightUA, 0.0, 0.0, 0.0);
				glUniform3f(spotLightUD, 0.0, 0.0, 0.0);
				glUniform3f(spotLightUS, 0.0, 0.0, 0.0);
				break;

			case false:
				spotLight = true;
				glUniform3f(spotLightUA, 1.0, 1.0, 1.0);
				glUniform3f(spotLightUD, 10.0, 10.0, 10.0);
				glUniform3f(spotLightUS, 1.0, 1.0, 1.0);
				break;
		}
		
	}
	*/
}
