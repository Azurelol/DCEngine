// Vertex shader used by 'Sprite'
#version 330 core

// We can store both position and texture-coordinate data in a single 'vec4' variable
// Because both position and texture coordinates contain two floats, we can combine them
// into a single vertex attribute. 
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 TexCoords>

out vec2 TexCoords; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  TexCoords = vertex.zw;  
  gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);

  // Flipping the texture vertically, due to SOIL2 not providing this functionality... 
  //TexCoords = vec2(texCoords.x, 1.0 - texCoords.y);

  if (TexCoords.x > 1)//For debugging. TexCoords.x and TexCoords.y should be smaller than 1;
  {
    gl_Position.x = gl_Position.x + 1;
  }

}