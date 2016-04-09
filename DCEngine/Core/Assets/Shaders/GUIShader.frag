#version 330

uniform sampler2D Texture;
in vec2 FragUV;
in vec4 FragColor;
out vec4 OutColor;
void main()
{

    OutColor = FragColor * texture(Texture, FragUV.st);
}