/******************************************************************************/
/*!
@file	InputSMFL_Keyboard.cpp
@author Connor Tilley, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "InputSFML.h"
// Engine
#include "..\..\Engine\Engine.h"
// Libraries
#include "..\Window\WindowSFML.h"
// GUI Handling
#include "../GUI/ImGuiSFML.h"

namespace DCEngine {
  namespace Systems {

    /* These functions will dispatch engine-events */
    void ToggleFullscreen();
    void ToggleEditor();
    void DispatchEngineExitEvent();
    void DispatchEnginePauseEvent();


    /**************************************************************************/
    /*!
    \brief  Polls for keyboard events for ImGui.
    */
    /**************************************************************************/
    void InputSFML::ImGuiPollKeyPressed(sf::Event & event)
    {
      // Update ImGui
      ImGuiIO& io = ImGui::GetIO();
      io.KeysDown[event.key.code] = true;
      io.KeyCtrl = event.key.control;
      io.KeyShift = event.key.shift;
    }

    /**************************************************************************/
    /*!
    @brief  Polls for keyboard events. Everytime a key is pressed, sends an
    event to the keyboard interface.
    @param  event A reference to the sf::Event that we pick up the data from.
    @todo  Currently there's some hardcoded function calls in this function
    for some systems. In the future, have those systems just listen
    to the key. For that to happen, systems must be able to receive
    events.
    */
    /**************************************************************************/
    void InputSFML::PollKeyPressed(sf::Event& event) {

      // Update ImGui
      ImGuiIO& io = ImGui::GetIO();
      io.KeysDown[event.key.code] = true;
      io.KeyCtrl = event.key.control;
      io.KeyShift = event.key.shift;
            
      // Do nothing if ImGui is currently capturing the keyboard
      if (ImGui::GetIO().WantCaptureKeyboard)
        return;

      // Create a keyboard pressed event
      auto keyDown = new Events::KeyDown();

      switch (event.key.code) {
      case sf::Keyboard::Escape: // RESERVED: Terminate engine
        //Daisy->getSystem<Window>()->WindowHandler->Terminate();
        //DispatchEngineExitEvent(); 
        //DispatchEnginePauseEvent();
        keyDown->Key = Keys::Escape;
        KeyboardRef->KeyDown_Escape = true;
        break;
      case sf::Keyboard::Delete:
        keyDown->Key = Keys::Delete;
        break;
      case sf::Keyboard::BackSpace:
        keyDown->Key = Keys::Backspace;
        break;
      case sf::Keyboard::Return:
        keyDown->Key = Keys::Return;
        break;
      case sf::Keyboard::Tab:
        keyDown->Key = Keys::Tab;
        break;
      case sf::Keyboard::LControl:
        keyDown->Key = Keys::LControl;
        KeyboardRef->KeyDown_LControl = true;
        break;
      case sf::Keyboard::RControl:
        keyDown->Key = Keys::RControl;
        KeyboardRef->KeyDown_RControl = true;
        break;
      case sf::Keyboard::LShift:
        keyDown->Key = Keys::LShift;
        KeyboardRef->KeyDown_LShift = true;
        break;
      case sf::Keyboard::RShift:
        keyDown->Key = Keys::RShift;
        KeyboardRef->KeyDown_RShift = true;
        break;
      case sf::Keyboard::F1:
        keyDown->Key = Keys::F1;
        break;
      case sf::Keyboard::F2:
        keyDown->Key = Keys::F2;
        break;
      case sf::Keyboard::F3:
        keyDown->Key = Keys::F3;
        break;
      case sf::Keyboard::F4:
        keyDown->Key = Keys::F4;
        break;
      case sf::Keyboard::F5:
        keyDown->Key = Keys::F5;
        break;
      case sf::Keyboard::F6:
        keyDown->Key = Keys::F6;
        break;
      case sf::Keyboard::F7:
        keyDown->Key = Keys::F7;
        break;
      case sf::Keyboard::F8:
        keyDown->Key = Keys::F8;
        break;
      case sf::Keyboard::F9:
				DispatchSystemEvents::WindowResize(Vec2(800, 600));
        keyDown->Key = Keys::F9;
        break;
      case sf::Keyboard::F10: // RESERVED: Toggle Fullscreen
        ToggleFullscreen();
        keyDown->Key = Keys::F10;
        break;


      case sf::Keyboard::Up:
        keyDown->Key = Keys::Up;
        break;
      case sf::Keyboard::Down:
        keyDown->Key = Keys::Down;
        break;
      case sf::Keyboard::Left:
        keyDown->Key = Keys::Left;
        break;
      case sf::Keyboard::Right:
        keyDown->Key = Keys::Right;
        break;
      case sf::Keyboard::Space:
        keyDown->Key = Keys::Space;
        KeyboardRef->KeyDown_Space = true;
        break;


	  case sf::Keyboard::Q:
		  keyDown->Key = Keys::Q;
		  KeyboardRef->KeyDown_Q = true;
		  break;
      case sf::Keyboard::W:
		  keyDown->Key = Keys::W;
	      KeyboardRef->KeyDown_W = true;
		  break;
	  case sf::Keyboard::E:
		  keyDown->Key = Keys::E;
		  KeyboardRef->KeyDown_E = true;
		  break;
	  case sf::Keyboard::R:
		  keyDown->Key = Keys::R;
		  KeyboardRef->KeyDown_R = true;
		  break;
	  case sf::Keyboard::T:
		  keyDown->Key = Keys::T;
		  KeyboardRef->KeyDown_T = true;
		  break;
	  case sf::Keyboard::Y:
		  keyDown->Key = Keys::Y;
		  KeyboardRef->KeyDown_Y = true;
		  break;
	  case sf::Keyboard::U:
		  keyDown->Key = Keys::U;
		  KeyboardRef->KeyDown_U = true;
		  break;
	  case sf::Keyboard::I:
		  keyDown->Key = Keys::I;
		  KeyboardRef->KeyDown_I = true;
		  break;
	  case sf::Keyboard::O:
		  keyDown->Key = Keys::O;
		  KeyboardRef->KeyDown_O = true;
		  break;
	  case sf::Keyboard::P:
		  keyDown->Key = Keys::P;
		  KeyboardRef->KeyDown_P = true;
		  break;


	  case sf::Keyboard::A:
		keyDown->Key = Keys::A;
		KeyboardRef->KeyDown_A = true;
		break;
      case sf::Keyboard::S:
        keyDown->Key = Keys::S;
        KeyboardRef->KeyDown_S = true;
        break;
      case sf::Keyboard::D:
        keyDown->Key = Keys::D;
        KeyboardRef->KeyDown_D = true;
        break;
	  case sf::Keyboard::F:
		keyDown->Key = Keys::F;
		KeyboardRef->KeyDown_F = true;
		break;
	  case sf::Keyboard::G:
		  keyDown->Key = Keys::G;
		  KeyboardRef->KeyDown_G = true;
		  break;
	  case sf::Keyboard::H:
		  keyDown->Key = Keys::H;
		  KeyboardRef->KeyDown_H = true;
		  break;
	  case sf::Keyboard::J:
		  keyDown->Key = Keys::J;
		  KeyboardRef->KeyDown_J = true;
		  break;
	  case sf::Keyboard::K:
		  keyDown->Key = Keys::K;
		  KeyboardRef->KeyDown_K = true;
		  break;
	  case sf::Keyboard::L:
		  keyDown->Key = Keys::L;
		  KeyboardRef->KeyDown_L = true;
		  break;

	  case sf::Keyboard::Z:
		  keyDown->Key = Keys::Z;
		  KeyboardRef->KeyDown_Z = true;
		  break;
	  case sf::Keyboard::X:
		  keyDown->Key = Keys::X;
		  KeyboardRef->KeyDown_X = true;
		  break;
	  case sf::Keyboard::C:
		  keyDown->Key = Keys::C;
		  KeyboardRef->KeyDown_C = true;
		  break;
	  case sf::Keyboard::V:
		  keyDown->Key = Keys::V;
		  KeyboardRef->KeyDown_V = true;
		  break;
	  case sf::Keyboard::B:
		  keyDown->Key = Keys::B;
		  KeyboardRef->KeyDown_B = true;
		  break;
	  case sf::Keyboard::N:
		  keyDown->Key = Keys::N;
		  KeyboardRef->KeyDown_N = true;
		  break;
	  case sf::Keyboard::M:
		  keyDown->Key = Keys::M;
		  KeyboardRef->KeyDown_M = true;
		  break;

        /* Numpad */
      case sf::Keyboard::Add:
        keyDown->Key = Keys::Add;
        break;
      case sf::Keyboard::Subtract:
        keyDown->Key = Keys::Subtract;
        break;
      case sf::Keyboard::Num1:
        keyDown->Key = Keys::Num1;
        break;
      case sf::Keyboard::Num2:
        keyDown->Key = Keys::Num2;
        break;
      case sf::Keyboard::Num3:
        keyDown->Key = Keys::Num3;
        break;
      case sf::Keyboard::Num4:
        keyDown->Key = Keys::Num4;
        break;
      case sf::Keyboard::Num5:
        keyDown->Key = Keys::Num5;
        break;
      case sf::Keyboard::Num6:
        keyDown->Key = Keys::Num6;
        break;
      case sf::Keyboard::Num7:
        keyDown->Key = Keys::Num7;
        break;
      case sf::Keyboard::Num8:
        keyDown->Key = Keys::Num8;
        break;
      case sf::Keyboard::Num9:
        keyDown->Key = Keys::Num9;
        break;
	  case sf::Keyboard::Tilde:
		  keyDown->Key = Keys::Tilde;
		  break;

      default:
        break;
      }

      
      // If not paused, dispatch the events
      //if (!Paused)

      // Dispatch the event to the keyboard interface
      Daisy->getKeyboard()->Dispatch<Events::KeyDown>(keyDown);
      delete keyDown;
    }

    /**************************************************************************/
    /*!
    \brief  Polls for keyboard events for ImGui.
    */
    /**************************************************************************/
    void InputSFML::ImGuiPollKeyReleased(sf::Event & event)
    {
      // Update ImGui
      ImGuiIO& io = ImGui::GetIO();
      io.KeysDown[event.key.code] = false;
      io.KeyCtrl = event.key.control;
      io.KeyShift = event.key.shift;
    }

    /**************************************************************************/
    /*!
    \brief  Polls for keyboard events. Everytime a key is released, sends an
    event to the keyboard interface.
    */
    /**************************************************************************/
    void InputSFML::PollKeyReleased(sf::Event & event) {

      // Update ImGui
      ImGuiIO& io = ImGui::GetIO();
      io.KeysDown[event.key.code] = false;
      io.KeyCtrl = event.key.control;
      io.KeyShift = event.key.shift;

      //if (Paused)
      //  return;

      // Do nothing if ImGui is currently capturing the keyboard
      if (ImGui::GetIO().WantCaptureKeyboard)
        return;

      // Create a keyboard pressed event
      auto keyUp = new Events::KeyUp();

      switch (event.key.code) {
      case sf::Keyboard::Tilde:
        keyUp->Key = Keys::Tilde;
        break;
      case sf::Keyboard::Escape:
        //Daisy->getSystem<Window>()->WindowHandler->Terminate();
        KeyboardRef->KeyDown_Escape = false;
        break;
      case sf::Keyboard::Space:
        keyUp->Key = Keys::Space;
        KeyboardRef->KeyDown_Space = false;
        break;
      case sf::Keyboard::Tab:
        keyUp->Key = Keys::Tab;
        break;
      case sf::Keyboard::Delete:
        keyUp->Key = Keys::Delete;
        break;
      case sf::Keyboard::BackSpace:
        keyUp->Key = Keys::Backspace;
        break;
      case sf::Keyboard::Return:
        keyUp->Key = Keys::Return;
        break;
      case sf::Keyboard::LControl:
        keyUp->Key = Keys::LControl;
        KeyboardRef->KeyDown_LControl = false;
        break;
      case sf::Keyboard::RControl:
        keyUp->Key = Keys::RControl;
        KeyboardRef->KeyDown_RControl = false;
        break;
      case sf::Keyboard::LShift:
        keyUp->Key = Keys::LShift;
        KeyboardRef->KeyDown_LShift = false;
        break;
      case sf::Keyboard::RShift:
        keyUp->Key = Keys::RShift;
        KeyboardRef->KeyDown_RShift = false;
        break;
      case sf::Keyboard::F1:
        keyUp->Key = Keys::F1;
        break;
      case sf::Keyboard::F2:
        keyUp->Key = Keys::F2;
        break;
      case sf::Keyboard::F3:
        keyUp->Key = Keys::F3;
        break;
      case sf::Keyboard::F4:
        keyUp->Key = Keys::F4;
        break;

      case sf::Keyboard::F5:
        keyUp->Key = Keys::F5;
        break;
      case sf::Keyboard::F6:
        keyUp->Key = Keys::F6;
        break;
      case sf::Keyboard::F7:
        keyUp->Key = Keys::F7;   
        break;
      case sf::Keyboard::F8:
        keyUp->Key = Keys::F8;
        break;
      case sf::Keyboard::F9:
        keyUp->Key = Keys::F9;
        break;
      case sf::Keyboard::F10:
        keyUp->Key = Keys::F10;
        break;

      case sf::Keyboard::Up:
        keyUp->Key = Keys::Up;
        break;
      case sf::Keyboard::Down:
        keyUp->Key = Keys::Down;
        break;
      case sf::Keyboard::Left:
        keyUp->Key = Keys::Left;
        break;
      case sf::Keyboard::Right:
        keyUp->Key = Keys::Right;
        break;

      case sf::Keyboard::Q:
        keyUp->Key = Keys::Q;
        KeyboardRef->KeyDown_Q = false;
        break;
	  case sf::Keyboard::W:
		  keyUp->Key = Keys::W;
		  KeyboardRef->KeyDown_W = false;
		  break;
	  case sf::Keyboard::E:
		  keyUp->Key = Keys::E;
		  KeyboardRef->KeyDown_E = false;
		  break;
	  case sf::Keyboard::R:
		  keyUp->Key = Keys::R;
		  KeyboardRef->KeyDown_R = false;
		  break;
	  case sf::Keyboard::T:
		  keyUp->Key = Keys::T;
		  KeyboardRef->KeyDown_T = false;
		  break;
	  case sf::Keyboard::Y:
		  keyUp->Key = Keys::Y;
		  KeyboardRef->KeyDown_Y = false;
		  break;
	  case sf::Keyboard::U:
		  keyUp->Key = Keys::U;
		  KeyboardRef->KeyDown_U = false;
		  break;
	  case sf::Keyboard::I:
		  keyUp->Key = Keys::I;
		  KeyboardRef->KeyDown_I = false;
		  break;
	  case sf::Keyboard::O:
		  keyUp->Key = Keys::O;
		  KeyboardRef->KeyDown_O = false;
		  break;
	  case sf::Keyboard::P:
		  keyUp->Key = Keys::P;
		  KeyboardRef->KeyDown_P = false;
		  break;


      case sf::Keyboard::A:
        keyUp->Key = Keys::A;
        KeyboardRef->KeyDown_A = false;
        break;
	  case sf::Keyboard::S:
		  keyUp->Key = Keys::S;
		  KeyboardRef->KeyDown_S = false;
		  break;
      case sf::Keyboard::D:
        keyUp->Key = Keys::D;
        KeyboardRef->KeyDown_D = false;
        break;
	  case sf::Keyboard::F:
		  keyUp->Key = Keys::F;
		  KeyboardRef->KeyDown_F = false;
		  break;
	  case sf::Keyboard::G:
		  keyUp->Key = Keys::G;
		  KeyboardRef->KeyDown_G = false;
		  break;
	  case sf::Keyboard::H:
		  keyUp->Key = Keys::H;
		  KeyboardRef->KeyDown_H = false;
		  break;
	  case sf::Keyboard::J:
		  keyUp->Key = Keys::J;
		  KeyboardRef->KeyDown_J = false;
		  break;
	  case sf::Keyboard::K:
		  keyUp->Key = Keys::K;
		  KeyboardRef->KeyDown_K = false;
		  break;
	  case sf::Keyboard::L:
		  keyUp->Key = Keys::L;
		  KeyboardRef->KeyDown_L = false;
		  break;

	  case sf::Keyboard::Z:
		  keyUp->Key = Keys::Z;
		  KeyboardRef->KeyDown_Z = false;
		  break;
	  case sf::Keyboard::X:
		  keyUp->Key = Keys::X;
		  KeyboardRef->KeyDown_X = false;
		  break;
	  case sf::Keyboard::C:
		  keyUp->Key = Keys::C;
		  KeyboardRef->KeyDown_C = false;
		  break;
	  case sf::Keyboard::V:
		  keyUp->Key = Keys::V;
		  KeyboardRef->KeyDown_V = false;
		  break;
	  case sf::Keyboard::B:
		  keyUp->Key = Keys::B;
		  KeyboardRef->KeyDown_B = false;
		  break;
	  case sf::Keyboard::N:
		  keyUp->Key = Keys::N;
		  KeyboardRef->KeyDown_N = false;
		  break;
	  case sf::Keyboard::M:
		  keyUp->Key = Keys::M;
		  KeyboardRef->KeyDown_M = false;
		  break;

      default:
        break;
      }

      // Dispatch the event to the keyboard interface
      Daisy->getKeyboard()->Dispatch<Events::KeyUp>(keyUp);
      delete keyUp;
    }



    /**************************************************************************/
    /*!
    \brief  Toggles fullscreen on and off.
    */
    /**************************************************************************/
    void ToggleFullscreen()
    {
      auto fsevent = new Events::WindowFullScreenToggle();
      Daisy->Dispatch<Events::WindowFullScreenToggle>(fsevent);
      delete fsevent;

    }

    /**************************************************************************/
    /*!
    \brief  Toggles the Editor on.
    */
    /**************************************************************************/
    void ToggleEditor() {
      auto toggleEditor = new Events::EditorEnabled();
      Daisy->Dispatch<Events::EditorEnabled>(toggleEditor);
      delete toggleEditor;
    }

    /**************************************************************************/
    /*!
    \brief  Dispatches the event signaling the Engine exit.
    */
    /**************************************************************************/
    void DispatchEngineExitEvent()
    {
      auto exitEvent = new Events::EngineExit();
      Daisy->Dispatch<Events::EngineExit>(exitEvent);
      delete exitEvent;      
    }
    void DispatchEnginePauseEvent()
    {
      // Pause the engine (Physics, Input, Events)
      auto pause = new Events::EnginePauseMenu();
      Daisy->Dispatch<Events::EnginePauseMenu>(pause);
      delete pause;
      DCTrace << "InputSFML::DispatchEnginePauseEvent - Dispatching 'EnginePauseMenu' event \n";
    }
  }
}