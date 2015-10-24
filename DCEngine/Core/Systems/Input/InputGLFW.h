/******************************************************************************/
/*!
\file   InputGLFW.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The Input implementation through GLFW.

*/
/******************************************************************************/
#pragma once

// GLEW
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"
// GLFW
#define GLFW_BUILD_DLL
#include "..\..\..\Dependencies\include\GLFW\glfw3.h"

namespace DCEngine {
  namespace Systems {

    class InputGLFW {
    
    public:
      InputGLFW();

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      GLFWwindow* WindowContext;

      //  KEYBOARD INPUT //
      inline static auto KeyCallback(GLFWwindow* win,
        int key, int scancode,
        int action, int mode) {
        InputGLFW* window = static_cast<InputGLFW*>(glfwGetWindowUserPointer(win));

        switch (key) {
        case GLFW_KEY_ESCAPE:
          if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, GL_TRUE);
            break;
          }
        case GLFW_KEY_F1:
          break;
        }
      }

    }; // InputGLFW


  } // Systems
} // DCEngine

// I want to be able to place the input callback functions in this file

