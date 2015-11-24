/******************************************************************************/
/*!
@file   DebugGraphics.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "DebugGraphics.h"

namespace DCEngine
{
  namespace Debug {

    GLenum CheckOpenGLError()
    {
      GLenum error;
      error = glGetError();
      return error;
    }

    GLenum CheckOpenGLError(std::string message) {
      GLenum error;
      error = glGetError();
      if (error) {
        DCTrace << message << "\n";
      }
      return error;
    }

  }
}