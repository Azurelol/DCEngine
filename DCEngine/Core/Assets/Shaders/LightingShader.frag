#version 330 core
// We take a texture and color vector that both affect the final color of the
// fragment. 
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

uniform Light gLight;
uniform bool useLight;

uniform sampler2D gWorldCoords;
uniform sampler2D gWorldNormal;
uniform sampler2D gColor;

in vec2 gTexCoords;

out vec4 gFragColor;

float GenerateZ0DiffuseFactor(vec3 fragPos, vec3 fragNormal, vec3 lightPosition)
{
	if(lightPosition.z == 0.0 && fragPos.z == 0.0)
	{
		vec3 lightVector = normalize(lightPosition - fragPos);
		float diffuseFactor = (dot(lightVector, fragNormal) + .2) / 2;
		return clamp(diffuseFactor, 0.0, 1.0) * .9 + .1;
	}
	else
		return 1.0;
}

float GenerateFalloffFactor(float distance, float range, float falloff)
{
	if(distance <= range)
		if(falloff > 0.0)
			return clamp((1.0 - distance/range) / (falloff * falloff), 0, 1);
		else
			return 1.0;
	else return 0.0;
}

float GeneratePointLightValues(vec3 fragPos, vec3 fragNormal, vec3 lightPosition, float range, float falloff)
{
	float distance = length(fragPos - lightPosition);
	return GenerateFalloffFactor(distance, range, falloff)
	 * GenerateZ0DiffuseFactor(fragPos, fragNormal, lightPosition);
}

float GenerateSpotLightValues(vec3 fragPos, vec3 fragNormal, vec3 lightPosition, float lightRange, float lightFalloff, vec3 lightDirection,
	float lightInnerAngle, float lightOuterAngle, mat4 model)
{
	float diffuseFactor = GenerateZ0DiffuseFactor(fragPos, fragNormal, lightPosition);
	float distance = length(fragPos - lightPosition);
	float distanceAttenuation = GenerateFalloffFactor(distance, lightRange, lightFalloff);

	vec3 lightVector = (fragPos - lightPosition) / distance;
	float angleFalloff = 0.0;
	vec4 worldDirection = model * vec4(lightDirection.xyz, 0.0);
	float angleDifference = abs(acos(dot(lightVector, normalize(worldDirection.xyz))));

	if(angleDifference > lightOuterAngle)
		return 0.0;

	if(lightInnerAngle < lightOuterAngle)
	{
		angleFalloff = (angleDifference - lightOuterAngle) / (lightInnerAngle - lightOuterAngle);
		angleFalloff = clamp(angleFalloff, 0.0, 1.0);
	}
	else
	{
		angleFalloff = 1;
	}

	return distanceAttenuation * angleFalloff * diffuseFactor;
}

vec3 GenerateIlluminationValues(vec3 fragPos, vec3 fragNormal)
{
	vec3 coefficients = vec3(0,0,0);
	float diffI = 0;
	switch(gLight.LightType)
	{
	case 0:
		diffI = GeneratePointLightValues(fragPos, fragNormal, gLight.Position, gLight.Range, gLight.Falloff);
		break;
	case 1:
		diffI = GenerateSpotLightValues(fragPos, fragNormal, gLight.Position, gLight.Range, gLight.Falloff,
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
	return coefficients;
}

void main()
{
	vec3 lightValue = vec3(1);
	vec3 fragPos = texture(gWorldCoords, gTexCoords).rgb;
	vec3 normal = texture(gWorldNormal, gTexCoords).rgb;
	vec4 color = texture(gColor, gTexCoords);
	if(useLight)
	{
		lightValue = GenerateIlluminationValues(fragPos, normal);
		lightValue = clamp(lightValue, 0, 1);
	}
	gFragColor = color * vec4(lightValue, 1);
}