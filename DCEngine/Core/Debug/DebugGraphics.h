/******************************************************************************/
/*!
@file   DebugGraphics.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

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