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
uniform Light gLight;
uniform bool useLight;
uniform int numLights;
uniform Light Lights[MaxNumLights];
uniform sampler2D image;
uniform vec4 spriteColor;

in vec2 TexCoords;
in vec3 WorldCoords;
in vec3 VertWorldNormal;
out vec4 color;

float GenerateZ0DiffuseFactor(vec3 lightPosition)
{
	if(lightPosition.z == 0 && WorldCoords.z == 0)
	{
		vec3 lightVector = normalize(lightPosition - WorldCoords);
		vec3 normal = normalize(VertWorldNormal);
		float diffuseFactor = dot(lightVector, normal) + .25;
		return clamp(diffuseFactor, 0, 1);
	}
	else
	{
		return 1;
	}
}

float GenerateFalloffFactor(float distance, float range, float falloff)
{
	float factor;
	if(distance <= range)
	{
		if(falloff > 0)
			factor = (1 - distance/range) / (falloff * falloff);
		else
			factor = 1;
		return clamp(factor, 0, 1);
	}
	else return 0;
}

float GeneratePointLightValues(vec3 position, float range, float falloff)
{
	float diffuseFactor = GenerateZ0DiffuseFactor(position);
	float distance = length(WorldCoords - position);
	float distanceAttenuation = GenerateFalloffFactor(distance, range, falloff);
	return distanceAttenuation * diffuseFactor;
}

float GenerateSpotLightValues(vec3 position, float range, float falloff, vec3 direction,
	float innerAngle, float outerAngle, mat4 model)
{
	float diffuseFactor = GenerateZ0DiffuseFactor(position);
	float distance = length(WorldCoords - position);
	float distanceAttenuation = GenerateFalloffFactor(distance, range, falloff);

	vec3 lightVector = (WorldCoords - position) / distance;
	float angleFalloff = 0;
	vec4 worldDirection = model * vec4(direction.xyz, 0);
	float angleDifference = abs(acos(dot(lightVector, normalize(worldDirection.xyz))));

	if(angleDifference > outerAngle)
		return 0;

	angleFalloff = (angleDifference - outerAngle) / (innerAngle - outerAngle);
	angleFalloff = clamp(angleFalloff, 0, 1);

	return distanceAttenuation * angleFalloff * diffuseFactor;
}

vec3 GenerateIlluminationValues(void)
{
	vec3 coefficients = vec3(0,0,0);
	float diffI = 0;
	switch(gLight.LightType)
	{
	case 0:
		diffI = GeneratePointLightValues(gLight.Position, gLight.Range, gLight.Falloff);
		break;
	case 1:
		diffI = GenerateSpotLightValues(gLight.Position, gLight.Range, gLight.Falloff,
			gLight.Direction, gLight.InnerAngle, gLight.OuterAngle, gLight.Model);
		break;
	default:
		diffI = 1;
		break;
	}
	diffI *= gLight.Intensity;
	coefficients.x += gLight.Color.x * diffI;
	coefficients.y += gLight.Color.y * diffI;
	coefficients.z += gLight.Color.z * diffI;
	//else
	//{
	//	for(int i = 0; i < numLights; ++i)
	//	{
	//		float diffI = 0;
	//		Light light = Lights[i];
	//		switch(light.LightType)
	//		{
	//		case 0:
	//			diffI = GeneratePointLightValues(light.Position, light.Range, light.Falloff);
	//			break;
	//		case 1:
	//			diffI = GenerateSpotLightValues(light.Position, light.Range, light.Falloff,
	//				light.Direction, light.InnerAngle, light.OuterAngle, light.Model);
	//			break;
	//		default:
	//			diffI = 1;
	//			break;
	//		}
	//		
	//	}
	//}
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
	if(Tmpcolor.a < 0.01)
    discard;
	if(useLight)
	{
		lightValue = GenerateIlluminationValues();
		lightValue = clamp(lightValue, 0, 1);
	}
	
  color = vec4(
	Tmpcolor.x * lightValue.x,
	Tmpcolor.y * lightValue.y,
	Tmpcolor.z * lightValue.z,
	Tmpcolor.w);
}
