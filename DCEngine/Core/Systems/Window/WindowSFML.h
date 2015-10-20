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

    class Window;

    class WindowSFML {
    friend class Window;
    friend class InputSFML;
    public:

      WindowSFML(Window& windowInterface);
      ~WindowSFML();

      void SetWindowSize(int width, int height);
      void SetWindowCaption(std::string caption);

      // (!!!) Temporary public access to the sf::Window object
      sf::Window* GetWindow() {
        return WindowContext.get();
      }

    private:
      Window& WindowInterface;
      std::unique_ptr<sf::Window> WindowContext;
      sf::Event EventObj;
      std::string CaptionText;

      void Initialize();
      void Update(float dt);
      void Terminate();
      void StartFrame();
      void EndFrame();

      // Settings for the underlying OpenGL context
      const int _depthBits = 24; //!< Number of bits per pixel to use for the depth buffer.
      const int _stencilBits = 8; //!< Number of bits per pixel to use for the stencil buffer.
      const int _antiAliasingLevel = 4; //!< The multisampling level.
      const int _majorVersion = 3;
      const int _minorVersion = 0;

    };


  }

}