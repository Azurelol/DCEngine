#version 330 core
// We take a texture and color vector that both affect the final color of the
// fragment. 
struct Light
{
	bool Visible;
	bool VisibilityCulling;
	bool VisibilityEvents;
	bool CastShadows;
	bool Diffuse;
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
	float CullLight;
};

uniform Light gLight;
uniform bool useLight;

uniform sampler2D gWorldCoords;
uniform sampler2D gWorldNormal;
uniform sampler2D gColor;

in vec2 gTexCoords;

layout (location = 0) out vec4 FragColor;

float GenerateZ0DiffuseFactor(vec3 fragPos, vec3 fragNormal, vec3 lightPosition)
{
	if(lightPosition.z == 0.0 && fragPos.z == 0.0)
	{
		vec3 lightVector = normalize(lightPosition - fragPos);
		float luminence = dot(lightVector, fragNormal) + .2;
		if(luminence < 0)
			return 0;
		else
			return luminence;
	}
	else
		return 1.0;
}

float GenerateFalloffFactor(float distance, float range, float falloff)
{
	if(distance <= range)
		if(falloff > 0.0)
			return (1.0 - (distance / range)) * exp(-falloff);
		else
			return 1.0;
	else return 0.0;
}

float GeneratePointLightValues(vec3 fragPos, vec3 fragNormal, Light light)
{
	float distance = length(fragPos - light.Position);
	float diffuseFactor = 1;
	if(light.Diffuse)
		diffuseFactor = GenerateZ0DiffuseFactor(fragPos, fragNormal, light.Position);
	return GenerateFalloffFactor(distance, light.Range, light.Falloff) * diffuseFactor;
}

float GenerateSpotLightValues(vec3 fragPos, vec3 fragNormal, Light light)
{
	float diffuseFactor = 1;
	if(light.Diffuse)
		diffuseFactor = GenerateZ0DiffuseFactor(fragPos, fragNormal, light.Position);
	float distance = length(fragPos - light.Position);
	float distanceAttenuation = GenerateFalloffFactor(distance, light.Range, light.Falloff);

	vec3 lightVector = (fragPos - light.Position) / distance;
	float angleFalloff = 0.0;
	vec4 worldDirection = light.Model * vec4(light.Direction.xyz, 0.0);
	float angleDifference = abs(acos(dot(lightVector, normalize(worldDirection.xyz))));

	if(angleDifference > light.OuterAngle)
		return 0.0;

	if(light.InnerAngle < light.OuterAngle)
		angleFalloff = (angleDifference - light.OuterAngle) / (light.InnerAngle - light.OuterAngle);
	else
		angleFalloff = 1;

	return distanceAttenuation * angleFalloff * diffuseFactor;
}

vec3 GenerateIlluminationValues(vec3 fragPos, vec3 fragNormal)
{
	vec3 coefficients = vec3(0,0,0);
	float diffI = 0;
	switch(gLight.LightType)
	{
	case 0:
		diffI = GeneratePointLightValues(fragPos, fragNormal, gLight);
		break;
	case 1:
		diffI = GenerateSpotLightValues(fragPos, fragNormal, gLight);
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
		if(gLight.CullLight == 0 || (gLight.Position.z + gLight.CullLight <= fragPos.z))
			lightValue = GenerateIlluminationValues(fragPos, normal);
		else
			lightValue = vec3(0);
	FragColor = color * vec4(lightValue, 1);
}