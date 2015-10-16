// Vertex shader used by 'Sprite'
#version 330 core

// We can store both position and texture-coordinate data in a single 'vec4' variable
// Because both position and texture coordinates contain two floats, we can combine them
// into a single vertex attribute. 
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 TexCoords>

out vec2 TexCoords; 

//basic
uniform mat4 model;
//camera
uniform mat4 view;
uniform mat4 projection;
//animation
uniform int currentFrame;
uniform int isAnimaitonActivated;
uniform int haveAnimationButStop;
uniform float frameLength;

void main() {
  TexCoords = vertex.zw;
  
  //Animation
  if (isAnimaitonActivated == 1)
  {
	if (TexCoords.x == 1)
	{
	  TexCoords.x = frameLength;
	  TexCoords.x += frameLength * currentFrame;
	}
	if (TexCoords.x == 0)
	{
	  TexCoords.x = 0;
	  TexCoords.x += frameLength * currentFrame;
	}
  }
    
  gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);

  // Flipping the texture vertically, due to SOIL2 not providing this functionality... 
  //TexCoords = vec2(texCoords.x, 1.0 - texCoords.y);

  /*if (TexCoords.x > 1)//For debugging. TexCoords.x and TexCoords.y should be smaller than 1;
  {
    gl_Position.x = gl_Position.x + 1;
  }*/

}