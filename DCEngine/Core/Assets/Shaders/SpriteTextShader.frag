#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 WorldCoords;
in vec2 TexCoords;

uniform sampler2D image; 
uniform vec4 spriteColor;

void main()
{
    float sampled = texture(image, TexCoords).r;
    if(sampled < 0.1)
      discard;
	gPosition = vec4(WorldCoords, 1);
	gNormal = vec4(0,1,0,1);
	gColor = vec4(spriteColor.rgb, spriteColor.a * sampled);
}