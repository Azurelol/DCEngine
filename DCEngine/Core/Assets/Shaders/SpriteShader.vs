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
//Horizontal
uniform int currentColumn;
//Vertical
uniform int currentRow;
uniform int isAnimaitonActivated;
uniform int haveAnimationButStop;
uniform float rowHeight;
uniform float columnLength;
//Cut
uniform float CutMinX;
uniform float CutMinY;
uniform float CutMaxX;
uniform float CutMaxY;


void main() {
  TexCoords = vertex.zw;
  
  //Animation
  if (isAnimaitonActivated == 1)
  {
	if (TexCoords.x == 1)
	{
	  TexCoords.x = columnLength;
	  TexCoords.x += columnLength * currentColumn;
	}
	if (TexCoords.x == 0)
	{
	  TexCoords.x = 0;
	  TexCoords.x += columnLength * currentColumn;
	}

	if (TexCoords.y == 1)
	{
	  TexCoords.y = rowHeight;
	  TexCoords.y += rowHeight * currentRow;
	}
	if (TexCoords.y == 0)
	{
	  TexCoords.y = 0;
	  TexCoords.y += rowHeight * currentRow;
	}
  }
  else
  {
    if (TexCoords.x == 1)
	{
	  TexCoords.x = CutMaxX;
	}
	if (TexCoords.x == 0)
	{
	  TexCoords.x = CutMinX;
	}

	if (TexCoords.y == 1)
	{
	  TexCoords.y = CutMaxY;
	}
	if (TexCoords.y == 0)
	{
	  TexCoords.y = CutMinY;
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