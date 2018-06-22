#version 440 core

in vec2 UV;

out vec3 color;

uniform sampler2D modelTexture;

void main(){
    color = texture( modelTexture, UV ).rgb;
}