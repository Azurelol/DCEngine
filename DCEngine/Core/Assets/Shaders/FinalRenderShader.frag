#version 330

uniform sampler2D LightedFrag;
uniform float Exposure;
in vec2 TexCoords;

out vec4 FragColor;

void main(void)
{
	//const float gamma = 2.2;
	vec3 hdrColor = texture(LightedFrag, TexCoords).rgb;
	
	// Exposure tone mapping
	vec3 mapped = vec3(1.0) - exp(-hdrColor * Exposure);
	
	// Gamma correction (pls no)
	//mapped = pow(mapped, vec3(1.0 / gamma));
	
	FragColor = vec4(mapped, 1.0);
}