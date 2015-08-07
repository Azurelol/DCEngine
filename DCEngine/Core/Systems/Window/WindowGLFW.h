/******************************************************************************/
/*!
\file   WindowGLFW.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief
The x

Functions include:
- Name
Description here.

*/
/******************************************************************************/

#pragma once

#include "../System.h"

// GLEW
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"
// GLFW
#define GLFW_BUILD_DLL
#include "..\..\..\Dependencies\include\GLFW\glfw3.h"


namespace DCEngine {
  namespace Systems {

    class WindowGLFW : public System {
    public:

      WindowGLFW();
      void Initialize();
      void Update(float dt);
      void Terminate();


      void StartFrame();
      void EndFrame();


    private:
      void PollEvents();
      void ViewportUpdate();

      inline static auto KeyCallback(GLFWwindow* win,
        int key, int scancode,
        int action, int mode) {
        WindowGLFW* window = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(win));

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
      
      GLFWwindow* _window;      

      const GLuint _width = 800;
      const GLuint _height = 600;


    };




  } // Systems
} // DCEngine
