// Fragment shader used by 'Sprite'
#version 330 core

// We take a texture and color vector that both affect the final color of the
// fragment. 
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main() {
  color = spriteColor;
  //color = vec4(spriteColor) * texture(image, TexCoords);

  //Debug code
  if (TexCoords.x == 1)
  {
    color.x = color.x + 1;
  }
}
