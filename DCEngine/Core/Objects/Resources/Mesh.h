/******************************************************************************/
/*!
\file   Mesh.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/13/2015
\brief  Mesh resource class. Contains hardware vertex buffer and index buffer.
*/
/******************************************************************************/
#pragma once
#include "..\Resource.h"

// GLEW
#define GLEW_STATIC
#include "..\..\Dependencies\include\GLEW\glew.h"

namespace DCEngine {

  class Mesh : public Resource {

  public:
    Mesh();

  private:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;


  };


}