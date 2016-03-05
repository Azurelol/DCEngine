#version 330 core

uniform sampler2D image;
in vec4 fragColor;
in vec2 TexCoords; 
out vec4 color;

void main()
{
	color = vec4(fragColor) * texture(image, TexCoords);
}