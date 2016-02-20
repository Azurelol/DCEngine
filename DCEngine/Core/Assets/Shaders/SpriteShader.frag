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
	vec3 Direction;
	float InnerAngle;
	float OuterAngle;
	vec3 Position;
	mat4 Model;
};

uniform int isTexture;
uniform Light Lights[MaxNumLights];
uniform int numLights;
uniform sampler2D image;
uniform vec4 spriteColor;

in vec2 TexCoords;
in vec3 WorldCoords;
out vec4 color;

float GeneratePointLightValues(vec3 position, float range, float falloff)
{
	float luminesence = 0;
	float distance = length(WorldCoords - position);
	if(distance <= range)
	{
		if(falloff > 0)
			luminesence = (1 - distance/range) * (1 - falloff);
		else
			luminesence = 1;
	}
	return luminesence;
}

float GenerateSpotLightValues(vec3 position, float range, float falloff, vec3 direction,
	float innerAngle, float outerAngle, mat4 model)
{
	float luminesence = 0;
	float distance = length(WorldCoords - position);
	if(distance <= range)
	{
		if(falloff > 0)
			luminesence = (1 - distance/range) * (1 - falloff);
		else
			luminesence = 1;
	}
	else return 0;

	vec3 lightVector = (WorldCoords - position) / distance;
	float angleFalloff = 0;
	vec4 worldDirection = model * vec4(direction.xyz, 0);
	float angleDifference = abs(acos(dot(lightVector, normalize(worldDirection.xyz))));

	if(angleDifference > outerAngle)
		return 0;

	angleFalloff = (angleDifference - outerAngle) / (innerAngle - outerAngle);
	clamp(angleFalloff, 0, 1);
	clamp(luminesence, 0, 1);
	return luminesence * angleFalloff;
}

vec3 GenerateIlluminationValues(void)
{
	vec3 coefficients = vec3(0,0,0);
	for(int i = 0; i < numLights; ++i)
	{
		Light light = Lights[i];
		float diffI = 0;
		switch(light.LightType)
		{
		case 0:
			diffI = GeneratePointLightValues(light.Position, light.Range, light.Falloff);
			break;
		case 1:
			diffI = GenerateSpotLightValues(light.Position, light.Range, light.Falloff,
				light.Direction, light.InnerAngle, light.OuterAngle, light.Model);
			break;
		default:
			diffI = 1;
			break;
		}
		diffI *= light.Intensity;
		coefficients.x += light.Color.x * diffI;
		coefficients.y += light.Color.y * diffI;
		coefficients.z += light.Color.z * diffI;
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

	vec3 lightValue = vec3(1);
	if(Tmpcolor.a < 0.1)
    discard;
	if(numLights != 0)
	{
		lightValue = GenerateIlluminationValues();
		clamp(lightValue, 0, 1);
	}
	
  color = vec4(
	Tmpcolor.x * lightValue.x,
	Tmpcolor.y * lightValue.y,
	Tmpcolor.z * lightValue.z,
	Tmpcolor.w);
}
