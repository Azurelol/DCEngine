/******************************************************************************/
/*!
\file   InputSFML.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The Input implementation through SFML.

*/
/******************************************************************************/
#pragma once

// STD
#include <memory>
// SFML
#include "..\..\..\Dependencies\include\SFML\Window.hpp"

class WindowSFML;

namespace DCEngine {
  namespace Systems {

    class InputSFML {
    public:
      InputSFML();
      ~InputSFML();

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      void PollEvents();
      void PollKeyboard(sf::Event& event);

      // Observing pointers are pointers which do not keep 
      // a pointed object alive. The simplest kind is a raw pointer.
      sf::Window* _window; 
      sf::Event _event;

      //sf::Keyboard _keyboard;
            
      //sf::Event* _event;
      //sf::Keyboard _keyboard;

    };


  }

}