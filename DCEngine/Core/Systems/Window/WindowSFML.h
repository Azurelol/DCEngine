/******************************************************************************/
/*!
\file   WindowGLFW.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  The Window implementation through SFML.
\note   References:
        "http://www.sfml-dev.org/tutorials/2.3/window-window.php"

*/
/******************************************************************************/
#pragma once

// STD
#include <string>
#include <memory>
// GLEW
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"
// SFML
#include "..\..\..\Dependencies\include\SFML\Window.hpp"

namespace DCEngine {
  namespace Systems {

    enum class WindowMode {
      Default = sf::Style::Default,
      Fullscreen = sf::Style::Fullscreen,
    };

    class Window;
    class WindowSFML {
    friend class Window;
    friend class InputSFML;
    public:

      WindowSFML(Window& windowInterface);
      ~WindowSFML();

      // (!!!) Temporary public access to the sf::Window object
      sf::Window* GetWindow() {
        return WindowContext.get();
      }

    private:
      Window& WindowInterface;
      std::unique_ptr<sf::Window> WindowContext;
      sf::Event EventObj;
      WindowMode Mode = WindowMode::Default;
      
      sf::Vector2i getWindowDimensions();
      void setFullScreen();
      void setWindow(WindowMode style);

      void Initialize();
      void Update(float dt);
      void Terminate();
      void StartFrame();
      void EndFrame();
      
      // Settings for the underlying OpenGL context
      sf::ContextSettings ContextSettings;
      const int _depthBits = 24; //!< Number of bits per pixel to use for the depth buffer.
      const int _stencilBits = 8; //!< Number of bits per pixel to use for the stencil buffer.
      const int _antiAliasingLevel = 4; //!< The multisampling level.
      const int _majorVersion = 3;
      const int _minorVersion = 3;

      // FPS
      sf::Clock Clock;
      float LastTime;
      
    };


  }

}