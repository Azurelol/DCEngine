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

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text; 
uniform vec4 textColor;

void main() {
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
  color = textColor * sampled;
}
