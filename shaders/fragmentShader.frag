#version 440 core

uniform mat4 View;

in vec3 vPositionEye;
in vec2 UV;
in vec3 vNormalEye;

uniform sampler2D textureSampler;
layout (location = 0) out vec4 fragColor;


struct Mat{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

 uniform Mat mat;



// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};
uniform AmbientLight ambientLight; // Fonte de luz ambiente global

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
uniform PointLight pointLight;


// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Direção da luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};
uniform DirectionalLight directionalLight;


struct SpotLight {
	vec3 position;		
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
	float constant;		
	float linear;		
	float quadratic;	

	float cutOff, exponent;
	vec3 spotDirection;
};

uniform SpotLight spotLight;



vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
vec4 calcSpotLight(SpotLight light);


vec4 light[4];

void main(){
	vec4 emissive = vec4(mat.emissive, 1.0);

	light[0] = calcAmbientLight(ambientLight);
	light[1] = calcDirectionalLight(directionalLight);
	light[2] = calcPointLight(pointLight);
	light[3] = calcSpotLight(spotLight);

	fragColor = (light[0] + light[1]  + light[2] + light[3] ) * texture(textureSampler, UV);
}

vec4 calcAmbientLight(AmbientLight light) {
	vec4 ambient = vec4(mat.ambient + light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	// ambient
	vec4 ambient = vec4(mat.ambient + light.ambient, 1.0);
	// luz difusa
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Direção inversa à da direção luz.
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(mat.diffuse * light.diffuse, 1.0) * NdotL;
	
	// luz especular
	vec3 V = normalize(-vPositionEye);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, mat.shininess) * vec4(light.specular * mat.specular, 1.0);

	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// ambient
	vec4 ambient = vec4(mat.ambient + light.ambient , 1.0);
	// luz difusa
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEye);
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(mat.diffuse , 1.0) * NdotL;

	// luz especular
	vec3 V = normalize(-vPositionEye);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, mat.shininess) * vec4(mat.specular * light.specular, 1.0);
	
	// attenuattion
	float dist = length(mat3(View) * light.position - vPositionEye);	
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (attenuation * (ambient + diffuse + specular));
}

		
vec4 calcSpotLight(SpotLight light)
{

	// ambient
	vec4 ambient = vec4(mat.ambient + light.ambient, 1.0);
	// luz difusa
	vec3 lightDirectionEyeSpace = (View * vec4(light.spotDirection, 0.0)).xyz;
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightDirectionEyeSpace * (lightPositionEyeSpace - vPositionEye)); // direção luz, posição eye
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(mat.diffuse +  light.diffuse, 1.0) * NdotL;
	// luz especular
	vec3 V = normalize(-vPositionEye);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, mat.shininess) * vec4(light.specular * mat.specular, 1.0);

	// cutoff
	float theta = dot(lightPositionEyeSpace, normalize(-light.spotDirection));
	float epsilon = (light.cutOff - light.exponent);
	float intensity = clamp((theta - light.exponent) / epsilon, 0.0,1.0);
	diffuse *= intensity;
	specular *= intensity;
	// attenuattion
	float dist = length(mat3(View) * light.position - vPositionEye);	
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

			
	return (attenuation * (ambient + diffuse + specular));

}
		

