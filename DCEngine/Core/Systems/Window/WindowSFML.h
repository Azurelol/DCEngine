#pragma once

#include <memory>


#include "..\System.h"
#include "..\..\..\Dependencies\include\SFML\Window.hpp"

/* References:
   http://www.sfml-dev.org/tutorials/2.0/start-vc.php
   http://www.sfml-dev.org/tutorials/2.0/window-opengl.php
*/

namespace DCEngine {
  namespace Systems {
    class WindowSFML : public System {
    public:
      WindowSFML(); // What is ctor?
      void Initialize();
      void Update(float dt);
      void Terminate();
    private:
      sf::Window _Window; // The window
      sf::Context _Context; // OpenGL context
      sf::Event _Event; // Events from the window
      bool _Fullscreen; // Whether screen is fullscreen.
      int _Width;
      int _Height;      

    };

  }

}