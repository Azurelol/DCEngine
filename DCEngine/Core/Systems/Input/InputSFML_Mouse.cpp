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
    @brief  Polls for MousePressed evemts. Everytime a key is released, sends an
    event to the keyboard interface.
    */
    /**************************************************************************/
    void InputSFML::PollMouseButtonPressed(sf::Event & event) {

      // Create a mouse button pressed event
      auto mouseDown = new Events::MouseDown();
      mouseDown->Position.x = static_cast<float>(event.mouseButton.x);
      mouseDown->Position.y = static_cast<float>(event.mouseButton.y);

      //mouseDown->Position.x = (float)event.mouseButton.x - (WindowContext->getSize().x / 2);
      //mouseDown->Position.y = -1 * ((float)event.mouseButton.y - (WindowContext->getSize().y / 2));

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
      //mouseUp->x = sf::Mouse::getPosition().x;
      //mouseUp->Position.x = (float)event.mouseButton.x - (WindowContext->getSize().x / 2);
      //mouseUp->Position.y = -1 * ((float)event.mouseButton.y - (WindowContext->getSize().y / 2));

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

    /**************************************************************************/
    /*!
    \brief  Polls for mouse wheel events.
    */
    /**************************************************************************/
    void InputSFML::PollMouseWheelMoved(sf::Event & event)
    {
      // Update ImGui
      //ImGuiIO& io = ImGui::GetIO();
      //io.MouseWheel += (float)event.mouseWheel.delta;
    }
  }
}