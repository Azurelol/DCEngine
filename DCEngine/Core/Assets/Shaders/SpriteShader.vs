// Vertex shader used by 'Sprite'
#version 330 core

// We can store both position and texture-coordinate data in a single 'vec4' variable
// Because both position and texture coordinates contain two floats, we can combine them
// into a single vertex attribute. 
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 TexCoords>


out vec2 TexCoords;
out vec3 WorldCoords; 
out vec3 WorldNormal;

//basic
uniform mat4 model; //the model to world matrix
uniform mat4 rotation; //just rotation matrix
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
//Flip
uniform int flipx;
uniform int flipy;


void main() {
  TexCoords = vertex.zw;
  if (flipx == 1)
  {
    TexCoords.x = (1 - TexCoords.x);
  }
  if (flipy == 1)
  {
    TexCoords.y = (1 - TexCoords.y);
  }
  
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
		  TexCoords.y = 1;
		  TexCoords.y -= rowHeight * currentRow;
		}
		if (TexCoords.y == 0)
		{
		  TexCoords.y = 1 - rowHeight;
		  TexCoords.y -= rowHeight * currentRow;
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

	vec4 temp = model * vec4(vertex.xy, 0.0, 1.0);
  gl_Position = projection * view * temp;
	WorldCoords = temp.xyz;
	vec4 tempNormal = rotation * vec4(vertex.xy, 0, 0);
	WorldNormal = vec3(tempNormal.xy, tempNormal.z + .1);
}
