#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // IF (APPLY_PROJECTION == TRUE)
  // We read matrix multiplication from right to left
  gl_Position = projection * view * model * vec4(position, 1.0f);

  // IF (APPLY_PROJECTION == FALSE)
  //gl_Position = transform * vec4(position, 1.0f);

  ourColor = color;
  TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}