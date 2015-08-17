/******************************************************************************/
/*!
\file   GLTextureTutorial.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  My study of the "Textures" tutorial:
        "http://learnopengl.com/#!Getting-started/Textures"
        A texture is a 2D (or 1D, 3D) image used to add detail to an object.

        It also includes some transformations done with matrixes, with the
        GLM library.
        "http://learnopengl.com/#!Getting-started/Transformations"

*/
/******************************************************************************/
#pragma once
#include "GLTutorial.h"

// OpenGL Mathematics for doing transformations
#include "..\..\Dependencies\include\GLM\glm.hpp"
#include "..\..\Dependencies\include\GLM\gtc\matrix_transform.hpp"
#include "..\..\Dependencies\include\GLM\gtc\type_ptr.hpp"

namespace Tutorial {

  class GLTextureTutorial : public GLTutorial {

  public:
    void Initialize();
    void Update();
    void Terminate();

    void ApplyTransformation();
    void GenerateMesh();
    void GenerateTexture(const std::string imagePath, GLuint& texture);

  private:
    int width, height;
    unsigned char* image;
    GLuint texture1, texture2;

    glm::vec4 vec; //!< Define a vec using GLM's built-in vector class
    glm::mat4 transform; //!< // Define a 'mat4', a 4-by-4 identity matrix by default
    DCEngine::ShaderPtr shader;
    
    GLuint vertexShader, fragmentShader;
    GLuint VBO, VAO, EBO;
    GLuint shaderProgram;


  };



}