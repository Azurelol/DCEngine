// Fragment shader used by 'Sprite'
#version 330 core

// We take a texture and color vector that both affect the final color of the
// fragment. 
in vec2 TexCoords;
out vec4 color;

uniform int isTexture;
uniform sampler2D image;
uniform vec4 spriteColor;

void main() {

  //color = vec4(spriteColor);

  color = vec4(spriteColor) * texture(image, TexCoords);

  /*
  if (isTexture == 1) {
    color = vec4(spriteColor) * texture(image, TexCoords);
  }
  else {
    color = vec4(spriteColor);
  }
  */

  

  //Debug code
  /*
  if (TexCoords.x == 1)
  {
    color.x = color.x + 1;
  }

  */
}
