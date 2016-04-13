#version 330

layout (location = 0) in vec4 Vertex;

out vec2 TexCoords;

void main (void)
{
	TexCoords = Vertex.zw;
	gl_Position = vec4(Vertex.xy, 0 ,1);
}