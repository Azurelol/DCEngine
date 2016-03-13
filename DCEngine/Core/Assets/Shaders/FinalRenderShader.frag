#version 330

uniform sampler2D LightedFrag;
uniform float Exposure;
in vec2 TexCoords;

out vec4 FragColor;

void main(void)
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(LightedFrag, TexCoords).rgb;
	

	//vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
  //  // Gamma correction 
  //mapped = pow(mapped, vec3(1.0 / gamma));

	// Exposure tone mapping
	vec3 mapped = vec3(1.0) - exp(-hdrColor * Exposure);
	
	// Gamma correction 
	mapped = pow(mapped, vec3(1.0 / gamma));
	
	FragColor = vec4(mapped, 1.0);
}