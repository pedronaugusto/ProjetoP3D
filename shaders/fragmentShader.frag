#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};
uniform AmbientLight ambientLight; // Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Direção da luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};
// Estrutura de uma fonte de luz pontual
struct PointLight	{
	vec3 position;		// Posição do ponto de luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenuação constante
	float linear;		// Coeficiente de atenuação linear
	float quadratic;	// Coeficiente de atenuação quadrática
};

// Estrutura de uma fonte de luz pontual
struct PointLight	{
	vec3 position;		// Posição do ponto de luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenuação constante
	float linear;		// Coeficiente de atenuação linear
	float quadratic;	// Coeficiente de atenuação quadrática
};

uniform PointLight pointLight[2]; // Duas fontes de luz pontual

// Estrutura de uma fonte de luz cónica
struct SpotLight {
	vec3 position;		// Posição do foco de luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenuação constante
	float linear;		// Coeficiente de atenuação linear
	float quadratic;	// Coeficiente de atenuação quadrática

	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

uniform SpotLight spotLight; // Fonte de luz cónica


in vec2 UV;

out vec3 color;

uniform sampler2D modelTexture;




vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
vec4 calcSpotLight(SpotLight light);

void main(){
    color = texture( modelTexture, UV ).rgb;
}