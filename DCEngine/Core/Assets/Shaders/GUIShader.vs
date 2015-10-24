#version 330 core
  
uniform mat4 ProjMtx;
in vec2 Position;
in vec2 UV;
in vec4 Color;

out vec2 FragUV;
out vec4 FragColor;

void main()
{
    FragUV = UV;
    FragColor = Color;
    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
}