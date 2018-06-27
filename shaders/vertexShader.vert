#version 440 core

uniform mat4 ModelView;
uniform mat4 ModelViewProjection;
uniform mat3 NormalMatrix;

// Coordenada local do vértice
layout(location = 0) in vec3 vPosition;
// Normal do vértice
layout(location = 1) in vec3 vNormal;

out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;
out vec3 textureVector;

void main(){
	// Posição do vértice em coordenadas de olho
	vertexPositionEyeSpace = (ModelView * vec4(vPosition, 1.0)).xyz;

	// Transformar a normal do vértice
	vNormalEyeSpace = normalize(NormalMatrix * vNormal);

	// Coordenada de textura
	textureVector = vPosition;

	// Posição final do vértice
    gl_Position =  ModelViewProjection * vec4(vertexPosition,1.0f);
}