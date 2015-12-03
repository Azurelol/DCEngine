/*****************************************************************************/
/*!
\file   InputSFML.cpp
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   9/12/2015
\brief  Handling the keyboard and system input.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include <Windows.h>
#include "InputSFML.h"

// Engine
#include "..\..\Engine\Engine.h"
// Libraries
#include "..\Window\WindowSFML.h"
#include <SFML\System\Vector2.hpp>
// GUI Handling
#include "../GUI/ImGuiSFML.h"

namespace DCEngine {

  // Need access to the engine to get a pointer to the GLFWwindow object
  extern std::unique_ptr<Engine> Daisy;

  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  Constructor for the InputSFML class.
    */
    /**************************************************************************/
    InputSFML::InputSFML() {
    }

    InputSFML::~InputSFML() {
    }

    /**************************************************************************/
    /*!
    \brief  Initialize.
    */
    /**************************************************************************/
    void InputSFML::Initialize() {

     #if(USE_SFML)      
      WindowContext = Daisy->getSystem<Window>()->WindowHandler->WindowContext.get();
     #endif

      // Should key presses repeat?
      WindowContext->setKeyRepeatEnabled(false);

      // Store the reference to the mouse and keyboard interface objects
      KeyboardRef = Daisy->getKeyboard();
      MouseRef = Daisy->getMouse();

    }

    /**************************************************************************/
    /*!
    \brief  Update.
    */
    /**************************************************************************/
    void InputSFML::Update(float dt) {
      PollEvents();    
    }

    /**************************************************************************/
    /*!
    @brief  Polls for all input events..
    @todo   Currently intruded upon by ImGui input code.
    */
    /**************************************************************************/
    void InputSFML::PollEvents() {
      
      // Check all window's events that were triggered since the last iteration
      if (WindowContext->pollEvent(_event) == false)
        return;

      // Poll for events
      switch (_event.type) {
	  case sf::Event::Closed:
		  WindowContext->close();
		  Daisy->Stop();
		  break;
      case sf::Event::KeyPressed:
        //ImGuiPollKeyPressed(_event);
        PollKeyPressed(_event);
        break;
      case sf::Event::KeyReleased:
        //ImGuiPollKeyReleased(_event);
        PollKeyReleased(_event);
        break;
      case sf::Event::MouseMoved:
        PollMouseMoved(_event);
        break;
      case sf::Event::MouseButtonPressed:
        Daisy->getSystem<GUI>()->GUIHandler->MousePressed[_event.mouseButton.button] = true;
        PollMouseButtonPressed(_event);
        break;
      case sf::Event::MouseButtonReleased:
        Daisy->getSystem<GUI>()->GUIHandler->MousePressed[_event.mouseButton.button] = false;
        PollMouseButtonReleased(_event);
        break;
      case sf::Event::MouseWheelMoved:
        PollMouseWheelMoved(_event);
        //ImGuiIO& io = ImGui::GetIO();
        //io.MouseWheel += (float)_event.mouseWheel.delta;
        break;
      case sf::Event::TextEntered:
        PollTextEntered(_event);
        break;
		
    // KEYS: alt+tab, ctrl+alt+delete
	  case sf::Event::LostFocus:
		  if (Daisy->getSystem<Window>()->WindowHandler->Mode == WindowMode::Fullscreen)
		  {
			  ShowWindow(WindowContext->getSystemHandle(), SW_MINIMIZE);        
		  }

      // @todo plz fix me
      KeyboardRef->KeyDown_S = false;
      KeyboardRef->KeyDown_W = false;
      KeyboardRef->KeyDown_A = false;
      KeyboardRef->KeyDown_D = false;
      KeyboardRef->KeyDown_D = false;
      KeyboardRef->KeyDown_Space = false;

      DispatchSystemEvents::WindowLostFocus();
		  break;      
	  case sf::Event::GainedFocus:
		  ShowWindow(WindowContext->getSystemHandle(), SW_RESTORE);
      DispatchSystemEvents::WindowGainedFocus();
		  break;


        // Don't process other events
      default:
      // @todo WHAT?? - CTRL - ALT - DELETE COMBO FOR TASK MANAGER
        // Send lose focus event
		  if ((((unsigned short)GetKeyState(VK_CONTROL)) >> 15) && (((unsigned short)GetKeyState(VK_MENU) >> 15)) && (((unsigned short)GetKeyState(VK_MENU) >> 15)))
		  {
			  ShowWindow(WindowContext->getSystemHandle(), SW_MINIMIZE);
        DispatchSystemEvents::WindowLostFocus();
		  }
        break;
      }
    }
    
  /**************************************************************************/
  /*!
  \brief  Polls for text events.
  */
  /**************************************************************************/
  void InputSFML::PollTextEntered(sf::Event & event)
  {
    // Update ImGui
    if (event.text.unicode > 0 && event.text.unicode < 0x10000)
      ImGui::GetIO().AddInputCharacter(event.text.unicode);  
  }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void InputSFML::Terminate() {
    }
  }
}
