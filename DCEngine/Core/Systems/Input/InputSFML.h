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
  
  class Keyboard;
  class Mouse;

  namespace Systems {
    
    class ImGuiSFML;
    class InputSFML {
      friend class ImGuiSFML;
    public:
      InputSFML();
      ~InputSFML();

      void Initialize();
      void Update(float dt);
      void Terminate();

    private:
      void PollEvents();
      void PollKeyPressed(sf::Event& event);
      void PollKeyReleased(sf::Event& event);
      void PollMouseButtonPressed(sf::Event& event);
	    void PollMouseButtonReleased(sf::Event& event);

      // Observing pointers are pointers which do not keep 
      // a pointed object alive. The simplest kind is a raw pointer.
      sf::Window* WindowContext; 
      sf::Event _event;
      Keyboard* KeyboardRef;
      Mouse* MouseRef;
      bool RepeatInput = true;

    };


  }

}