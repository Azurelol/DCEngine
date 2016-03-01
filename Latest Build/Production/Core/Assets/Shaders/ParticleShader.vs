#version 330 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 color;
layout (location = 2) in mat4 model;

uniform mat4 view;
uniform mat4 projection;
uniform float CutMinX;
uniform float CutMinY;
uniform float CutMaxX;
uniform float CutMaxY;

out vec2 TexCoords; 
out vec4 fragColor;

void main()
{
	TexCoords = vertex.zw;
	if (TexCoords.x == 1)
	{
	  TexCoords.x = CutMaxX;
	}
	if (TexCoords.x == 0)
	{
	  TexCoords.x = CutMinX;
	}

	if (TexCoords.y == 1)
	{
	  TexCoords.y = CutMaxY;
	}
	if (TexCoords.y == 0)
	{
	  TexCoords.y = CutMinY;
	}

	fragColor = color;
	gl_Position = projection * view * model * vec4(vertex.xy, 0, 1);
}