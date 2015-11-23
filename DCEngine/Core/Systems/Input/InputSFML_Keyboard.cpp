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
            
      // Create a keyboard pressed event
      auto keyDown = new Events::KeyDown();

      switch (event.key.code) {
      case sf::Keyboard::Escape: // RESERVED: Terminate engine
        Daisy->getSystem<Window>()->WindowHandler->Terminate();
        DispatchEngineExitEvent();        
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
      case sf::Keyboard::F7: // RESERVED: Toggle Test
        keyDown->Key = Keys::F7;
        Daisy->getSystem<Systems::Editor>()->ToggleTest();
        break;
      case sf::Keyboard::F8: // RESERVED: Toggle Editor
        //Daisy->getSystem<Systems::Editor>()->ToggleEditor();
        ToggleEditor();
        keyDown->Key = Keys::F8;
        break;
      case sf::Keyboard::F9:
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


      case sf::Keyboard::W:
        keyDown->Key = Keys::W;
        KeyboardRef->KeyDown_W = true;
        break;
      case sf::Keyboard::S:
        keyDown->Key = Keys::S;
        KeyboardRef->KeyDown_S = true;
        break;
      case sf::Keyboard::A:
        keyDown->Key = Keys::A;
        KeyboardRef->KeyDown_A = true;
        break;
      case sf::Keyboard::D:
        keyDown->Key = Keys::D;
        KeyboardRef->KeyDown_D = true;
        break;
      case sf::Keyboard::Q:
        keyDown->Key = Keys::Q;
        break;
      case sf::Keyboard::E:
        keyDown->Key = Keys::E;
        break;
      case sf::Keyboard::Z:
        keyDown->Key = Keys::Z;
        break;
      case sf::Keyboard::X:
        keyDown->Key = Keys::X;
        break;
	  case sf::Keyboard::F:
		  keyDown->Key = Keys::F;
		  KeyboardRef->KeyDown_F = true;
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

      // Create a keyboard pressed event
      auto keyUp = new Events::KeyUp();

      switch (event.key.code) {
      case sf::Keyboard::Tilde:
        keyUp->Key = Keys::Tilde;
        break;
      case sf::Keyboard::Escape:
        Daisy->getSystem<Window>()->WindowHandler->Terminate();
        KeyboardRef->KeyDown_Escape = false;
        break;
      case sf::Keyboard::Space:
        keyUp->Key = Keys::Space;
        KeyboardRef->KeyDown_Space = false;
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
      case sf::Keyboard::F7: // RESERVED: Toggle Test
        keyUp->Key = Keys::F7;
        Daisy->getSystem<Systems::Editor>()->ToggleTest();        
        break;
      case sf::Keyboard::F8:
        //Daisy->getSystem<Systems::Editor>()->ToggleEditor();
        keyUp->Key = Keys::F8;
        break;
      case sf::Keyboard::F9:
        keyUp->Key = Keys::F9;
        break;
      case sf::Keyboard::F10: // RESERVED: Toggle Fullscreen
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

      case sf::Keyboard::W:
        keyUp->Key = Keys::W;
        KeyboardRef->KeyDown_W = false;
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
      auto fsevent = new Events::FullscreenEnabledEvent();
      Daisy->Dispatch<Events::FullscreenEnabledEvent>(fsevent);
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
  }
}