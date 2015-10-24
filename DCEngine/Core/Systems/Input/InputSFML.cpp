#include "InputSFML.h"

#include "..\..\Engine\Engine.h"
#include "..\Window\WindowSFML.h"
#include <SFML\System\Vector2.hpp>

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
      WindowContext = Daisy->getSystem<Window>(EnumeratedSystem::Window)->WindowHandler->WindowContext.get();
     #endif

      // Should key presses repeat?
      WindowContext->setKeyRepeatEnabled(false);
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
    \brief  Polls for all input events..
    */
    /**************************************************************************/
    void InputSFML::PollEvents() {
      
      // Check all window's events that were triggered since the last iteration
      if (WindowContext->pollEvent(_event) == false)
        return;

      switch (_event.type) {
      case sf::Event::KeyPressed:
        PollKeyPressed(_event);
        break;
      case sf::Event::KeyReleased:
        PollKeyReleased(_event);
        break;
      case sf::Event::MouseButtonPressed:
        PollMouseButtonPressed(_event);
        break;
      case sf::Event::MouseButtonReleased:
        PollMouseButtonReleased(_event);
        break;
        // Don't process other events
      default:
        break;
      }


    }

    /**************************************************************************/
    /*!
    @brief  Polls for keyboard events. Everytime a key is pressed, sends an 
            event to the keyboard interface.
    @param  event A reference to the sf::Event that we pick up the data from.
    */
    /**************************************************************************/
    void InputSFML::PollKeyPressed(sf::Event& event) {

      // Create a keyboard pressed event
      auto keyDown = new Events::KeyDown();
      
      switch (event.key.code) {
      case sf::Keyboard::Escape: // RESERVED: Terminate engine
        Daisy->getSystem<Window>(EnumeratedSystem::Window)->WindowHandler->Terminate();
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

      case sf::Keyboard::W:
        keyDown->Key = Keys::W;
        break;
      case sf::Keyboard::S:
        keyDown->Key = Keys::S;
        break;
      case sf::Keyboard::A:
        keyDown->Key = Keys::A;
        break;
      case sf::Keyboard::D:
        keyDown->Key = Keys::D;
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
      case sf::Keyboard::F8: // RESERVED: Fullscreen
        Daisy->getSystem<Window>(EnumeratedSystem::Window)->WindowHandler->setFullScreen();
        keyDown->Key = Keys::F8;
        break;

        /* Numpad */
      case sf::Keyboard::Add :
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
      // Dispatch the event to the keyboard interface
      Daisy->getKeyboard()->Dispatch<Events::KeyDown>(keyDown);
      delete keyDown;
    }

    /**************************************************************************/
    /*!
    \brief  Polls for keyboard events. Everytime a key is released, sends an
            event to the keyboard interface.
    */
    /**************************************************************************/
    void InputSFML::PollKeyReleased(sf::Event & event) {

      // Create a keyboard pressed event
      auto keyUp = new Events::KeyUp();

      switch (event.key.code) {
      case sf::Keyboard::Escape:        
        Daisy->getSystem<Window>(EnumeratedSystem::Window)->WindowHandler->Terminate();
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

      default:
        break;
      }

      // Dispatch the event to the keyboard interface
      Daisy->getKeyboard()->Dispatch<Events::KeyUp>(keyUp);
      delete keyUp;
    }

    void InputSFML::PollMouseButtonPressed(sf::Event & event) {

      // Create a mouse button pressed event
      auto mouseDown = new Events::MouseDown();
      mouseDown->Position.x = (float)event.mouseButton.x - (WindowContext->getSize().x / 2);
      mouseDown->Position.y = -1 * ((float)event.mouseButton.y - (WindowContext->getSize().y / 2));

      switch (event.mouseButton.button) {

      case sf::Mouse::Button::Left:
        mouseDown->ButtonPressed = MouseButton::Left;
        break;
      case sf::Mouse::Button::Right:
        mouseDown->ButtonPressed = MouseButton::Right;
        break;
      case sf::Mouse::Button::Middle:
        mouseDown->ButtonPressed = MouseButton::Middle;
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
		//mouseUp->x = sf::Mouse::getPosition().x;
    mouseUp->Position.x = (float)event.mouseButton.x - (WindowContext->getSize().x / 2);
    mouseUp->Position.y = -1 * ((float)event.mouseButton.y - (WindowContext->getSize().y / 2));

    switch (event.mouseButton.button) {

    case sf::Mouse::Button::Left:
      mouseUp->ButtonReleased = MouseButton::Left;
      break;
    case sf::Mouse::Button::Right:
      mouseUp->ButtonReleased = MouseButton::Right;
      break;
    case sf::Mouse::Button::Middle:
      mouseUp->ButtonReleased = MouseButton::Middle;
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
    \brief  Terminates.
    */
    /**************************************************************************/
    void InputSFML::Terminate() {
    }



  }

}