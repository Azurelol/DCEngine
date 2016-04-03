// Fragment shader used by 'Sprite'
#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColor;

in vec2 TexCoords;
in vec3 WorldCoords; 
in vec3 WorldNormal;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform int isTexture;

void main()
{
	vec4 Tmpcolor;
  if (isTexture == 1) 
    Tmpcolor = spriteColor * texture(image, TexCoords);
  else 
    Tmpcolor = spriteColor;

	if(Tmpcolor.a < 0.01)
    discard;

  // Store the fragment position vector in the first gbuffer texture
  gPosition = vec4(WorldCoords, 1);
  // Also store the per-fragment normals into the gbuffer
  gNormal = vec4(normalize(WorldNormal), 1);
  // And the diffuse per-fragment color
  gColor = Tmpcolor;
}