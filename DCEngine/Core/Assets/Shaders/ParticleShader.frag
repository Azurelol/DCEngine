#version 330 core

//layout (location = 0) out vec4 gPosition;
//layout (location = 1) out vec4 gNormal;
out vec4 gColor;
in vec2 TexCoords; 
in vec3 WorldCoord;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
	//gPosition = vec4(WorldCoord, 1);
	//gNormal = vec4(0,1,0,1);
	gColor = spriteColor * texture(image, TexCoords);
}