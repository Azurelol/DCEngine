/******************************************************************************/
/*!
\file   WindowGLFW.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  The Window implementation through GLFW.

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

    class WindowGLFW {
    public:

      WindowGLFW();
      void Initialize();
      void Update(float dt);
      void Terminate();

      void StartFrame();
      void EndFrame();

      void SetWindowSize(int width, int height);
      void SetWindowCaption(std::string caption);

      GLFWwindow* GetWindow() {
        return _window;
      }


    private:
      void PollEvents();
      void ViewportUpdate();

      //  KEYBOARD INPUT //
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

      std::string _caption;
      GLuint _width = 800;
      GLuint _height = 600;


    };




  } // Systems
} // DCEngine
