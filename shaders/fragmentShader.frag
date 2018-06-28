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
	vec3 position;		// Posi��o do ponto de luz, espa�o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua��o constante
	float linear;		// Coeficiente de atenua��o linear
	float quadratic;	// Coeficiente de atenua��o quadr�tica
};
layout(location = 30) uniform PointLight pointLight;


// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Dire��o da luz, espa�o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};
layout(location = 40) uniform DirectionalLight directionalLight; // Fonte de luz direcional


/*
// Estrutura de uma fonte de luz c�nica
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
	//reflex�o luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	//reflex�o luz difusa.
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Dire��o inversa � da dire��o luz.
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	//reflex�o luz especular.
	vec3 V = normalize(-vPositionEye);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	//fonte de luz direcional para a cor final do fragmento.
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// C�lculo da reflex�o da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// reflex�o luz difusa.
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEye);
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	//reflex�o luz especular.
	vec3 V = normalize(-vPositionEye);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	// attenuation
	float dist = length(mat3(View) * light.position - vPositionEye);	// C�lculo da dist�ncia entre o ponto de luz e o v�rtice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// C�lculo da contribui��o da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (ambient + diffuse + specular));
}

		/*
		vec4 calculoLuzConica(LuzConica light)
		{
			cutoff = radians(clamp(light.Cutoff, 0.0, 45.0));
			
			//reflex�o luz ambiente.
			vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

			//reflex�o luz difusa.
			vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
			vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
			vec3 L = normalize(lightDirectionEyeSpace * (lightPositionEyeSpace - vPositionEyeSpace)); // dire��o luz, posi��o eye
			vec3 N = normalize(vNormalEyeSpace);
			float NdotL = max(dot(N, L), 0.0);
			vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
			
	         if(dot(N, L)> Cutoff)
			{
				//reflex�o especular.
				vec3 V = normalize(-vPositionEyeSpace);
				vec3 R = reflect(L, N);
				float RdotV = max(dot(R, V), 0.0);
				vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
				
				// atenua��o
				float dist = length(mat3(View) * light.position - vPositionEyeSpace);
				float atenuacao = 1.0*(spotdirection * position) / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
			}
			return (atenuacao * (ambient + diffuse + specular));

		}
		*/

