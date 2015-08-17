// We need to alter the vertex shader to accept texture coordinates as a vertex attrib
// then forward the coordinates to the fragment shader
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

// GLM has matrix types for transform operations. (mat2, mat3, mat4)
uniform mat4 transform;

void main() {

  //gl_Position = vec4(position, 1.0f);
  
  // Added the unfirom and multiplied the position vector with the transform matrix
  // before passing it to gl_Position.
  gl_Position = transform * vec4(position, 1.0f);
  
  ourColor = color;

  /*  The texture is flipped upside-down because OpenGL expects the 0.0 coordinate
      on the y-axis to be on the bottom side of the image, but images usually have 0.0
      at the top of the y-axis. To solve this, we can either:
 
  1.  We can alter the texture coordinates in the vertex data and swap the y values 
      (subtract the y coordinates from 1).
  2.  We can edit the vertex shader to swap the y-coordinate automatically by replacing 
      the TexCoord assignment with TexCoord = vec2(texCoord.x, 1.0f - texCoord.y); */
  TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);

}