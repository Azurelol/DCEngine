/******************************************************************************/
/*!
\file   Window.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  The abstract interface for the Window class.

*/
/******************************************************************************/
#include "../System.h"
#pragma once

/* Libraries */
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"

#define USE_SFML 1 // When set to 0, will use GLFW implementation
#define TRACE_ON 1

// Window Handler
#if(USE_SFML)
#include "WindowSFML.h"
class InputSFML;
#else
#include "WindowGLFW.h"
class InputGLFW;
#endif

namespace DCEngine {
  namespace Systems {
    class Window : public System {      

      #if(USE_SFML)
      friend class InputSFML;
      #else
      friend class InputGLFW;
      #endif

    public:
      Window();
      ~Window();
      void Initialize();
      void Update(float dt);
      void Terminate();

      void StartFrame();
      void EndFrame();
      
    private:
      const std::string Caption = "Daisy Chain Engine v.lol";
      const int Width = 1024;
      const int Height = 600;
      
      #if(USE_SFML)
      std::unique_ptr<WindowSFML> WindowHandler;
      #else
      std::unique_ptr<WindowGLFW> WindowHandler;
      #endif


      //std::unique_ptr<WindowGLFW> WindowHandler;
    };


  } // Systems
} // DCEngine
