/******************************************************************************/
/*!
@file   InputSFML.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/22/2015
@brief  The Input implementation through SFML.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
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
    
    class Input;
    class ImGuiSFML;
    class InputSFML {
      friend class ImGuiSFML;
      friend class Input;
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
      void PollMouseMoved(sf::Event& event);
      void PollMouseButtonPressed(sf::Event& event);
	    void PollMouseButtonReleased(sf::Event& event);
      void PollMouseWheelMoved(sf::Event& event);
      void PollTextEntered(sf::Event& event);

      void ImGuiPollKeyPressed(sf::Event& event);
      void ImGuiPollKeyReleased(sf::Event& event);
      void ImGuiPollMouseButtonPressed(sf::Event& event);
      void ImGuiPollMouseButtonReleased(sf::Event& event);


      

      // Observing pointers are pointers which do not keep 
      // a pointed object alive. The simplest kind is a raw pointer.
      bool Paused;
      sf::Window* WindowContext; 
      sf::Event _event;
      Keyboard* KeyboardRef;
      Mouse* MouseRef;
      bool RepeatInput = true;

    };


  }

}