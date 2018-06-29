#version 440 core
			
in vec3 vertexPosition_modelspace;
in vec2 vertexUV;
in vec3 vertexNormal;


out vec3 vPositionEye;
out vec2 UV;
out vec3 vNormalEye;


uniform mat3 NormalMatrix;
uniform mat4 ModelView;
uniform mat4 MVP;
uniform mat4 View;


void main(){
    vPositionEye = (ModelView * vec4(vertexPosition_modelspace,1.0)).xyz;
    vNormalEye = normalize(NormalMatrix * vertexNormal);
	
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    UV = vertexUV;
}