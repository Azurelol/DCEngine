/******************************************************************************/
/*!
\file   GLTriangle.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  My study of the "Hello Triangle" tutorial:
        "http://learnopengl.com/#!Getting-started/Hello-Triangle"

*/
/******************************************************************************/

#pragma once

#include "GLTutorial.h" // Tutorial for learning

namespace DCEngine {
  namespace Tutorial {

    class GLTriangle : public GLTutorial {

    public:
      ~GLTriangle() {};

      void Initialize();
      void Update();
      void Terminate();

    private:

      GLuint vertexShader, fragmentShader;
      GLuint VBO, VAO, EBO;
      GLuint shaderProgram;

    };
  }

}