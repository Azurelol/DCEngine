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

    class WindowSFML {
    friend class InputSFML;
    public:
      WindowSFML();
      ~WindowSFML();

      void Initialize();
      void Update(float dt);
      void Terminate();

      void StartFrame();
      void EndFrame();

      void SetWindowSize(int width, int height);
      void SetWindowCaption(std::string caption);

    private:
      void ViewportUpdate();

      std::unique_ptr<sf::Window> _window;
      sf::Event _event;

      //sf::Window* _window; //!< The window context.
      //sf::Event _event; //!< Handles events.

      std::string _caption;

      // Settings for the underlying OpenGL context
      GLuint _width = 800;
      GLuint _height = 600;
      const int _depthBits = 24; //!< Number of bits per pixel to use for the depth buffer.
      const int _stencilBits = 8; //!< Number of bits per pixel to use for the stencil buffer.
      const int _antiAliasingLevel = 4; //!< The multisampling level.
      const int _majorVersion = 3;
      const int _minorVersion = 0;

    };


  }

}