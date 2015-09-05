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
      //WindowHandler = std::move(Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler);
      //WindowHandler.reset(Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler);
      //_window = Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler->_window;      
      _window = Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler->_window.get();
     #endif    

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
      // Check all window's events that were triggered since the last ieration
      _window->pollEvent(_event);

      if (_event.type == sf::Event::KeyPressed)
        PollKeyboard(_event);
    }

    /**************************************************************************/
    /*!
    \brief  Polls for keyboard events. Everytime a key is pressed, send an event
            to the engine.
    */
    /**************************************************************************/
    void InputSFML::PollKeyboard(sf::Event& event) {

      //if (event.KeyPressed == sf::Keyboard::Escape)
      if (event.key.code == sf::Keyboard::Escape)
        Daisy->GetSystem<Window>(EnumeratedSystem::Window)->WindowHandler->Terminate();


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