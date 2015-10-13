#include "DebugGraphics.h"

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
      trace << message << "\n";
    }
    return error;    
  }

}