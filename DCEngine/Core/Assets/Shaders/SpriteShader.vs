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
uniform int totalColumns;
//Vertical
uniform int currentRow;
uniform int totalRows;

//Flip
uniform int flipx;
uniform int flipy;


void main()
{
  TexCoords = vertex.zw;
  if (flipx == 1)
    TexCoords.x = (1 - TexCoords.x);
  if (flipy == 1)
    TexCoords.y = (1 - TexCoords.y);

	TexCoords.x = (TexCoords.x + currentColumn) / totalColumns;
	TexCoords.y = (TexCoords.y + currentRow) / totalRows;

	vec4 temp = model * vec4(vertex.xy, 0.0, 1.0);
  gl_Position = projection * view * temp;
	WorldCoords = temp.xyz;
	vec4 tempNormal = rotation * vec4(vertex.xy, 0, 0);
	WorldNormal = vec3(tempNormal.xy, tempNormal.z + .1);
}
