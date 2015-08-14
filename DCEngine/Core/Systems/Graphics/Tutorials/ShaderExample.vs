#version version_number

/* GLSL has data types for specifying what kind of variable we want to
work with. It has mose of the default basic types: int, float, double, uint, bool.
It also features two container types that will be used extensively: vectors, matrices. */

/*  GLSL has defined the in and out keywords specifically for the purpose of 
defining inputs and outputs, allowing shaders to specify those using these keywords.
The output variable of one shader will match the input variable of the next shader 
stage. 

If we want to send ata from one shader to the other we'd have to declare an output
in the sending shader and a similar input in the receiving shader. When both types are
equal OpenGL will link those variables together and it is possible to send data
between shaders (this is done when linking a program object) */

/* The vertex shader differs in that it receives input straight from the vertex data.
To define how the vertex data is organized we specify the input variables with location
metadata so we can configure the vertex attributes on the CPU.

The fragment shader is another exception in that it requires a vec4 color output
variable, since fragment shaders need to generate a final output color.
*/

in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

void main(){

// Process input(s) and do graphics stuff

// Output processed stuff to output variable
out_variable_name = processed_stuff;


}