/* Each shader begins with a declaration of its version. 
 Since OpenGL 3.3+, the version numbers of GLSL match those of OpenGL
 We also mention we're using core profile functionality 
*/
#version 330 core

/* We declare all the input vertex attributes in the vertex shader with the 
   'in' keyword. Since right now we only care about position data, we only need
   a single vertex attribute. GLSL has a vector datatype that contains 1-4 floats based
   on its postfix digit. 
   Since each vertex has a 3D coordinate we create a vec3 input variable with the name
   position. We also specifically set the location of the input variable via:
   'layout (location = 0)
*/
layout(location = 0) in vec3 position;

/* (!) Graphics programming uses the mathematical concept of a vector quite often.
   A vector in GLSL has a maximum size of 4 and each of its values can be retrieved
   via vec.x, vec.y,  vec.z, vec.w. The 'vec.w' component is not used as a position
   in space (It's 3D, not 4D) but used for perspective division. 
*/

// Specifies a color output to the fragment shader
out vec4 vertexColor; 

void main() {

/* To set the output of the vertex shader, we assign the position data to the predefined
   gl_Position variable which is a 'vec4' behind the scenes. At the end of main whatever
   is set to gl_Position will be used as the output of the vertex shader. 

   Since our input is a vector of size 3, we have to cast this vector of size 4.
   We can do this by inserting the vec3 values inside the vec4 constructor,
   setting the 'w' component to 1.0f (Later to be explained why)
*/
  gl_Position = vec4(position.x, position.y, position.z, 1.0);

  // Set the output variable (that will be used as an input by the fragment shader)
  vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);

}