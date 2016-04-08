/******************************************************************************/
/*!
\file   Window.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  The abstract interface for the Window class.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
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

  namespace Components {
    class Camera;
  }  

  namespace Systems {
    class Window : public System {      
      friend class Engine;
      friend class GLCameraTutorial;
      friend class InputSFML;
      friend class WindowSFML;
      friend class Components::Camera; 
      // Wow, that's a lot of friends...

    public:
      glm::vec2 getWindowDimensions();      
      void setFullscreen();
      std::unique_ptr<WindowSFML> WindowHandler;      
      
    private:
      GraphicsConfig& Settings;
      std::string Caption = "Daisy Chain Engine v.Segment Presentation";
      unsigned CurrentFramerate;
      
      Window(GraphicsConfig& config, const std::string& caption);
      void Initialize();
      void Subscribe();
      void Update(float dt);
      float FPS();
      void CalculateFPS(float dt);
      void Terminate();
      void StartFrame();
      void EndFrame();

      /* Events */
      void OnWindowFullScreenToggleEvent(Events::WindowFullScreenToggle* event);
			void OnWindowResizeEvent(Events::WindowResize* event);
      void OnEngineExitEvent(Events::EngineExit* event);
      void OnSetWindowCaptionEvent(Events::SetWindowCaption* event);

    };

  } // Systems
} // DCEngine
