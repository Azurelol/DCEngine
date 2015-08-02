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
//#include "GLEW\glew.h"
#include "..\..\..\Dependencies\include\GLEW\glew.h"
// GLFW
#define GLFW_BUILD_DLL
//#include "GLFW\glfw3.h"
#include "..\..\..\Dependencies\include\GLFW\glfw3.h"


namespace DCEngine {
  namespace Systems {

    class WindowGLFW : public System {
    public:

      WindowGLFW();
      void Initialize();
      void Update(float dt);
      void Terminate();


    private:
      void PollEvents();
      void ViewportUpdate();
      void PollWindowEvent();
      void PollKeyEvent();
      void PollMouseEvent();
      
      GLFWwindow* _window;      

      int _width = 800;
      int _height = 600;


    };




  } // Systems
} // DCEngine
