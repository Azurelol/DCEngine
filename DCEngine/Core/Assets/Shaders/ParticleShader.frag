#version 330 core

uniform sampler2D image;
in vec4 fragColor;
in vec2 TexCoords; 
in vec3 WorldCoord;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

void main()
{
	gPosition = vec4(WorldCoord, 1);
	gNormal = vec4(0,1,0,1);
	gColor = vec4(fragColor) * texture(image, TexCoords);
}