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
	vec3 position;		// Posi��o do ponto de luz, espa�o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua��o constante
	float linear;		// Coeficiente de atenua��o linear
	float quadratic;	// Coeficiente de atenua��o quadr�tica
};
uniform PointLight pointLight;


// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Dire��o da luz, espa�o do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};
uniform DirectionalLight directionalLight; // Fonte de luz direcional


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

uniform SpotLight spotLight;
*/


vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
//vec4 calculoLuzConica(LuzConica light);


vec4 light[4];

void main(){
	vec4 emissive = vec4(mat.emissive, 1.0);

	//fonte de luz ambiente
	light[0] = calcAmbientLight(ambientLight);
	//fonte de luz direcional
	light[1] = calcDirectionalLight(directionalLight);
	//fonte de luz Pontual
	light[2] = calcPointLight(pointLight);
	//fonte de luz Conica
	//light[3] = calculoLuzConica(luzconica);

	fragColor = (emissive + light[0] + light[1]  + light[2] + light[3] ) * texture(textureSampler, UV);
}

vec4 calcAmbientLight(AmbientLight light) {
	// C�lculo da contribui��o da fonte de luz ambiente global, para a cor do objeto.
	vec4 ambient = vec4(mat.ambient + light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	// C�lculo da reflex�o da componente da luz ambiente.
	vec4 ambient = vec4(mat.ambient + light.ambient, 1.0);

	// C�lculo da reflex�o da componente da luz difusa.
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Dire��o inversa � da dire��o luz.
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(mat.diffuse * light.diffuse, 1.0) * NdotL;
	
	// C�lculo da reflex�o da componente da luz especular.
	// Como os c�lculos est�o a ser realizados nas coordenadas do olho, ent�o a c�mara est� na posi��o (0,0,0).
	// Resulta ent�o:
	//		(0,0,0) - vPositionEye
	// Que pode ser simplificado como:
	//		- vPositionEye
	vec3 V = normalize(-vPositionEye);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, mat.shininess) * vec4(light.specular * mat.specular, 1.0);

	// C�lculo da contribui��o da fonte de luz direcional para a cor final do fragmento.
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// C�lculo da reflex�o da componente da luz ambiente.
	vec4 ambient = vec4(light.ambient , 1.0);

	// C�lculo da reflex�o da componente da luz difusa.
	//vec3 lightPositionEyeSpace = mat3(View) * light.position;
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEye);
	vec3 N = normalize(vNormalEye);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(mat.diffuse , 1.0) * NdotL;

	// C�lculo da reflex�o da componente da luz especular.
	// Como os c�lculos est�o a ser realizados nas coordenadas do olho, ent�o a c�mara est� na posi��o (0,0,0).
	// Resulta ent�o:
	//		(0,0,0) - vPositionEye
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEye);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, mat.shininess) * vec4(mat.specular, 1.0);
	
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
			vec4 ambient = vec4(mat.ambient * light.ambient, 1.0);

			//reflex�o luz difusa.
			vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
			vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
			vec3 L = normalize(lightDirectionEyeSpace * (lightPositionEyeSpace - vPositionEye)); // dire��o luz, posi��o eye
			vec3 N = normalize(vNormalEye);
			float NdotL = max(dot(N, L), 0.0);
			vec4 diffuse = vec4(mat.diffuse * light.diffuse, 1.0) * NdotL;
			
	         if(dot(N, L)> Cutoff)
			{
				//reflex�o especular.
				vec3 V = normalize(-vPositionEye);
				vec3 R = reflect(L, N);
				float RdotV = max(dot(R, V), 0.0);
				vec4 specular = pow(RdotV, mat.shininess) * vec4(light.specular * mat.specular, 1.0);
				
				// atenua��o
				float dist = length(mat3(View) * light.position - vPositionEye);
				float atenuacao = 1.0*(spotdirection * position) / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
			}
			return (atenuacao * (ambient + diffuse + specular));

		}
		*/

