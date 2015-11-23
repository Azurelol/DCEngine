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