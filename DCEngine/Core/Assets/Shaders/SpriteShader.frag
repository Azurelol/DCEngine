// Fragment shader used by 'Sprite'
#version 330 core

// We take a texture and color vector that both affect the final color of the
// fragment. 

const int MaxNumLights = 20;

struct Light
{
	bool Visible;
	bool VisibilityCulling;
	bool VisibilityEvents;
	bool CastShadows;
	int LightType;

	vec4 Color;
	float Intensity;
	float Range;
	float Falloff;

	float Angle;
	float Size;
	vec3 Position;
};

uniform int isTexture;
uniform Light Lights[MaxNumLights];
uniform int numLights;
uniform sampler2D image;
uniform vec4 spriteColor;

in vec2 TexCoords;
in vec2 WorldCoords;
out vec4 color;

float distance(vec2 pointA, vec2 pointB)
{
	float a = pointB.x - pointA.x;
	float b = pointB.y - pointA.y;
	return sqrt((a * a) + (b * b));
}

vec3 GenerateIlluminationValues(void)
{
	vec3 coefficients = vec3(0,0,0);
	for(int i = 0; i < numLights; ++i)
	{
		float diffI = 0;
		float distance = distance(WorldCoords, Lights[i].Position.xy);
		if(distance <= Lights[i].Range)
		{
			if(Lights[i].Falloff > 0)
				diffI = (1 - distance/Lights[i].Range) * (1 - Lights[i].Falloff);
			else
				diffI = 1;
		}
		coefficients.x += Lights[i].Color.x * diffI * Lights[i].Intensity;
		coefficients.y += Lights[i].Color.y * diffI * Lights[i].Intensity;
		coefficients.z += Lights[i].Color.z * diffI * Lights[i].Intensity;
	}
	return coefficients;
}

void main()
{
  vec4 Tmpcolor;

  if (isTexture == 1) 
  {
    Tmpcolor = vec4(spriteColor) * texture(image, TexCoords);
  }
  else 
  {
    Tmpcolor = vec4(spriteColor);
  }
  if(Tmpcolor.a < 0.1)
    discard;

	vec3 lightValue = GenerateIlluminationValues();
	clamp(lightValue, 0, 1);
  color = vec4(
	Tmpcolor.x * lightValue.x,
	Tmpcolor.y * lightValue.y,
	Tmpcolor.z * lightValue.z,
	Tmpcolor.w);
}
