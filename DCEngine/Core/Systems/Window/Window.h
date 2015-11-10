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
#include "WindowSFML.h"
class InputSFML;

class GLCameraTutorial;

namespace DCEngine {
  class Engine;

  namespace Systems {
    class Window : public System {      
      friend class Engine;
      friend class GLCameraTutorial;
      friend class InputSFML;
      friend class WindowSFML;
      friend class Camera; 
      // Wow, that's a lot of friends...

    public:
      glm::vec2 getWindowDimensions();      
      void setFullscreen();
      std::unique_ptr<WindowSFML> WindowHandler;      
      
    private:
      std::string Caption = "Daisy Chain Engine v.Segment Presentation";
      std::string Version;
      unsigned int Width = 1024;
      unsigned int Height = 600;
      unsigned int Framerate = 60;

      
      Window(std::string& caption, unsigned int framerate, unsigned int width, unsigned int height);
      void Initialize();
      void Update(float dt);
      void Terminate();
      void StartFrame();
      void EndFrame();
      void Subscribe();

      /* Events */
      void OnFullscreenEnabledEvent(Events::FullscreenEnabledEvent* event);

    };

  } // Systems
} // DCEngine
