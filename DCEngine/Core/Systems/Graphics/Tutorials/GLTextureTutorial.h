/******************************************************************************/
/*!
\file   GLTextureTutorial.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  My study of the "Textures" tutorial:
"http://learnopengl.com/#!Getting-started/Textures"

*/
/******************************************************************************/
#pragma once
#include "GLTutorial.h"

namespace Tutorial {

  class GLTextureTutorial : public GLTutorial {

  public:
    void Initialize();
    void Update();
    void Terminate();

  private:
    int width, height;
    unsigned char* image;


    GLuint vertexShader, fragmentShader;
    GLuint VBO, VAO, EBO;
    GLuint shaderProgram;


  };



}