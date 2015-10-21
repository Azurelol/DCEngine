#pragma once
#define GLEW_STATIC
#include <GLEW\glew.h>
#include "Debug.h"


namespace DCEngine
{
  namespace Debug {

    GLenum CheckOpenGLError();
    GLenum CheckOpenGLError(std::string message);

  }

}