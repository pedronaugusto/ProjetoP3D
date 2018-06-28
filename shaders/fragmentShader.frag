#version 440 core

uniform mat4 View;

in vec3 vPositionEye;
in vec2 UV;
in vec3 vNormalEye;

uniform sampler2D textureSampler;
out vec4 fragColor;


struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

layout(location = 6) uniform Material material;



// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};
layout(location = 11) uniform AmbientLight ambientLight; // Fonte de luz ambiente global

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
layout(location = 30) uniform PointLight pointLight;


// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Direção da luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};
layout(location = 40) uniform DirectionalLight directionalLight; // Fonte de luz direcional


/*
// Estrutura de uma fonte de luz cónica
struct LuzConica {
	vec3 position;		
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
	float constant;		
	float linear;		
	float quadratic;	

	float Cutoff, Exponent;
	vec3 spotDirection;
};

layout(location = 50) uniform SpotLight spotLight;
*/


vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
//vec4 calculoLuzConica(LuzConica light);


vec4 light[4];

void main(){
	vec4 emissive = vec4(material.emissive, 1.0);

	//fonte de luz ambiente
	light[0] = calcAmbientLight(ambientLight);
	//fonte de luz direcional
	light[1] = calcDirectionalLight(directionalLight);
	//fonte de luz Pontual
	light[2] = calcPointLight(pointLight);
	//fonte de luz Conica
	//light[3] = calculoLuzConica(luzconica);

	fragColor = (emissive + light[0] + light[1]  + light[2] + light[3] ) * texture(textureSampler, UV).rgb;
}

vec4 calcAmbientLight(AmbientLight light) {
	//fonte de luz ambiente global, para a cor do objeto.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	//reflexão luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	//reflexão luz difusa.
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Direção inversa à da direção luz.
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	//reflexão luz especular.
	vec3 V = normalize(-vPositionEye);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	//fonte de luz direcional para a cor final do fragmento.
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// Cálculo da reflexão da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// reflexão luz difusa.
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEye);
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	//reflexão luz especular.
	vec3 V = normalize(-vPositionEye);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	// attenuation
	float dist = length(mat3(View) * light.position - vPositionEye);	// Cálculo da distância entre o ponto de luz e o vértice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// Cálculo da contribuição da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (ambient + diffuse + specular));
}

		/*
		vec4 calculoLuzConica(LuzConica light)
		{
			cutoff = radians(clamp(light.Cutoff, 0.0, 45.0));
			
			//reflexão luz ambiente.
			vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

			//reflexão luz difusa.
			vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
			vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
			vec3 L = normalize(lightDirectionEyeSpace * (lightPositionEyeSpace - vPositionEyeSpace)); // direção luz, posição eye
			vec3 N = normalize(vNormalEyeSpace);
			float NdotL = max(dot(N, L), 0.0);
			vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
			
	         if(dot(N, L)> Cutoff)
			{
				//reflexão especular.
				vec3 V = normalize(-vPositionEyeSpace);
				vec3 R = reflect(L, N);
				float RdotV = max(dot(R, V), 0.0);
				vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
				
				// atenuação
				float dist = length(mat3(View) * light.position - vPositionEyeSpace);
				float atenuacao = 1.0*(spotdirection * position) / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
			}
			return (atenuacao * (ambient + diffuse + specular));

		}
		*/

