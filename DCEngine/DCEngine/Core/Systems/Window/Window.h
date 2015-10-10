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

class GLCameraTutorial;

namespace DCEngine {
  class Engine;

  namespace Systems {
    class Window : public System {      
      friend class Engine;
      friend class GLCameraTutorial;
      #if(USE_SFML)
      friend class InputSFML;
      #else
      friend class InputGLFW;
      #endif     

    public:

      void StartFrame();
      void EndFrame();
      // Temporary
      #if(USE_SFML)
      std::unique_ptr<WindowSFML> WindowHandler;
      #else
      std::unique_ptr<WindowGLFW> WindowHandler;
      #endif
      
    private:
      const std::string Caption = "Daisy Chain Engine v.lol";
      const int Width = 1024;
      const int Height = 600;
      
      Window();
      void Initialize();
      void Update(float dt);
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

    };

  } // Systems
} // DCEngine
