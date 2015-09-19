#version 330 core

// We take a texture and color vector that both affect the final color of the
// fragment. 
in vec2 TexCoords;
out vec4 color;

uniform Sampler2D image;
// An uniform color vector means we can change the sprite color from game-code
uniform vec3 spriteColor;

void main() {
  color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}
