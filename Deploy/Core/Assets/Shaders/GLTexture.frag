#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// The fragment hsader needs access to the texture object. How can it be passed, then?
// GLSL has a built-in data-type for texture objects called a sampler that takes as 
// postfix the texturre type. We can add a texture to the fragment shader by 
// simplify declarign an uniform samplerD
uniform sampler2D ourTexture1;

// Editing the fragment shader to accept another sampler
uniform sampler2D ourTexture2;

void main() {
  // To sample the color of a texture we use GLSL's built-in 'texture' function
  // that takes as its first argument a texture sampler and as its second argument
  // the corresponding texture coordinate. 

  // The 'texture' function then samples the corresponding color value using the texture
  // parameters we set earlier. The output of this fragment shader is then the (filtered)
  // color of the texture at the (interpolated) texture coordinate

  // The final output color is now the combination of two texture lookups.
  // 'mix' takes two input values and linearly interpolates between them based on its
  // third argument. A value of 0.2 will return 80% of the first input color, 20% of second
  color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);

  // Getting funky
  //color = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0f);

}