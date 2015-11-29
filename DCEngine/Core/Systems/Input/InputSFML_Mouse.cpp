/******************************************************************************/
/*!
@file   InputSFML_Mouse.cpp
@author Christian Sagel
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
#include <SFML\System\Vector2.hpp>
// GUI Handling
#include "../GUI/ImGuiSFML.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Polls for MouseMoved events.
    */
    /**************************************************************************/
    void InputSFML::PollMouseMoved(sf::Event & event)
    {
      // Send an event with the mouse moved information
      auto mouseUpdate = new Events::MouseUpdate();      
      mouseUpdate->ScreenPosition.x = static_cast<float>(event.mouseMove.x);
      mouseUpdate->ScreenPosition.y = static_cast<float>(event.mouseMove.y);
      Daisy->getMouse()->Dispatch<Events::MouseUpdate>(mouseUpdate);
      delete mouseUpdate;
    }

    /**************************************************************************/
    /*!
    @brief  Polls for MousePressed evemts. Everytime a key is released, sends an
    event to the keyboard interface.
    */
    /**************************************************************************/
    void InputSFML::PollMouseButtonPressed(sf::Event & event) {

      // Create a mouse button pressed event
      auto mouseDown = new Events::MouseDown();
      mouseDown->Position.x = static_cast<float>(event.mouseButton.x);
      mouseDown->Position.y = static_cast<float>(event.mouseButton.y);
      
      switch (event.mouseButton.button) {

      case sf::Mouse::Button::Left:
        mouseDown->ButtonPressed = MouseButton::Left;
        MouseRef->MouseDown_Left = true;
        break;
      case sf::Mouse::Button::Right:
        mouseDown->ButtonPressed = MouseButton::Right;
        MouseRef->MouseDown_Right = true;
        break;
      case sf::Mouse::Button::Middle:
        mouseDown->ButtonPressed = MouseButton::Middle;
        MouseRef->MouseDown_Middle = true;
        break;

      default:
        break;
      }

      // Dispatch the event to the mouse interface
      Daisy->getMouse()->Dispatch<Events::MouseDown>(mouseDown);
      delete mouseDown;
    }

    void InputSFML::PollMouseButtonReleased(sf::Event & event) {

      // Create a mouse button pressed event
      auto mouseUp = new Events::MouseUp();
      mouseUp->Position.x = static_cast<float>(event.mouseButton.x);
      mouseUp->Position.y = static_cast<float>(event.mouseButton.y);

      switch (event.mouseButton.button) {

      case sf::Mouse::Button::Left:
        mouseUp->ButtonReleased = MouseButton::Left;
        MouseRef->MouseDown_Left = false;
        break;
      case sf::Mouse::Button::Right:
        mouseUp->ButtonReleased = MouseButton::Right;
        MouseRef->MouseDown_Right = false;
        break;
      case sf::Mouse::Button::Middle:
        mouseUp->ButtonReleased = MouseButton::Middle;
        MouseRef->MouseDown_Middle = false;
        break;

      default:
        break;
      }

      // Dispatch the event to the mouse interface
      Daisy->getMouse()->Dispatch<Events::MouseUp>(mouseUp);
      delete mouseUp;
    }



    void InputSFML::ImGuiPollMouseButtonPressed(sf::Event & event)
    {
    }

    void InputSFML::ImGuiPollMouseButtonReleased(sf::Event & event)
    {
    }

    /**************************************************************************/
    /*!
    \brief  Polls for mouse wheel events.
    */
    /**************************************************************************/
    void InputSFML::PollMouseWheelMoved(sf::Event & event)
    {
      // Update ImGui
      ImGuiIO& io = ImGui::GetIO();
      io.MouseWheel += (float)event.mouseWheel.delta;

      auto delta = io.MouseWheel;

      // Dispatch the event
      auto mouseScrolled = new Events::MouseScroll();
      mouseScrolled->Delta += static_cast<float>(event.mouseWheel.delta);
      mouseScrolled->Direction = Vec2(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
      Daisy->getMouse()->Dispatch<Events::MouseScroll>(mouseScrolled);
      delete mouseScrolled;

    }
  }
}