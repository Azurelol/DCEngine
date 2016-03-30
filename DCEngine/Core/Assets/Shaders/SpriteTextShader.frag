/* Fragment Shader used by 'SpriteText'. It takes two uniforms:
   - The mono-colored bitmap image of the glyph
   - The color uniform for adjusting the text's final color.

   First it samples the color value of the bitmap texture. Because
   its texture data is stored in just its red component,
   it samples the r component as the sampled alpha value. By varying
   the color's alpha value the resulting color will be transparent
   for all the glyphs' background colors and non-transparent for the
   actual character pixels.

   We also multiply the RGB colors by the textColor uniform to vary
   the text color.
*/

#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 WorldCoords;
in vec2 TexCoords;

uniform sampler2D text; 
uniform vec4 textColor;

void main()
{
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
  gColor = textColor * sampled;
	gPosition = WorldCoords;
	gNormal = vec3(0,1,0);
}
