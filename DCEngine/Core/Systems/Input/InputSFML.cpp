#include "InputSFML.h"

#include "..\..\Engine\Engine.h"
#include "..\Window\WindowSFML.h"

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
      _window = Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler->_window.get();
     #endif

      // Should key presses repeat?
      _window->setKeyRepeatEnabled(false);
     

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
      if (_window->pollEvent(_event) == false)
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
      // Don't process other events
      default: 
        break;
      }


    }

    /**************************************************************************/
    /*!
    \brief  Polls for keyboard events. Everytime a key is pressed, sends an 
            event to the keyboard interface.
    */
    /**************************************************************************/
    void InputSFML::PollKeyPressed(sf::Event& event) {

      // Create a keyboard pressed event
      auto keyDown = new Events::KeyDown();
      
      switch (event.key.code) {
      case sf::Keyboard::Escape:
        Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler->Terminate();
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

      }
      // Dispatch the event to the keyboard interface
      Daisy->GetKeyboard()->Dispatch<Events::KeyDown>(keyDown);
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
        
        Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler->Terminate();
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

      }

      // Dispatch the event to the keyboard interface
      Daisy->GetKeyboard()->Dispatch<Events::KeyUp>(keyUp);
    }

    void InputSFML::PollMouseButtonPressed(sf::Event & event) {

      // Create a mouse button pressed event
      auto mouseDown = new Events::MouseDown();
      //if (event.key.code == sf::Mouse::Left)

      // Dispatch the event to the mouse interface
      Daisy->GetMouse()->Dispatch<Events::MouseDown>(mouseDown);
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