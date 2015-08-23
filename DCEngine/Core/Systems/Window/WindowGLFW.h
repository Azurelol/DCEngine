/******************************************************************************/
/*!
\file   WindowGLFW.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  The Window implementation through GLFW.
\note   References:
        "http://www.learnopengl.com/#!Getting-Started/Hello-Window"

*/
/******************************************************************************/
#pragma once

// STD
#include <string>
// GLEW
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"
// GLFW
#define GLFW_BUILD_DLL
#include "..\..\..\Dependencies\include\GLFW\glfw3.h"


namespace DCEngine {
  namespace Systems {

    class WindowGLFW {
    friend class InputGLFW;
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


      
      GLFWwindow* _window;      

      std::string _caption;
      GLuint _width = 800;
      GLuint _height = 600;


    };




  } // Systems
} // DCEngine
